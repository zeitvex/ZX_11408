import{_ as s,o as a,c as p,ae as e}from"./chunks/framework.RRduUuAx.js";const m=JSON.parse('{"title":"408 数据结构 —— 代码实现总览","description":"","frontmatter":{},"headers":[],"relativePath":"数据结构/代码实现/index.md","filePath":"数据结构/代码实现/index.md","lastUpdated":1776859928000}'),l={name:"数据结构/代码实现/index.md"};function r(i,n,c,t,o,b){return a(),p("div",null,[...n[0]||(n[0]=[e(`<h1 id="_408-数据结构-——-代码实现总览" tabindex="-1">408 数据结构 —— 代码实现总览 <a class="header-anchor" href="#_408-数据结构-——-代码实现总览" aria-label="Permalink to &quot;408 数据结构 —— 代码实现总览&quot;">​</a></h1><p>本目录是 408 考研《数据结构》全部代码题覆盖的弹药库，按照<strong>用途</strong>与<strong>语言</strong>划分为 4 个子目录。</p><hr><h2 id="目录结构" tabindex="-1">目录结构 <a class="header-anchor" href="#目录结构" aria-label="Permalink to &quot;目录结构&quot;">​</a></h2><div class="language- vp-adaptive-theme line-numbers-mode"><button title="Copy Code" class="copy"></button><span class="lang"></span><pre class="shiki shiki-themes github-light github-dark vp-code" tabindex="0"><code><span class="line"><span></span></span>
<span class="line"><span>代码实现/</span></span>
<span class="line"><span></span></span>
<span class="line"><span>├── 伪代码版本/</span></span>
<span class="line"><span></span></span>
<span class="line"><span>│   ├── c版本/        ← 纯 C 伪代码，面向考卷手写，不含 main，不可单独编译</span></span>
<span class="line"><span></span></span>
<span class="line"><span>│   │   └── README.md</span></span>
<span class="line"><span></span></span>
<span class="line"><span>│   └── c++版本/      ← C++ 引用风格(&amp;)伪代码，面向考卷手写</span></span>
<span class="line"><span></span></span>
<span class="line"><span>│       └── README.md</span></span>
<span class="line"><span></span></span>
<span class="line"><span>├── 可运行版本/</span></span>
<span class="line"><span></span></span>
<span class="line"><span>│   ├── c版本/        ← 完整可编译 C 程序，含 main 与测试数据</span></span>
<span class="line"><span></span></span>
<span class="line"><span>│   │   └── README.md</span></span>
<span class="line"><span></span></span>
<span class="line"><span>│   └── c++版本/      ← 完整可编译 C++ 程序，含 main 与测试数据</span></span>
<span class="line"><span></span></span>
<span class="line"><span>│       └── README.md</span></span>
<span class="line"><span></span></span>
<span class="line"><span>└── README.md         ← 本文件</span></span></code></pre><div class="line-numbers-wrapper" aria-hidden="true"><span class="line-number">1</span><br><span class="line-number">2</span><br><span class="line-number">3</span><br><span class="line-number">4</span><br><span class="line-number">5</span><br><span class="line-number">6</span><br><span class="line-number">7</span><br><span class="line-number">8</span><br><span class="line-number">9</span><br><span class="line-number">10</span><br><span class="line-number">11</span><br><span class="line-number">12</span><br><span class="line-number">13</span><br><span class="line-number">14</span><br><span class="line-number">15</span><br><span class="line-number">16</span><br><span class="line-number">17</span><br><span class="line-number">18</span><br><span class="line-number">19</span><br><span class="line-number">20</span><br><span class="line-number">21</span><br><span class="line-number">22</span><br><span class="line-number">23</span><br><span class="line-number">24</span><br></div></div><h2 id="四个版本的区别与使用场景" tabindex="-1">四个版本的区别与使用场景 <a class="header-anchor" href="#四个版本的区别与使用场景" aria-label="Permalink to &quot;四个版本的区别与使用场景&quot;">​</a></h2><p>| 版本 | 用途 | 是否可编译 | 语法风格 |</p><p>|:---|:---|:---😐:---|</p><p>| <strong>伪代码/c版本</strong> | 考卷手写、快速默背 | ✗ | 纯 C 指针风格 <code>-&gt;</code> |</p><p>| <strong>伪代码/c++版本</strong> | 考卷手写（推荐） | ✗ | C++ 引用 <code>&amp;</code>，免去二级指针 |</p><p>| <strong>可运行/c版本</strong> | 理解算法、调试验证 | ✓ gcc | 纯 C，含 main + 测试数据 |</p><p>| <strong>可运行/c++版本</strong> | 理解算法、调试验证 | ✓ g++ | C++，含 main + 测试数据 |</p><h2 id="推荐学习路径" tabindex="-1">推荐学习路径 <a class="header-anchor" href="#推荐学习路径" aria-label="Permalink to &quot;推荐学习路径&quot;">​</a></h2><ol><li><p><strong>先跑可运行版本</strong> → 看到输出结果，建立直觉</p></li><li><p><strong>对照理论笔记</strong> → <code>数据结构/01~08_xxx.md</code></p></li><li><p><strong>背诵伪代码版本</strong> → 考前默写训练</p></li><li><p><strong>参考代码题.md</strong> → 了解历年真题出题套路</p></li></ol>`,14)])])}const u=s(l,[["render",r]]);export{m as __pageData,u as default};
