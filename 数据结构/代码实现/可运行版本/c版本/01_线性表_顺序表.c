/*
 * ============================================================
 *   408 考研数据结构 —— 顺序表 (可编译运行版)
 *   文件: 01_线性表_顺序表.c
 *   编译: gcc 01_线性表_顺序表.c -o sqlist && sqlist
 * ============================================================
 *
 *   本文件包含:
 *     1. 顺序表的定义与初始化
 *     2. 插入、删除操作 (考研必背)
 *     3. 按值/按序号查找
 *     4. ★ 真题经典: 数组循环左移 p 位 (2010 年真题)
 *     5. ★ 真题经典: 两个有序数组求中位数 (2011 年真题)
 *     6. ★ 真题经典: 寻找主元素 (2013 年真题)
 *
 *   注意: 教材中数组下标一般从 0 开始, 逻辑位序从 1 开始。
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MaxSize 50
typedef int ElemType;

/* ---- 顺序表结构体定义 ---- */
typedef struct {
    ElemType data[MaxSize]; /* 用静态数组存放数据元素 */
    int length;             /* 当前长度 */
} SqList;


/* ========== 基础操作 ========== */

/* 初始化: 将长度置零即可 */
void InitList(SqList *L) {
    L->length = 0;
}

/* 插入: 在第 i 个位置(1-based)插入元素 e
 *   - 要把第 i 个及之后的元素全部后移一位
 *   - 时间复杂度: 最好 O(1), 最坏 O(n), 平均 O(n)
 */
int ListInsert(SqList *L, int i, ElemType e) {
    if (i < 1 || i > L->length + 1) return 0;
    if (L->length >= MaxSize) return 0;

    int j;
    for (j = L->length; j >= i; j--)  /* 从最后一个元素开始后移 */
        L->data[j] = L->data[j - 1];

    L->data[i - 1] = e;  /* 位序 i 对应下标 i-1 */
    L->length++;
    return 1;
}

/* 删除: 删除第 i 个位置(1-based)的元素, 用 e 带回
 *   - 要把第 i+1 个及之后的元素全部前移一位
 */
int ListDelete(SqList *L, int i, ElemType *e) {
    if (i < 1 || i > L->length) return 0;

    *e = L->data[i - 1];
    int j;
    for (j = i; j < L->length; j++)
        L->data[j - 1] = L->data[j];

    L->length--;
    return 1;
}

/* 按值查找: 返回第一个等于 e 的元素的位序(1-based), 找不到返回 0 */
int LocateElem(SqList L, ElemType e) {
    int i;
    for (i = 0; i < L.length; i++)
        if (L.data[i] == e) return i + 1;
    return 0;
}

/* 打印顺序表 */
void PrintList(SqList L) {
    int i;
    printf("[ ");
    for (i = 0; i < L.length; i++)
        printf("%d ", L.data[i]);
    printf("] (length=%d)\n", L.length);
}


/* ========== 408 真题经典算法 ========== */

/*
 * ★ 2010 年真题: 将数组 a[0..n-1] 循环左移 p 位
 *   思路: 三次逆置
 *     1. 逆置 a[0..p-1]
 *     2. 逆置 a[p..n-1]
 *     3. 逆置 a[0..n-1]
 *   时间 O(n), 空间 O(1)
 */
void Reverse(int a[], int left, int right) {
    int temp;
    while (left < right) {
        temp = a[left];
        a[left] = a[right];
        a[right] = temp;
        left++;
        right--;
    }
}

void LeftShift(int a[], int n, int p) {
    p = p % n; /* 防止 p >= n 的情况 */
    if (p == 0) return;
    Reverse(a, 0, p - 1);     /* 步骤1: 逆置前 p 个 */
    Reverse(a, p, n - 1);     /* 步骤2: 逆置后 n-p 个 */
    Reverse(a, 0, n - 1);     /* 步骤3: 整体逆置 */
}

/*
 * ★ 2011 年真题: 两个等长有序序列 A,B 各含 n 个元素, 求合并后的中位数
 *   O(n) 解法: 双指针归并计数
 *     两个指针分别指向 A 和 B 的起始位置,
 *     每次移动较小者的指针, 当总共移动了 n-1 次时,
 *     下一个较小者就是中位数。
 */
