#include <stdio.h>
#include <stdlib.h>

/* =========================================
 * 408 统考数据结构核心代码 - 线性表 (纯 C 语言风格)
 * 考试默写要求：极高。必须随手能写出链表的防断链操作。
 * ========================================= */

#define MaxSize 50
typedef int ElemType;

// 1. 顺序表结构体定义
typedef struct {
    ElemType data[MaxSize];
    int length;
} SqList;

// 2. 单链表结构体定义
typedef struct LNode {
    ElemType data;
    struct LNode *next;
} LNode, *LinkList;

/* -----------------------------------------
 * 【1】顺序表核心操作
 * ----------------------------------------- */

// 初始化顺序表
void InitList(SqList *L) {
    L->length = 0;
}

// 顺序表插入
// 注意：数组下标从 0 开始，第 i 个位置的下标为 i-1
int ListInsert(SqList *L, int i, ElemType e) {
    if (i < 1 || i > L->length + 1) return 0; // 判断 i 的范围是否有效
    if (L->length >= MaxSize) return 0;       // 存满了不能插
    
    for (int j = L->length; j >= i; j--) {
        L->data[j] = L->data[j - 1];          // 将第 i 个元素及之后的元素后移
    }
    L->data[i - 1] = e;                       // 在位置 i 放上 e
    L->length++;
    return 1;
}

// 顺序表删除
int ListDelete(SqList *L, int i, ElemType *e) {
    if (i < 1 || i > L->length) return 0;     // 判断 i 的范围是否有效
    *e = L->data[i - 1];                      // 【考点】取出被删除的元素
    for (int j = i; j < L->length; j++) {
        L->data[j - 1] = L->data[j];          // 将第 i 个位置之后的元素前移
    }
    L->length--;
    return 1;
}

/* -----------------------------------------
 * 【2】单链表核心操作
 * ----------------------------------------- */

// 头插法建立单链表（常用于链表的【原地逆置】）
LinkList List_HeadInsert(LinkList L) {
    LNode *s;
    int x;
    L = (LinkList)malloc(sizeof(LNode)); // 创建头结点
    L->next = NULL;
    
    scanf("%d", &x);
    while (x != 9999) { // 9999 为结束标志
        s = (LNode *)malloc(sizeof(LNode));
        s->data = x;
        
        // 核心两步，千万不能反！
        s->next = L->next; 
        L->next = s;
        
        scanf("%d", &x);
    }
    return L;
}

// 尾插法建立单链表（注意：必须增加一个尾指针 r）
LinkList List_TailInsert(LinkList L) {
    int x;
    L = (LinkList)malloc(sizeof(LNode));
    LNode *s, *r = L; // r 为表尾指针
    
    scanf("%d", &x);
    while (x != 9999) {
        s = (LNode *)malloc(sizeof(LNode));
        s->data = x;
        r->next = s;  // r 连上新结点
        r = s;        // r 指向新的表尾
        scanf("%d", &x);
    }
    r->next = NULL; // 【易错点】尾结点指针置空
    return L;
}

// 删除指定结点 p 的后继结点
// 考研大题常见子操作，注意判空保护
int DeleteNextNode(LNode *p) {
    if (p == NULL || p->next == NULL) return 0;
    
    LNode *q = p->next;  // 记录要删除的结点
    p->next = q->next;   // 断开联系
    free(q);             // 释放空间
    return 1;
}

// 找到链表的中间结点（经常利用双指针/快慢指针法）
LNode* FindMidNode(LinkList L) {
    LNode *p = L->next, *q = L->next;
    while (q != NULL && q->next != NULL) {
        p = p->next;       // 慢指针走一步
        q = q->next->next; // 快指针走两步
    }
    return p; // 当快指针到底时，慢指针正好在中间
}
