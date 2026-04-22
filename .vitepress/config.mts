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
      { text: '计算机组成原理', link: '/计算机组成原理/01_计算机系统概述' },
      { text: '高数', link: '/高数/00_考研高数一科目大纲与总览' }
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
      '/计算机组成原理/': [
        {
          text: '计算机组成原理',
          items: [
            { text: '计算机系统概述', link: '/计算机组成原理/01_计算机系统概述' },
            { text: '数据的表示和运算', link: '/计算机组成原理/02_数据的表示和运算' },
            { text: '存储系统', link: '/计算机组成原理/03_存储系统' },
            { text: '指令系统', link: '/计算机组成原理/04_指令系统' },
            { text: '中央处理器', link: '/计算机组成原理/05_中央处理器' },
            { text: '总线', link: '/计算机组成原理/06_总线' },
            { text: '输入输出系统', link: '/计算机组成原理/07_输入输出系统' },
            { text: '考频与重难点总结', link: '/计算机组成原理/08_考频与重难点总结' },
            { text: '大纲与思维导图', link: '/计算机组成原理/09_计算机组成原理大纲_思维导图' }
          ]
        }
      ],
      '/高数/': [
        {
          text: '高等数学',
          items: [
            { text: '大纲与总览', link: '/高数/00_考研高数一科目大纲与总览' },
            { text: '预备知识与基础公式', link: '/高数/01_预备知识与基础公式' },
            { text: '函数与极限', link: '/高数/02_函数_极限_连续' },
            { text: '一元函数微分学', link: '/高数/03_一元函数微分学' },
            { text: '一元函数积分学', link: '/高数/04_一元函数积分学' },
            { text: '向量代数与空间解析几何', link: '/高数/05_向量代数与空间解析几何' },
            { text: '多元函数微分学', link: '/高数/06_多元函数微分学' },
            { text: '多元函数积分学', link: '/高数/07_多元函数积分学_重积分与曲线曲面积分' },
            { text: '无穷级数', link: '/高数/08_无穷级数' },
            { text: '常微分方程', link: '/高数/09_常微分方程' },
            { text: '考频与重难点总结', link: '/高数/10_考频与重难点总结' }
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
