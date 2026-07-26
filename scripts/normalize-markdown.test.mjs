import test from 'node:test'
import assert from 'node:assert/strict'

import { normalizeMarkdown } from './normalize-markdown.mjs'

const run = (input) => normalizeMarkdown(input).trimEnd()

test('相邻行内公式之间的正文不会被误判为公式', () => {
  assert.equal(run('设 $A$ 与 $B$ 互斥。'), '设 $A$ 与 $B$ 互斥。')
  assert.equal(run('已知 $E$，求 $F$。'), '已知 $E$，求 $F$。')
  assert.equal(
    run("若 $f(x)$ 为奇函数，则 $f''(0) = 0$（因 $f''$ 为奇函数）。"),
    "若 $f(x)$ 为奇函数，则 $f''(0) = 0$（因 $f''$ 为奇函数）。"
  )
})

test('修复定界符内侧的空格（会导致 MathJax 不渲染）', () => {
  assert.equal(run('区间 $I $ 上，$ x_0 \\in I$，若极限'), '区间 $I$ 上，$x_0 \\in I$，若极限')
  assert.equal(run('事件 $E $ 与 $ F$'), '事件 $E$ 与 $F$')
})

test('中文与公式之间补空格，标点旁不补', () => {
  assert.equal(run('设$x$为实数'), '设 $x$ 为实数')
  assert.equal(run('取 $x$，则'), '取 $x$，则')
})

test('行内代码与公式内容不受标点规范化影响', () => {
  assert.equal(run('使用 `a.b` 调用'), '使用 `a.b` 调用')
  assert.equal(run('结果为 $P(A).$ 的形式'), '结果为 $P(A).$ 的形式')
})

test('正文中的数字不会被占位符误替换', () => {
  assert.equal(run('共 3 个 $n$ 阶子式'), '共 3 个 $n$ 阶子式')
})

test('块级公式保持独立成行', () => {
  assert.equal(run('$$a+b$$'), '$$\na+b\n$$')
})

test('被列表项包住的块公式不会让状态错位', () => {
  // 历史 bug：`- $$` / `5. $$` 不等于 `$$`，inMath 未开启却被闭合行开启，
  // 导致其后整个文件被当成公式内容——正文被吞进公式块，规范化也静默失效。
  for (const marker of ['-', '5.', '1)']) {
    const source = ['前一项', '', `${marker} $$`, 'x=1', '$$', '', '## 后面的标题', '正文'].join('\n')
    const result = run(source)
    assert.ok(result.includes('## 后面的标题'), `${marker}: 标题应当仍被正常处理`)
    assert.ok(!new RegExp(`^\\s*\\${marker[0]}.*\\$\\$`, 'm').test(result), `${marker}: $$ 应被还原为独立块`)
    assert.equal(normalizeMarkdown(result), normalizeMarkdown(normalizeMarkdown(result)))
  }
})

test('标题上下自动补空行', () => {
  assert.equal(run('正文\n## 标题\n下一段'), '正文\n\n## 标题\n\n下一段')
  // 已经有空行时不重复添加
  assert.equal(run('正文\n\n## 标题\n\n下一段'), '正文\n\n## 标题\n\n下一段')
  // 代码块内的 # 不是标题
  assert.equal(run('```python\n# 注释\nx = 1\n```'), '```python\n# 注释\nx = 1\n```')
})

test('幂等：再次规范化不产生新变化', () => {
  const source = [
    '设 $A$ 与 $B$ 互斥，则 $P(A\\cup B)=P(A)+P(B)$。',
    '',
    '取 $x_0$，共 3 个 $n$ 阶子式。'
  ].join('\n')

  const once = normalizeMarkdown(source)
  assert.equal(normalizeMarkdown(once), once)
})
