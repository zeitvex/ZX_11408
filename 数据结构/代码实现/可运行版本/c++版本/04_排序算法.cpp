/*
 * ============================================================
 *   408 考研数据结构 —— 八大排序 (可运行 C++ 版)
 *   文件: 04_排序算法.cpp
 *   编译: g++ 04_排序算法.cpp -o test && ./test
 * ============================================================
 *
 *   包含全部 8 种考纲排序算法:
 *     1. 直接插入排序          5. 快速排序
 *     2. 折半插入排序          6. 简单选择排序
 *     3. 希尔排序              7. 堆排序 (下标从1)
 *     4. 冒泡排序              8. 归并排序
 * ============================================================
 */

#include <cstdio>

/* 工具函数 */
void PrintArr(int A[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", A[i]);
    printf("\n");
}

void CopyArr(int src[], int dst[], int n) {
    for (int i = 0; i < n; i++)
        dst[i] = src[i];
}

void swap(int &a, int &b) {
    int t = a; a = b; b = t;
}


/* ===========================================
 *  1. 直接插入排序
 *     稳定, 最好 O(n), 最坏/平均 O(n^2)
 * =========================================== */
void InsertSort(int A[], int n) {
    for (int i = 1; i < n; i++) {
        int temp = A[i];
        int j;
        for (j = i - 1; j >= 0 && temp < A[j]; j--)
            A[j + 1] = A[j];   // 比 temp 大的后移
        A[j + 1] = temp;        // 插入到正确位置
    }
}


/* ===========================================
 *  2. 折半插入排序
 *     稳定, 比较次数减少但移动不变, O(n^2)
 * =========================================== */
void BinaryInsertSort(int A[], int n) {
    for (int i = 1; i < n; i++) {
        int temp = A[i];
        int low = 0, high = i - 1;

        // 折半查找插入位置
        while (low <= high) {
            int mid = (low + high) / 2;
            if (A[mid] > temp)
                high = mid - 1;
            else
                low = mid + 1;
        }
        // high+1 就是插入位置, 后移 [high+1, i-1]
        for (int j = i - 1; j >= high + 1; j--)
            A[j + 1] = A[j];
        A[high + 1] = temp;
    }
}


/* ===========================================
 *  3. 希尔排序
 *     不稳定, 约 O(n^1.3)
 * =========================================== */
void ShellSort(int A[], int n) {
    for (int dk = n / 2; dk >= 1; dk /= 2) {       // 增量递减
        for (int i = dk; i < n; i++) {               // 对每组做插排
            if (A[i] < A[i - dk]) {
                int temp = A[i];
                int j;
                for (j = i - dk; j >= 0 && temp < A[j]; j -= dk)
                    A[j + dk] = A[j];
                A[j + dk] = temp;
            }
        }
    }
}


/* ===========================================
 *  4. 冒泡排序
 *     稳定, 最好 O(n), 最坏 O(n^2)
 * =========================================== */
void BubbleSort(int A[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = n - 1; j > i; j--) {
            if (A[j - 1] > A[j]) {
                swap(A[j - 1], A[j]);
                swapped = true;
            }
        }
        if (!swapped) return;   // 本趟无交换, 已有序
    }
}


/* ===========================================
 *  5. 快速排序
 *     不稳定, 平均 O(nlogn), 最坏 O(n^2)
 * =========================================== */
int Partition(int A[], int low, int high) {
    int pivot = A[low];             // 取第一个作为枢轴

    while (low < high) {
        while (low < high && A[high] >= pivot)
            high--;                  // 从右找小的
        A[low] = A[high];

        while (low < high && A[low] <= pivot)
            low++;                   // 从左找大的
        A[high] = A[low];
    }
    A[low] = pivot;                  // 枢轴就位
    return low;
}

void QuickSort(int A[], int low, int high) {
    if (low < high) {
        int pos = Partition(A, low, high);
        QuickSort(A, low, pos - 1);   // 左半递归
        QuickSort(A, pos + 1, high);  // 右半递归
    }
}