int FindMedian(int A[], int B[], int n) {
    int ia = 0, ib = 0;
    int count;
    int last = 0; /* 记录上一个被选中的值 */

    for (count = 0; count < n; count++) {
        if (ia < n && (ib >= n || A[ia] <= B[ib])) {
            last = A[ia];
            ia++;
        } else {
            last = B[ib];
            ib++;
        }
    }
    return last;  /* 第 n 个就是中位数 */
}

/*
 * ★ 2013 年真题: 找主元素
 *   若数组 A[0..n-1] 中有某个元素出现次数 > n/2, 则称其为主元素
 *   返回主元素值, 若不存在返回 -1
 *
 *   经典 O(n) 做法: 摩尔投票法 (Boyer-Moore Voting)
 *     - 第一遍扫描: 用一个候选者 candidate 和计数器 count
 *       遇到相同的 count++, 不同的 count--
 *       count 归零时换人。最后剩下的就是候选者。
 *     - 第二遍扫描: 验证候选者出现次数是否真的 > n/2
 */
int FindMainElement(int A[], int n) {
    int candidate = A[0];
    int count = 1;
    int i;

    /* 第一遍: 找候选者 */
    for (i = 1; i < n; i++) {
        if (A[i] == candidate)
            count++;
        else {
            count--;
            if (count == 0) {
                candidate = A[i];
                count = 1;
            }
        }
    }

    /* 第二遍: 验证 */
    count = 0;
    for (i = 0; i < n; i++)
        if (A[i] == candidate) count++;

    if (count > n / 2)
        return candidate;
    else
        return -1;
}


/* ========== main: 全部演示 ========== */
int main() {
    printf("===== 408 顺序表 可运行演示 =====\n\n");

    /* --- 基础操作演示 --- */
    SqList L;
    InitList(&L);
    ListInsert(&L, 1, 10);
    ListInsert(&L, 2, 20);
    ListInsert(&L, 3, 30);
    ListInsert(&L, 2, 15); /* 在第2位插入15 */
    printf("插入后: ");
    PrintList(L); /* 预期: [10 15 20 30] */

    ElemType del;
    ListDelete(&L, 3, &del);
    printf("删除第3位(值=%d)后: ", del);
    PrintList(L); /* 预期: [10 15 30] */

    int pos = LocateElem(L, 15);
    printf("元素 15 在位序 %d\n\n", pos);

    /* --- 2010: 循环左移 --- */
    printf("--- 2010 真题: 循环左移 ---\n");
    int arr1[] = {0, 1, 2, 3, 4, 5, 6, 7};
    int n1 = 8, p = 3;
    printf("原数组: ");
    int i;
    for (i = 0; i < n1; i++) printf("%d ", arr1[i]);
    LeftShift(arr1, n1, p);
    printf("\n左移 %d 位: ", p);
    for (i = 0; i < n1; i++) printf("%d ", arr1[i]);
    printf("\n\n");

    /* --- 2011: 两有序数组求中位数 --- */
    printf("--- 2011 真题: 两有序数组求中位数 ---\n");
    int A[] = {1, 3, 5, 7, 9};
    int B[] = {2, 4, 6, 8, 10};
    int median = FindMedian(A, B, 5);
    printf("A = {1,3,5,7,9}, B = {2,4,6,8,10}\n");
    printf("中位数 = %d\n\n", median); /* 预期: 5 */

    /* --- 2013: 主元素 --- */
    printf("--- 2013 真题: 主元素 ---\n");
    int C[] = {0, 5, 5, 3, 5, 1, 5, 5};
    int mainElem = FindMainElement(C, 8);
    printf("数组 = {0,5,5,3,5,1,5,5}\n");
    printf("主元素 = %d\n\n", mainElem); /* 预期: 5 */

    int D[] = {0, 5, 5, 3, 5, 1, 2, 7};
    mainElem = FindMainElement(D, 8);
    printf("数组 = {0,5,5,3,5,1,2,7}\n");
    printf("主元素 = %d (不存在则 -1)\n", mainElem);

    return 0;
}
