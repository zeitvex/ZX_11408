import { defineConfig } from 'vitepress'

export default defineConfig({
  title: "ZX_11408 知识库",
  description: "2027 计算机考研 11408 全科复习笔记",
  lang: 'zh-CN',
  lastUpdated: true,
  
  themeConfig: {
    logo: '/logo.png',
    nav: [
      { text: '首页', link: '/' },
      { text: '数据结构', link: '/数据结构/01_绪论' },
      { text: '高数', link: '/高数/01_预备知识与基础公式' }
    ],
    sidebar: {
      '/数据结构/': [
        {
          text: '数据结构',
          items: [
            { text: '绪论', link: '/数据结构/01_绪论' },
            { text: '线性表', link: '/数据结构/02_线性表' },
            { text: '栈和队列', link: '/数据结构/03_栈_队列_数组' },
            { text: '串', link: '/数据结构/04_串' },
            { text: '树与二叉树', link: '/数据结构/05_树与二叉树' },
            { text: '图', link: '/数据结构/06_图' },
            { text: '查找', link: '/数据结构/07_查找' },
            { text: '排序', link: '/数据结构/08_排序' },
            { text: '考频总结', link: '/数据结构/09_考频与重难点总结' }
          ]
        }
      ],
      '/高数/': [
        {
          text: '高等数学',
          items: [
            { text: '预备知识', link: '/高数/01_预备知识与基础公式' },
            { text: '函数与极限', link: '/高数/02_函数_极限_连续' }
          ]
        }
      ]
    },
    socialLinks: [
      { icon: 'github', link: 'https://github.com/zeitvex/ZX_11408' }
    ],
    footer: {
      message: 'Released under the MIT License.',
      copyright: 'Copyright © 2026-present zeitvex'
    },
    outline: {
      label: '页面导航',
      level: [2, 3]
    },
    docFooter: {
      prev: '上一页',
      next: '下一页'
    }
  },
  
  markdown: {
    math: true, // 原生支持数学公式 (使用 MathJax 3)
    lineNumbers: true,
    languages: ['asm']
  }
})