/* ===========================================
 *  6. 简单选择排序
 *     不稳定, 时间始终 O(n^2)
 * =========================================== */
void SelectSort(int A[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++)
            if (A[j] < A[min]) min = j;
        if (min != i)
            swap(A[i], A[min]);
    }
}


/* ===========================================
 *  7. 堆排序 (大顶堆, 下标从 1, A[0] 作暂存)
 *     不稳定, O(nlogn), 空间 O(1)
 * =========================================== */
void AdjustDown(int A[], int k, int len) {
    A[0] = A[k];                     // 暂存要调整的结点
    for (int i = 2 * k; i <= len; i *= 2) {
        if (i < len && A[i] < A[i + 1])
            i++;                      // 找较大的孩子
        if (A[0] >= A[i])
            break;                    // 到位
        A[k] = A[i];                  // 大孩子上浮
        k = i;
    }
    A[k] = A[0];                      // 放到最终位置
}

void HeapSort(int A[], int len) {
    // 建初始大顶堆
    for (int i = len / 2; i > 0; i--)
        AdjustDown(A, i, len);

    // 排序: 不断交换堆顶与末尾, 再调整
    for (int i = len; i > 1; i--) {
        swap(A[1], A[i]);             // 堆顶与末尾交换
        AdjustDown(A, 1, i - 1);     // 调整剩余
    }
}


/* ===========================================
 *  8. 归并排序
 *     稳定, O(nlogn), 空间 O(n)
 * =========================================== */
int B_merge[50];   // 辅助数组

void Merge(int A[], int low, int mid, int high) {
    // 复制到辅助数组
    for (int k = low; k <= high; k++)
        B_merge[k] = A[k];

    // 归并: 两路合一
    int i = low, j = mid + 1, k = low;
    while (i <= mid && j <= high) {
        if (B_merge[i] <= B_merge[j])
            A[k++] = B_merge[i++];
        else
            A[k++] = B_merge[j++];
    }
    while (i <= mid)  A[k++] = B_merge[i++];
    while (j <= high) A[k++] = B_merge[j++];
}

void MergeSort(int A[], int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;
        MergeSort(A, low, mid);       // 排左半
        MergeSort(A, mid + 1, high);  // 排右半
        Merge(A, low, mid, high);     // 合并
    }
}


/* ========== main: 全部排序对比 ========== */
int main() {
    printf("===== 八大排序 C++ 可运行版 =====\n");
    int original[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int n = 8, A[8];

    printf("原始数组: ");
    PrintArr(original, n);
    printf("\n");

    CopyArr(original, A, n); InsertSort(A, n);
    printf("直接插入: "); PrintArr(A, n);

    CopyArr(original, A, n); BinaryInsertSort(A, n);
    printf("折半插入: "); PrintArr(A, n);

    CopyArr(original, A, n); ShellSort(A, n);
    printf("希尔排序: "); PrintArr(A, n);

    CopyArr(original, A, n); BubbleSort(A, n);
    printf("冒泡排序: "); PrintArr(A, n);

    CopyArr(original, A, n); QuickSort(A, 0, n - 1);
    printf("快速排序: "); PrintArr(A, n);

    CopyArr(original, A, n); SelectSort(A, n);
    printf("选择排序: "); PrintArr(A, n);

    CopyArr(original, A, n); MergeSort(A, 0, n - 1);
    printf("归并排序: "); PrintArr(A, n);

    /* 堆排序: A[0] 暂存, 数据从 A[1] 开始 */
    printf("\n--- 堆排序 (下标从1) ---\n");
    int H[] = {0, 49, 38, 65, 97, 76, 13, 27, 49};
    printf("排序前: ");
    for (int i = 1; i <= 8; i++) printf("%d ", H[i]);
    printf("\n");
    HeapSort(H, 8);
    printf("排序后: ");
    for (int i = 1; i <= 8; i++) printf("%d ", H[i]);
    printf("\n");

    return 0;
}
