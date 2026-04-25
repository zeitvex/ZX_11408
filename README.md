# ZX_11408 知识库项目

> **面向 2027 计算机考研 (11408 + 数一) 的高密度、实战型知识库。**

本仓库旨在构建一个系统、专业且易于快速回忆的考研复习体系。基于 **VitePress** 驱动，支持 LaTeX 公式渲染、Gfm 标注语法，并针对移动端与 Web 端进行了深度优化。

---

## 📂 目录结构

```bash
ZX_11408/
├── .vitepress/          # VitePress 配置文件 (侧边栏、导航栏、插件)
├── 操作系统/            # OS 核心知识点与 PV 模板
├── 计算机网络/          # CN 协议栈与报文段陷阱
├── 数据结构/            # DS 算法与逻辑结构
├── 计算机组成原理/      # CO 硬件位数与指令集
├── 高数/                # 高等数学核心公式与计算外挂
├── 线性代数/            # 矩阵理论与判定金律
├── 概率论/              # 随机变量分布速查
├── index.md             # 站点首页
└── package.json         # 项目依赖与管理脚本
```

---

## 🛠️ 项目维护常用命令

| 命令 | 用途 | 备注 |
| :--- | :--- | :--- |
| `npm install` | **初始化环境** | 首次克隆项目或 package.json 更新后运行 |
| `npm run dev` | **本地实时预览** | 启动开发服务器，支持热更新 (`http://localhost:5173`) |
| `npm run build` | **静态构建** | 生成部署用的 HTML 文件 (存放在 `.vitepress/dist`) |
| `npm run preview` | **预览构建结果** | 在本地模拟生产环境运行构建后的站点 |

---

## 🚀 深度部署流程 (1Panel + GitHub Actions)

本项目的部署采用 **“源码托管-云端编译-静态分发”** 的模式，实现推送即上线的自动化流程。

### 1. 主机端 (Local Machine)
这是您的写作环境，日常操作如下：
- **修改内容**：在本地 IDE (如 VS Code) 中编辑 Markdown 文件。
- **本地验证**：运行 `npm run dev` 查看效果，运行 `npm run build` 确保无死链。
- **推送更新**：执行 Git 三板斧：
  ```bash
  git add .
  git commit -m "feat: 补充 xxx 章节知识点"
  git push origin main
  ```

### 2. GitHub 端 (GitHub Actions)
当 `main` 分支接收到推送时，会自动触发 `.github/workflows/deploy.yml` 工作流：
- **环境搭建**：初始化 Node.js 环境，安装依赖。
- **静态构建**：执行 `npm run build` 生成 `.vitepress/dist` 目录。
- **分支分发**：将编译后的静态网页强制推送到仓库的 `gh-pages` 分支。
- **指令下发**：工作流通过 SSH 连接至您的 1Panel 服务器。

### 3. 服务器端 (1Panel Server)
服务器接收到 GitHub 的 SSH 指令后，在静态网站目录下执行同步：
- **目录定位**：进入 1Panel 创建的静态网站目录 (如 `/opt/1panel/apps/.../yoursite.com/index`)。
- **增量拉取**：执行 `git fetch --all` 拉取远程最新的 `gh-pages` 分支。
- **强制覆盖**：执行 `git reset --hard origin/gh-pages`，将网站内容瞬间更新为最新编译结果。
- **即时生效**：由于是 OpenResty 托管的静态站点，文件替换后无需重启服务即可访问。

---

## ✍️ 编写规范 (Style Guide)

- **标题层级**：`#` (页面标题), `##` (二级章节), `###` (三级考点)。
- **标注容器**：`> [!IMPORTANT]` (必背结论), `> [!WARNING]` (易错陷阱), `> [!TIP]` (解题口诀)。
- **公式规范**：行内 `$formula$`, 独立 `$$formula$$`。支持 MathJax3 渲染。

---

## 💡 备考建议
- **考频星级**：每个科目总结页均有 `★` 标注，优先复习五星考点。
- **快速回忆**：建议利用碎片时间在移动端查阅，通过高亮标注快速扫描知识点。
