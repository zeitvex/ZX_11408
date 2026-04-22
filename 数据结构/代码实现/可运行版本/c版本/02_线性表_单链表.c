/*
 * ============================================================
 *   408 考研数据结构 —— 单链表 (可编译运行版)
 *   文件: 02_线性表_单链表.c
 *   编译: gcc 02_线性表_单链表.c -o linklist && linklist
 * ============================================================
 *
 *   本文件包含:
 *     1. 单链表的定义 (带头结点)
 *     2. 头插法 / 尾插法建表 (两种最基本的创建方式)
 *     3. 按序号 / 按值查找
 *     4. 插入 / 删除结点
 *     5. ★ 高频考点: 链表原地逆置 (头插法逆置)
 *     6. ★ 高频考点: 快慢指针找中间结点
 *     7. ★ 课后题: 删除链表中所有值为 x 的结点
 *     8. ★ 课后题: 链表排序 (直接插入排序思路)
 *
 *   教材惯例: 单链表带头结点, 头结点不存数据,
 *             L->next 指向第一个真实的数据结点。
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef int ElemType;

/* ---- 单链表结构体定义 ---- */
typedef struct LNode {
    ElemType data;
    struct LNode *next;
} LNode, *LinkList;
/* LinkList 等价于 LNode *, 用 LinkList 强调"这是整个链表"  */
/* 用 LNode * 强调"这是某个结点的指针"                     */


/* ========== 基础操作 ========== */

/* 初始化: 创建一个空的带头结点链表 */
LinkList InitList() {
    LinkList L = (LinkList)malloc(sizeof(LNode));
    L->next = NULL;
    return L;
}

/* 头插法建表: 每次把新结点插到头结点后面
 *   结果: 数据顺序与输入顺序相反
 *   (考点: 常用于 "链表逆置" 的核心技巧)
 */
LinkList CreateList_Head(int arr[], int n) {
    LinkList L = (LinkList)malloc(sizeof(LNode));
    L->next = NULL;
    int i;
    for (i = 0; i < n; i++) {
        LNode *s = (LNode *)malloc(sizeof(LNode));
        s->data = arr[i];
        s->next = L->next;  /* 新结点指向原来的第一个 */
        L->next = s;         /* 头结点指向新结点 */
    }
    return L;
}

/* 尾插法建表: 每次把新结点接到表尾
 *   结果: 数据顺序与输入顺序相同
 *   (需要一个尾指针 r 跟踪表尾)
 */
LinkList CreateList_Tail(int arr[], int n) {
    LinkList L = (LinkList)malloc(sizeof(LNode));
    L->next = NULL;
    LNode *r = L; /* r 始终指向当前的尾结点 */
    int i;
    for (i = 0; i < n; i++) {
        LNode *s = (LNode *)malloc(sizeof(LNode));
        s->data = arr[i];
        r->next = s;
        r = s;
    }
    r->next = NULL; /* 千万别忘: 最后尾结点的 next 置空! */
    return L;
}

/* 按序号查找: 返回第 i 个结点的指针 (1-based) */
LNode *GetElem(LinkList L, int i) {
    if (i < 0) return NULL;
    if (i == 0) return L;  /* 第 0 个是头结点 */

    LNode *p = L->next;
    int j = 1;
    while (p != NULL && j < i) {
        p = p->next;
        j++;
    }
    return p; /* 如果 i 超出长度, p 会是 NULL */
}

/* 按值查找: 返回第一个值为 e 的结点指针 */
LNode *LocateElem(LinkList L, ElemType e) {
    LNode *p = L->next;
    while (p != NULL && p->data != e)
        p = p->next;
    return p; /* 找不到返回 NULL */
}

/* 在第 i 个位置插入值为 e 的新结点 */
int ListInsert(LinkList L, int i, ElemType e) {
    LNode *p = GetElem(L, i - 1); /* 找前驱 */
    if (p == NULL) return 0;

    LNode *s = (LNode *)malloc(sizeof(LNode));
    s->data = e;
    s->next = p->next; /* 新结点指向原第 i 个 */
    p->next = s;        /* 前驱指向新结点 */
    return 1;
}

/* 删除第 i 个结点, 被删除的值通过 e 带回 */
int ListDelete(LinkList L, int i, ElemType *e) {
    LNode *p = GetElem(L, i - 1); /* 找前驱 */
    if (p == NULL || p->next == NULL) return 0;

    LNode *q = p->next;   /* q 指向要删除的结点 */
    *e = q->data;
    p->next = q->next;    /* 前驱跳过被删结点 */
    free(q);               /* 释放空间 */
    return 1;
}

