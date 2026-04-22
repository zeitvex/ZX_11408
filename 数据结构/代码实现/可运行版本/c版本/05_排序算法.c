/*
 * ============================================================
 *   408 考研数据结构 —— 排序算法全家桶 (可编译运行版)
 *   文件: 05_排序算法.c
 *   编译: gcc 05_排序算法.c -o sort && sort
 * ============================================================
 *
 *   本文件一次性将 408 考纲要求的全部排序算法集齐并可运行:
 *     1. 直接插入排序
 *     2. 折半插入排序
 *     3. 希尔排序
 *     4. 冒泡排序
 *     5. ★ 快速排序 (含 Partition 核心及第 k 小应用)
 *     6. 简单选择排序
 *     7. ★ 堆排序 (大顶堆建堆 + 排序, 下标从 1 开始)
 *     8. ★ 归并排序
 *
 *   每个算法都带完整的测试, 一运行就能看到排序前后的对比。
 *
 *   注意: 堆排序中, 教材惯例 A[0] 作为暂存单元不存数据,
 *         有效元素从 A[1] 到 A[n]。
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

/* 打印数组辅助函数 */
void PrintArray(int A[], int n) {
    int i;
    for (i = 0; i < n; i++) printf("%d ", A[i]);
    printf("\n");
}

/* 拷贝数组 (每次测试前恢复原始数据) */
void CopyArray(int src[], int dst[], int n) {
    int i;
    for (i = 0; i < n; i++) dst[i] = src[i];
}

/* swap */
void swap(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
}


/* ===========================================
 *  1. 直接插入排序
 *     将 A[i] 插入到前面已排好的 A[0..i-1] 中
 *     最好 O(n), 最坏 O(n^2), 稳定
 * =========================================== */
void InsertSort(int A[], int n) {
    int i, j, temp;
    for (i = 1; i < n; i++) {
        if (A[i] < A[i - 1]) {
            temp = A[i];
            for (j = i - 1; j >= 0 && temp < A[j]; j--)
                A[j + 1] = A[j]; /* 一边找一边后移 */
            A[j + 1] = temp;
        }
    }
}


/* ===========================================
 *  2. 折半插入排序
 *     查找阶段用二分, 但移动次数不变, 总体仍 O(n^2)
 * =========================================== */
void BinaryInsertSort(int A[], int n) {
    int i, j, low, high, mid, temp;
    for (i = 1; i < n; i++) {
        temp = A[i];
        low = 0; high = i - 1;
        while (low <= high) {
            mid = (low + high) / 2;
            if (A[mid] > temp) high = mid - 1;
            else low = mid + 1;
        }
        /* high+1 就是插入位置 */
        for (j = i - 1; j >= high + 1; j--)
            A[j + 1] = A[j];
        A[high + 1] = temp;
    }
}


/* ===========================================
 *  3. 希尔排序
 *     增量序列: dk = n/2, n/4, ..., 1
 *     不稳定, 约 O(n^1.3)
 * =========================================== */
void ShellSort(int A[], int n) {
    int dk, i, j, temp;
    for (dk = n / 2; dk >= 1; dk /= 2) {
        for (i = dk; i < n; i++) {
            if (A[i] < A[i - dk]) {
                temp = A[i];
                for (j = i - dk; j >= 0 && temp < A[j]; j -= dk)
                    A[j + dk] = A[j];
                A[j + dk] = temp;
            }
        }
    }
}


/* ===========================================
 *  4. 冒泡排序
 *     最好 O(n), 最坏 O(n^2), 稳定
 *     每趟把一个最小/最大的元素冒到最终位置
 * =========================================== */
void BubbleSort(int A[], int n) {
    int i, j, flag;
    for (i = 0; i < n - 1; i++) {
        flag = 0;
        for (j = n - 1; j > i; j--) {
            if (A[j - 1] > A[j]) {
                swap(&A[j - 1], &A[j]);
                flag = 1;
            }
        }
        if (flag == 0) return; /* 本趟无交换, 已经有序 */
    }
}


/* ===========================================
 *  5. 快速排序
 *     核心: Partition 划分函数
 *     平均 O(nlogn), 最坏 O(n^2), 不稳定
 * =========================================== */
int Partition(int A[], int low, int high) {
    int pivot = A[low]; /* 取第一个作为枢轴 */
    while (low < high) {
        while (low < high && A[high] >= pivot) high--;
        A[low] = A[high]; /* 小的换到左边 */
        while (low < high && A[low] <= pivot) low++;
        A[high] = A[low]; /* 大的换到右边 */
    }
    A[low] = pivot; /* 枢轴就位 */
    return low;
}

