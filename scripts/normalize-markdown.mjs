import fs from 'node:fs/promises'
import path from 'node:path'

const rootDir = process.cwd()
const checkOnly = process.argv.includes('--check')
const ignoredDirs = new Set(['node_modules', '.git', '.vitepress'])
const ignoredSubDirs = new Set(['dist', 'cache'])

async function walk(dir) {
  const entries = await fs.readdir(dir, { withFileTypes: true })
  const files = []

  for (const entry of entries) {
    const fullPath = path.join(dir, entry.name)

    if (entry.isDirectory()) {
      if (ignoredDirs.has(entry.name)) {
        continue
      }

      if (path.basename(dir) === '.vitepress' && ignoredSubDirs.has(entry.name)) {
        continue
      }

      files.push(...(await walk(fullPath)))
      continue
    }

    if (entry.isFile() && entry.name.endsWith('.md')) {
      files.push(fullPath)
    }
  }

  return files
}

function formatDisplayMath(content) {
  const compactLines = content
    .split('\n')
    .map((line) => line.trim())
    .filter(Boolean)

  if (compactLines.length === 0) {
    return ['$$', '$$']
  }

  if (compactLines.length === 1) {
    const pieces = compactLines[0]
      .split(/\s{2,}/)
      .map((item) => item.trim())
      .filter(Boolean)

    if (pieces.length > 1) {
      return [
        '$$',
        ...pieces.map((piece, index) => (index < pieces.length - 1 ? `${piece} \\\\` : piece)),
        '$$'
      ]
    }
  }

  return ['$$', ...compactLines, '$$']
}

function normalizeInlineMath(line) {
  const normalizedLine = line.replace(/(?<!\$)\$([^$\n]+?)\$(?!\$)/g, (_, content) => {
    const normalized = content.trim().replace(/\s{2,}/g, ' ')
    return `$${normalized}$`
  })

  return normalizedLine.replace(/([一-龥A-Za-z0-9])(\$[^$\n]+\$)([一-龥A-Za-z0-9])/gu, '$1 $2 $3')
}

function normalizeHeading(line) {
  const headingMatch = line.match(/^(#{1,6})\s+(.*)$/)
  if (!headingMatch) {
    return line
  }

  const [, marks, rawTitle] = headingMatch
  const title = rawTitle
    .trim()
    .replace(/^([一二三四五六七八九十]+、)\s+/u, '$1')
    .replace(/^(\d+(?:\.\d+)*)\s+/u, '$1 ')

  return `${marks} ${title}`
}

function normalizeChinesePunctuation(line) {
  const trimmed = line.trim()
  if (
    !trimmed ||
    trimmed.startsWith('|') ||
    trimmed.startsWith('>') ||
    trimmed.startsWith('![') ||
    trimmed.startsWith('<img') ||
    trimmed.startsWith('```') ||
    trimmed === '$$'
  ) {
    return line
  }

  return line
    .replace(/([一-龥）】」”*])\.(?=\s|$|[一-龥])/gu, '$1。')
    .replace(/([一-龥）】」”*]),(?=\s|$|[一-龥])/gu, '$1，')
    .replace(/([A-Za-z]{2,})\.(?=[一-龥])/gu, '$1。')
}

function normalizeMarkdown(text) {
  const lines = text.replace(/\r\n/g, '\n').split('\n')
  const output = []
  let inFence = false
  let inMath = false
  let mathBuffer = []

  for (let i = 0; i < lines.length; i += 1) {
    let line = lines[i].replace(/[ \t]+$/g, '')
    const trimmed = line.trim()

    if (trimmed.startsWith('```')) {
      inFence = !inFence
      output.push(line)
      continue
    }

    if (inFence) {
      output.push(line)
      continue
    }

    if (trimmed === '$$') {
      if (!inMath) {
        inMath = true
        mathBuffer = []
      } else {
        output.push(...formatDisplayMath(mathBuffer.join('\n')))
        inMath = false
        mathBuffer = []
      }
      continue
    }

    if (!inMath && /^\$\$.+\$\$$/.test(trimmed)) {
      const content = trimmed.slice(2, -2).trim()
      output.push(...formatDisplayMath(content))
      continue
    }

    if (!inMath) {
      const inlineDisplayMathMatch = line.match(/^(.*?)\$\$(.+?)\$\$(.*)$/)
      if (inlineDisplayMathMatch) {
        const [, prefix, content, suffix] = inlineDisplayMathMatch
        const normalizedPrefix = normalizeChinesePunctuation(
          normalizeInlineMath(normalizeHeading(prefix.trimEnd()))
        ).trim()
        const normalizedSuffix = normalizeChinesePunctuation(
          normalizeInlineMath(normalizeHeading(suffix.trimStart()))
        ).trim()

        if (normalizedPrefix) {
          output.push(normalizedPrefix)
        }

        output.push(...formatDisplayMath(content))

        if (normalizedSuffix) {
          output.push(normalizedSuffix)
        }
        continue
      }
    }

    if (inMath) {
      mathBuffer.push(line)
      continue
    }

    line = normalizeHeading(line)
    line = normalizeInlineMath(line)
    line = normalizeChinesePunctuation(line)
    output.push(line)
  }

  if (inMath) {
    output.push(...formatDisplayMath(mathBuffer.join('\n')))
  }

  const normalized = output.join('\n').replace(/\n{3,}/g, '\n\n')
  return normalized.endsWith('\n') ? normalized : `${normalized}\n`
}

async function main() {
  const files = await walk(rootDir)
  const changedFiles = []

  for (const file of files) {
    const source = await fs.readFile(file, 'utf8')
    const normalized = normalizeMarkdown(source)

    if (normalized !== source) {
      changedFiles.push(path.relative(rootDir, file))

      if (!checkOnly) {
        await fs.writeFile(file, normalized, 'utf8')
      }
    }
  }

  if (checkOnly && changedFiles.length > 0) {
    console.error('Markdown normalization required:')
    for (const file of changedFiles) {
      console.error(`- ${file}`)
    }
    process.exitCode = 1
    return
  }

  if (!checkOnly) {
    console.log(`Normalized ${changedFiles.length} Markdown files.`)
  }
}

main().catch((error) => {
  console.error(error)
  process.exit(1)
})
