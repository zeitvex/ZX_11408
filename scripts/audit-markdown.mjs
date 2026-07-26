/**
 * 知识库健康检查：一次性扫出会导致渲染错误或影响阅读的问题。
 *
 * 用法：
 *   npm run audit          仅汇总
 *   npm run audit -- -v    附带问题清单
 *
 * 退出码非 0 表示存在需要修复的问题，可用于 CI。
 */
import fs from 'node:fs/promises'
import path from 'node:path'
import { pathToFileURL } from 'node:url'

const rootDir = process.cwd()
const verbose = process.argv.includes('-v') || process.argv.includes('--verbose')
const ignoredDirs = new Set(['node_modules', '.git', '.vitepress'])

const DOLLAR = '$'

async function walk(dir) {
  const entries = await fs.readdir(dir, { withFileTypes: true })
  const files = []
  for (const entry of entries) {
    const full = path.join(dir, entry.name)
    if (entry.isDirectory()) {
      if (ignoredDirs.has(entry.name)) continue
      files.push(...(await walk(full)))
    } else if (entry.isFile() && entry.name.endsWith('.md')) {
      files.push(full)
    }
  }
  return files
}

// 用等长空格屏蔽代码块/块级公式/行内代码，保持字符偏移不变
function maskNonProse(text) {
  const blank = (m) => ' '.repeat(m.length)
  return text
    .replace(/```[\s\S]*?```/g, blank)
    .replace(/\$\$[\s\S]*?\$\$/g, blank)
    .replace(/`[^`\n]*`/g, blank)
}

function unescapedDollarPositions(text) {
  const out = []
  for (let i = 0; i < text.length; i += 1) {
    if (text[i] === DOLLAR && (i === 0 || text[i - 1] !== '\\')) out.push(i)
  }
  return out
}

const CHECKS = [
  '公式定界符内侧空格',
  '跨行/不配对公式',
  'LaTeX 下标被转义',
  '表格被空行截断',
  '正文 f (x) 空格',
  '单文件多个 H1',
  '标题编号重复',
  '标题缺少空行',
  '块公式被包进列表',
  '非标准 LaTeX 命令',
  '\\limits 用法错误',
  '代码块缺语言标注',
  '图片链接失效'
]

// MathJax 默认不认识、会导致公式渲染失败的自造命令
const NONSTANDARD_MACROS = ['part', 'R', 'N', 'C', 'empty', 'and', 'or']

async function audit() {
  const files = (await walk(rootDir)).sort()
  const counts = Object.fromEntries(CHECKS.map((k) => [k, 0]))
  const hits = Object.fromEntries(CHECKS.map((k) => [k, []]))

  const record = (key, file, note) => {
    counts[key] += 1
    hits[key].push(note ? `${file}  ${note}` : file)
  }

  for (const file of files) {
    const rel = path.relative(rootDir, file).split(path.sep).join('/')
    const text = await fs.readFile(file, 'utf8')
    const lines = text.split('\n')
    const masked = maskNonProse(text)

    // 行内公式定界符内侧空格 —— MathJax 不会渲染
    const positions = unescapedDollarPositions(masked)
    for (let i = 0; i + 1 < positions.length; i += 2) {
      const inner = text.slice(positions[i] + 1, positions[i + 1])
      if (inner.includes('\n')) continue
      if (/^[ \t]/.test(inner) || /[ \t]$/.test(inner)) {
        record('公式定界符内侧空格', rel)
      }
    }

    // 公式跨行 —— 同样不会渲染
    let inFence = false
    let inBlock = false
    lines.forEach((line, idx) => {
      const trimmed = line.trim()
      if (trimmed.startsWith('```')) {
        inFence = !inFence
        return
      }
      if (inFence) return
      if (trimmed === '$$') {
        inBlock = !inBlock
        return
      }
      if (inBlock) return
      const stripped = line.replace(/`[^`\n]*`/g, ' ').split('$$').join('')
      if (unescapedDollarPositions(stripped).length % 2 === 1) {
        record('跨行/不配对公式', rel, `L${idx + 1}`)
      }
    })

    // 公式内的下标被 Markdown 转义/改写
    for (const m of masked.matchAll(/\$([^$\n]+)\$/g)) {
      if (m[1].includes('\\_') || /\*\{/.test(m[1])) {
        record('LaTeX 下标被转义', rel)
      }
    }

    // 表格中间的空行会把表格拆成多个段落
    const isRow = (l) => /^\s*\|.*\|\s*$/.test(l)
    for (let i = 1; i + 1 < lines.length; i += 1) {
      if (!lines[i].trim() && isRow(lines[i - 1]) && isRow(lines[i + 1])) {
        record('表格被空行截断', rel, `L${i + 1}`)
      }
    }

    // 函数名与括号之间被插入空格
    const fnSpaces = masked.match(/(?<![\\\w])[a-zA-Z] \(/g)
    if (fnSpaces) {
      counts['正文 f (x) 空格'] += fnSpaces.length
      hits['正文 f (x) 空格'].push(`${rel} (${fnSpaces.length})`)
    }

    // 每个页面应当只有一个一级标题
    inFence = false
    let h1 = 0
    for (const line of lines) {
      if (line.trim().startsWith('```')) {
        inFence = !inFence
        continue
      }
      if (!inFence && line.startsWith('# ')) h1 += 1
    }
    if (h1 > 1) record('单文件多个 H1', rel, `(${h1} 个)`)

    // 重复编号，如 "### 1. 一、xxx"：多为编号脚本被重复执行所致
    lines.forEach((line, idx) => {
      const m = line.match(/^#{2,6}\s+(.*)$/)
      if (!m) return
      const title = m[1].trim()
      if (/^\d+[.、]\s*[一二三四五六七八九十]+、/.test(title) ||
          /^[一二三四五六七八九十]+、\s*\d+[.、]/.test(title)) {
        record('标题编号重复', rel, `L${idx + 1} ${title.slice(0, 30)}`)
      }
    })

    // 代码块语言标注（闭合围栏不计入）
    for (let i = 0; i < lines.length; i += 1) {
      const open = lines[i].match(/^\s*```(.*)$/)
      if (!open) continue
      if (!open[1].trim()) record('代码块缺语言标注', rel, `L${i + 1}`)
      let j = i + 1
      while (j < lines.length && !/^\s*```\s*$/.test(lines[j])) j += 1
      i = j
    }

    // 标题上下缺空行；以及被列表项包住的块公式
    // （后者会让规范化脚本的公式状态错位，导致其后内容被静默跳过）
    inFence = false
    let frontMatterEnd = -1
    if (lines[0] === '---') {
      frontMatterEnd = lines.indexOf('---', 1)
    }
    lines.forEach((line, idx) => {
      if (line.trim().startsWith('```')) {
        inFence = !inFence
        return
      }
      if (inFence || idx <= frontMatterEnd) return

      if (/^#{1,6}\s+\S/.test(line)) {
        if (idx > 0 && lines[idx - 1].trim()) {
          record('标题缺少空行', rel, `L${idx + 1} 前`)
        }
        if (idx + 1 < lines.length && lines[idx + 1].trim()) {
          record('标题缺少空行', rel, `L${idx + 1} 后`)
        }
      }

      if (/^\s*(?:[-*+]|\d+[.)])\s+\$\$\s*$/.test(line)) {
        record('块公式被包进列表', rel, `L${idx + 1}`)
      }
    })

    // 自造的 LaTeX 命令（如 \part、\R），MathJax 不认识会导致公式渲染失败。
    // 先把矩阵换行 \\ 屏蔽掉，否则其后的字母会被误判为命令名。
    const mathText = masked.replace(/\\\\/g, '  ')
    for (const name of NONSTANDARD_MACROS) {
      const pattern = new RegExp(`\\\\${name}(?![A-Za-z])`, 'g')
      const found = mathText.match(pattern)
      if (found) {
        counts['非标准 LaTeX 命令'] += found.length
        hits['非标准 LaTeX 命令'].push(`${rel}  \\${name} × ${found.length}`)
      }
    }

    // \limits 必须紧跟算符：正确写法是 \iint\limits_{D}，
    // 写成 \iint_\limits{D} 会让 MathJax 报 "Misplaced \limits"。
    const badLimits = mathText.match(/[_^]\\limits/g)
    if (badLimits) {
      counts['\\limits 用法错误'] += badLimits.length
      hits['\\limits 用法错误'].push(`${rel} × ${badLimits.length}`)
    }

    // 本地图片是否存在
    for (const m of text.matchAll(/!\[[^\]]*\]\(([^)]+)\)/g)) {
      const target = m[1].split(/\s+/)[0].replace(/^<|>$/g, '')
      if (/^https?:/.test(target)) continue
      const abs = path.resolve(path.dirname(file), decodeURIComponent(target))
      try {
        await fs.access(abs)
      } catch {
        record('图片链接失效', rel, target)
      }
    }
  }

  const total = Object.values(counts).reduce((a, b) => a + b, 0)
  console.log('='.repeat(52))
  console.log(`扫描 ${files.length} 个 Markdown 文件`)
  console.log('='.repeat(52))
  for (const key of CHECKS) {
    console.log(`${counts[key] === 0 ? 'OK ' : '!! '}${key.padEnd(22)} ${counts[key]}`)
  }
  console.log('='.repeat(52))

  if (verbose) {
    for (const key of CHECKS) {
      if (!counts[key]) continue
      console.log(`\n--- ${key} ---`)
      for (const h of [...new Set(hits[key])].slice(0, 30)) console.log('   ' + h)
    }
  } else if (total > 0) {
    console.log('加 -v 查看详细清单')
  }

  if (total > 0) process.exitCode = 1
}

if (import.meta.url === pathToFileURL(process.argv[1]).href) {
  audit().catch((error) => {
    console.error(error)
    process.exit(1)
  })
}