/* 求表长 */
int ListLength(LinkList L) {
    int len = 0;
    LNode *p = L->next;
    while (p != NULL) {
        len++;
        p = p->next;
    }
    return len;
}

/* 打印链表 */
void PrintList(LinkList L) {
    LNode *p = L->next;
    printf("Head -> ");
    while (p != NULL) {
        printf("%d -> ", p->data);
        p = p->next;
    }
    printf("NULL\n");
}


/* ========== 高频考点算法 ========== */

/*
 * ★ 链表原地逆置 (不申请新结点)
 *   方法: 将原链表的结点逐一摘下, 用头插法重新挂到头结点后面
 *   时间 O(n), 空间 O(1)
 */
void ReverseList(LinkList L) {
    LNode *p = L->next;  /* p: 工作指针, 指向待处理结点 */
    LNode *r;              /* r: 保存 p 的后继, 防止断链 */
    L->next = NULL;        /* 先把头结点摘出来, 变成空链表 */

    while (p != NULL) {
        r = p->next;       /* 1. 保存后继 */
        p->next = L->next; /* 2. 头插: p 指向当前的第一个 */
        L->next = p;       /* 3. 头结点指向 p */
        p = r;             /* 4. p 前进到下一个待处理结点 */
    }
}

/*
 * ★ 快慢指针找中间结点
 *   快指针每次走 2 步, 慢指针每次走 1 步
 *   当快指针走到底时, 慢指针恰好在中间
 */
LNode *FindMiddle(LinkList L) {
    LNode *slow = L->next;
    LNode *fast = L->next;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

/*
 * ★ 删除链表中所有值为 x 的结点
 *   技巧: 用前驱指针判断后继的值
 */
void DeleteAllX(LinkList L, ElemType x) {
    LNode *p = L; /* p 是当前结点的前驱 */
    LNode *q;
    while (p->next != NULL) {
        if (p->next->data == x) {
            q = p->next;
            p->next = q->next;
            free(q);
            /* 注意: 这里不移动 p, 因为删除后 p->next 变了 */
        } else {
            p = p->next;
        }
    }
}

/*
 * ★ 链表排序 (直接插入排序)
 *   思路: 摘下原链头, 逐个插入到一个新的有序链表中
 */
void SortList(LinkList L) {
    if (L->next == NULL || L->next->next == NULL) return;

    LNode *p = L->next->next; /* 从第二个结点开始 */
    L->next->next = NULL;      /* 有序链表初始只有第一个结点 */

    while (p != NULL) {
        LNode *r = p->next;   /* 保存后继 */

        /* 在有序部分中找插入位置: 找到第一个 > p->data 的前驱 */
        LNode *q = L;
        while (q->next != NULL && q->next->data < p->data)
            q = q->next;

        /* 插入 */
        p->next = q->next;
        q->next = p;

        p = r; /* 处理下一个 */
    }
}


/* ========== main: 全部演示 ========== */
int main() {
    printf("===== 408 单链表 可运行演示 =====\n\n");

    /* 尾插法建表 */
    int data[] = {3, 1, 4, 1, 5, 9, 2, 6};
    LinkList L = CreateList_Tail(data, 8);
    printf("尾插法建表: ");
    PrintList(L);

    /* 头插法建表 (验证顺序反转) */
    int data2[] = {1, 2, 3};
    LinkList L2 = CreateList_Head(data2, 3);
    printf("头插法建表 {1,2,3}: ");
    PrintList(L2); /* 预期: 3 -> 2 -> 1 */

    /* 插入与删除 */
    ListInsert(L, 1, 99);
    printf("\n在第1位插入99: ");
    PrintList(L);

    ElemType del;
    ListDelete(L, 1, &del);
    printf("删除第1位(%d): ", del);
    PrintList(L);

    /* 链表逆置 */
    printf("\n--- 链表逆置 ---\n");
    printf("逆置前: ");
    PrintList(L);
    ReverseList(L);
    printf("逆置后: ");
    PrintList(L);

    /* 快慢指针找中间 */
    printf("\n--- 快慢指针找中间 ---\n");
    LNode *mid = FindMiddle(L);
    if (mid) printf("中间结点值 = %d\n", mid->data);

    /* 删除所有值为 1 的结点 */
    printf("\n--- 删除所有值为 1 的结点 ---\n");
    printf("删除前: ");
    PrintList(L);
    DeleteAllX(L, 1);
    printf("删除后: ");
    PrintList(L);

    /* 链表排序 */
    printf("\n--- 链表排序 ---\n");
    int data3[] = {5, 3, 8, 1, 2, 7};
    LinkList L3 = CreateList_Tail(data3, 6);
    printf("排序前: ");
    PrintList(L3);
    SortList(L3);
    printf("排序后: ");
    PrintList(L3);

    return 0;
}
