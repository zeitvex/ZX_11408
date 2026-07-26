import re, pathlib, io, collections

root = pathlib.Path('.')
out = io.StringIO()
re_fence = re.compile(r'```.*?```', re.S)
re_block = re.compile(r'\$\$.*?\$\$', re.S)
re_icode = re.compile(r'`[^`\n]*`')
re_inline = re.compile(r'\$[^$\n]+\$')
def blank(m): return ' ' * len(m.group(0))

def math_only(t):
    """只取公式内容"""
    spans = []
    x = re_fence.sub(blank, t)
    for m in re_block.finditer(x):
        spans.append(m.group(0))
    y = re_block.sub(blank, x)
    y = re_icode.sub(blank, y)
    for m in re_inline.finditer(y):
        spans.append(m.group(0))
    return spans

def prose_only(t):
    x = re_fence.sub(blank, t)
    x = re_block.sub(blank, x)
    x = re_icode.sub(blank, x)
    x = re_inline.sub(blank, x)
    x = re.sub(r'!?\[[^\]]*\]\([^)]*\)', ' ', x)
    return x

# ---------- 1. 公式书写风格 ----------
frac = collections.Counter()
dcmd = collections.Counter()
for f in sorted(root.rglob('*.md')):
    if 'node_modules' in str(f): continue
    t = f.read_text(encoding='utf-8')
    m = '\n'.join(math_only(t))
    for k in ['\\cfrac', '\\dfrac', '\\frac']:
        frac[k] += len(re.findall(re.escape(k) + r'(?![a-zA-Z])', m))
    dcmd['\\mathrm d(无花括号)'] += len(re.findall(r'\\mathrm d(?![a-zA-Z{])', m))
    dcmd['\\mathrm{d}'] += len(re.findall(r'\\mathrm\{d\}', m))
    dcmd['\\mathrm{~}其它'] += len(re.findall(r'\\mathrm\{(?!d\})', m))
    dcmd['裸 dx（无 \\mathrm）'] += len(re.findall(r'(?<![\\a-zA-Z])d[xyzt](?![a-zA-Z])', m))

out.write('=== 1. 分数命令使用 ===\n')
for k, v in frac.most_common():
    out.write('  %-10s %5d\n' % (k, v))
out.write('\n=== 2. 微分记号 d 的写法 ===\n')
for k, v in dcmd.most_common():
    out.write('  %-22s %5d\n' % (k, v))

# ---------- 3. 术语一致性 ----------
PAIRS = [
    ('结点', '节点'), ('二叉排序树', '二叉搜索树'), ('时间复杂度', '时间复杂性'),
    ('内存', '主存'), ('分组', '数据报'), ('比特', '位'),
    ('极大线性无关组', '最大线性无关组'), ('特征向量', '本征向量'),
    ('可微分', '可微'), ('审敛法', '判别法'), ('敛散性', '收敛性'),
    ('充要条件', '充分必要条件'), ('无穷小', '无穷小量'),
]
out.write('\n=== 3. 术语混用 ===\n')
for a, b in PAIRS:
    ca = cb = 0
    fa, fb = set(), set()
    for f in sorted(root.rglob('*.md')):
        if 'node_modules' in str(f): continue
        p = prose_only(f.read_text(encoding='utf-8'))
        na, nb = p.count(a), p.count(b)
        ca += na; cb += nb
        if na: fa.add(f.name)
        if nb: fb.add(f.name)
    if ca and cb:
        out.write('  %-14s %4d  vs  %-14s %4d   （同现文件 %d）\n'
                  % (a, ca, b, cb, len(fa & fb)))

pathlib.Path('_a1.txt').write_text(out.getvalue(), encoding='utf-8')
print('ok')
