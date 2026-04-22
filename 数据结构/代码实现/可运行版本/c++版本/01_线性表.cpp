/*
 * ============================================================
 *   408 考研数据结构 —— 线性表 (可运行 C++ 版)
 *   文件: 01_线性表.cpp
 *   编译: g++ 01_线性表.cpp -o test && ./test
 * ============================================================
 *
 *   覆盖内容:
 *     1. 顺序表: 初始化、插入、删除
 *     2. 单链表: 尾插法建表、链表逆置
 *     3. ★ 2010 真题: 循环左移 (三次逆置法)
 *     4. ★ 2011 真题: 两有序数组求中位数
 *     5. ★ 2013 真题: 摩尔投票法求主元素
 *
 *   C++ 特性: 使用引用 & 传参，简化指针操作
 * ============================================================
 */

#include <cstdio>
#include <cstdlib>

#define MaxSize 50
typedef int ElemType;


/* ========== 一、顺序表 ========== */

typedef struct {
    ElemType data[MaxSize];
    int length;
} SqList;

/* 初始化 */
void InitList(SqList &L) {
    L.length = 0;
}

/* 插入: 在第 i 个位置(1-based)插入元素 e */
bool ListInsert(SqList &L, int i, ElemType e) {
    if (i < 1 || i > L.length + 1 || L.length >= MaxSize)
        return false;

    for (int j = L.length; j >= i; j--)   // 后移腾位
        L.data[j] = L.data[j - 1];

    L.data[i - 1] = e;   // 位序 i 对应下标 i-1
    L.length++;
    return true;
}

/* 删除: 删除第 i 个位置的元素, 用 e 带回 */
bool ListDelete(SqList &L, int i, ElemType &e) {
    if (i < 1 || i > L.length)
        return false;

    e = L.data[i - 1];
    for (int j = i; j < L.length; j++)    // 前移填充
        L.data[j - 1] = L.data[j];

    L.length--;
    return true;
}

/* 打印顺序表 */
void PrintSqList(SqList L) {
    printf("[ ");
    for (int i = 0; i < L.length; i++)
        printf("%d ", L.data[i]);
    printf("]\n");
}


/* ========== 二、单链表 ========== */

typedef struct LNode {
    ElemType data;
    struct LNode *next;
} LNode, *LinkList;

/* 尾插法建表: 数据顺序与输入一致 */
LinkList CreateList_Tail(int arr[], int n) {
    LinkList L = (LinkList)malloc(sizeof(LNode));  // 创建头结点
    L->next = NULL;
    LNode *r = L;  // r 始终指向表尾

    for (int i = 0; i < n; i++) {
        LNode *s = (LNode *)malloc(sizeof(LNode));
        s->data = arr[i];
        r->next = s;   // 新结点接到表尾
        r = s;          // r 后移
    }
    r->next = NULL;     // 尾结点 next 置空
    return L;
}

/* 链表原地逆置: 头插法逆置, O(n)/O(1) */
void ReverseList(LinkList &L) {
    LNode *p = L->next;   // p: 工作指针
    LNode *r;              // r: 保存 p 的后继
    L->next = NULL;        // 头结点断开，变空表

    while (p != NULL) {
        r = p->next;       // 1. 保存后继
        p->next = L->next; // 2. 头插: p 指向当前第一个
        L->next = p;       // 3. 头结点指向 p
        p = r;             // 4. 前进到下一个
    }
}

/* 打印链表 */
void PrintList(LinkList L) {
    LNode *p = L->next;
    printf("Head->");
    while (p) {
        printf("%d->", p->data);
        p = p->next;
    }
    printf("NULL\n");
}


/* ========== 三、真题算法 ========== */

/*
 * ★ 2010 真题: 数组循环左移 p 位
 *   三次逆置法, 时间 O(n), 空间 O(1)
 */
void Reverse(int a[], int left, int right) {
    while (left < right) {
        int temp = a[left];
        a[left] = a[right];
        a[right] = temp;
        left++;
        right--;
    }
}

void LeftShift(int a[], int n, int p) {
    p %= n;                    // 防止 p >= n
    Reverse(a, 0, p - 1);     // 步骤1: 逆置前 p 个
    Reverse(a, p, n - 1);     // 步骤2: 逆置后 n-p 个
    Reverse(a, 0, n - 1);     // 步骤3: 整体逆置
}

/*
 * ★ 2011 真题: 两等长有序数组求中位数
 *   双指针归并计数, O(n)
 */
int FindMedian(int A[], int B[], int n) {
    int ia = 0, ib = 0;    // 两个数组的指针
    int last = 0;           // 记录每次选中的较小值

    for (int count = 0; count < n; count++) {
        if (ia < n && (ib >= n || A[ia] <= B[ib]))
            last = A[ia++];   // A 的当前元素较小(或 B 已用完)
        else
            last = B[ib++];   // B 的当前元素较小(或 A 已用完)
    }
    return last;  // 第 n 个就是中位数
}

/*
 * ★ 2013 真题: 摩尔投票法找主元素
 *   第一遍: 候选者计数, 归零换人
 *   第二遍: 验证候选者出现次数是否 > n/2
 */
int FindMainElement(int A[], int n) {
    int candidate = A[0];
    int count = 1;

    // 第一遍: 寻找候选者
    for (int i = 1; i < n; i++) {
        if (A[i] == candidate) {
            count++;
        } else {
            count--;
            if (count == 0) {
                candidate = A[i];
                count = 1;
            }
        }
    }

    // 第二遍: 验证
    count = 0;
    for (int i = 0; i < n; i++)
        if (A[i] == candidate) count++;

    return (count > n / 2) ? candidate : -1;
}


/* ========== main 演示 ========== */
int main() {
    printf("===== 线性表 C++ 可运行版 =====\n\n");

    /* --- 顺序表 --- */
    SqList L;
    InitList(L);
    ListInsert(L, 1, 10);
    ListInsert(L, 2, 20);
    ListInsert(L, 3, 30);
    printf("顺序表: ");
    PrintSqList(L);

    /* --- 单链表 --- */
    int d[] = {3, 1, 4, 1, 5};
    LinkList LL = CreateList_Tail(d, 5);
    printf("链表:   ");
    PrintList(LL);
    ReverseList(LL);
    printf("逆置:   ");
    PrintList(LL);

    /* --- 2010: 循环左移 --- */
    printf("\n--- 2010 真题 ---\n");
    int arr[] = {0, 1, 2, 3, 4, 5, 6, 7};
    LeftShift(arr, 8, 3);
    printf("循环左移3: ");
    for (int i = 0; i < 8; i++) printf("%d ", arr[i]);
    printf("\n");

    /* --- 2011: 中位数 --- */
    printf("\n--- 2011 真题 ---\n");
    int A[] = {1, 3, 5, 7, 9};
    int B[] = {2, 4, 6, 8, 10};
    printf("中位数: %d\n", FindMedian(A, B, 5));

    /* --- 2013: 主元素 --- */
    printf("\n--- 2013 真题 ---\n");
    int C[] = {0, 5, 5, 3, 5, 1, 5, 5};
    printf("主元素: %d\n", FindMainElement(C, 8));

    return 0;
}