void QuickSort(int A[], int low, int high) {
    if (low < high) {
        int pos = Partition(A, low, high);
        QuickSort(A, low, pos - 1);
        QuickSort(A, pos + 1, high);
    }
}


/* ===========================================
 *  6. 简单选择排序
 *     每趟从未排序部分选最小的, 放到已排序末尾
 *     时间始终 O(n^2), 不稳定
 * =========================================== */
void SelectSort(int A[], int n) {
    int i, j, min;
    for (i = 0; i < n - 1; i++) {
        min = i;
        for (j = i + 1; j < n; j++)
            if (A[j] < A[min]) min = j;
        if (min != i) swap(&A[i], &A[min]);
    }
}


/* ===========================================
 *  7. 堆排序 (下标从 1 开始, A[0] 作暂存单元)
 *     建堆 O(n), 排序 O(nlogn), 空间 O(1), 不稳定
 * =========================================== */
void AdjustDown(int A[], int k, int len) {
    int i;
    A[0] = A[k]; /* 暂存 */
    for (i = 2 * k; i <= len; i *= 2) {
        if (i < len && A[i] < A[i + 1]) i++; /* 找较大的孩子 */
        if (A[0] >= A[i]) break; /* 根比孩子大, 到位 */
        A[k] = A[i]; /* 大孩子上浮 */
        k = i;
    }
    A[k] = A[0]; /* 放到最终位置 */
}

void BuildMaxHeap(int A[], int len) {
    int i;
    for (i = len / 2; i > 0; i--) /* 从最后一个非叶结点开始 */
        AdjustDown(A, i, len);
}

void HeapSort(int A[], int len) {
    BuildMaxHeap(A, len);      /* 建初始大顶堆 */
    int i;
    for (i = len; i > 1; i--) {
        swap(&A[1], &A[i]);    /* 堆顶(最大)与末尾交换 */
        AdjustDown(A, 1, i - 1); /* 剩余元素重新调整 */
    }
}


/* ===========================================
 *  8. 归并排序
 *     辅助数组 B 开在外面, 避免反复分配
 *     时间总是 O(nlogn), 空间 O(n), 稳定
 * =========================================== */
int B_merge[50]; /* 辅助数组 */

void Merge(int A[], int low, int mid, int high) {
    int i, j, k;
    for (k = low; k <= high; k++) B_merge[k] = A[k]; /* 复制 */

    for (i = low, j = mid + 1, k = low; i <= mid && j <= high; k++) {
        if (B_merge[i] <= B_merge[j])
            A[k] = B_merge[i++];
        else
            A[k] = B_merge[j++];
    }
    while (i <= mid)  A[k++] = B_merge[i++];
    while (j <= high) A[k++] = B_merge[j++];
}

void MergeSort(int A[], int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;
        MergeSort(A, low, mid);
        MergeSort(A, mid + 1, high);
        Merge(A, low, mid, high);
    }
}


/* ========== main: 全部排序对比演示 ========== */
int main() {
    int original[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int n = 8;
    int A[8];

    printf("===== 408 八大排序 可运行演示 =====\n");
    printf("原始数组: "); PrintArray(original, n);
    printf("\n");

    CopyArray(original, A, n);
    InsertSort(A, n);
    printf("直接插入: "); PrintArray(A, n);

    CopyArray(original, A, n);
    BinaryInsertSort(A, n);
    printf("折半插入: "); PrintArray(A, n);

    CopyArray(original, A, n);
    ShellSort(A, n);
    printf("希尔排序: "); PrintArray(A, n);

    CopyArray(original, A, n);
    BubbleSort(A, n);
    printf("冒泡排序: "); PrintArray(A, n);

    CopyArray(original, A, n);
    QuickSort(A, 0, n - 1);
    printf("快速排序: "); PrintArray(A, n);

    CopyArray(original, A, n);
    SelectSort(A, n);
    printf("选择排序: "); PrintArray(A, n);

    /* 堆排序: A[0] 做暂存, 数据从 A[1] 开始 */
    printf("\n--- 堆排序 (下标从1开始) ---\n");
    int H[] = {0, 49, 38, 65, 97, 76, 13, 27, 49}; /* H[0]不用 */
    printf("排序前: ");
    int i;
    for (i = 1; i <= 8; i++) printf("%d ", H[i]);
    printf("\n");
    HeapSort(H, 8);
    printf("排序后: ");
    for (i = 1; i <= 8; i++) printf("%d ", H[i]);
    printf("\n");

    printf("\n--- 归并排序 ---\n");
    CopyArray(original, A, n);
    printf("排序前: "); PrintArray(A, n);
    MergeSort(A, 0, n - 1);
    printf("排序后: "); PrintArray(A, n);

    return 0;
}
