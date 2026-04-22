#include <stdio.h>
#include <stdlib.h>

/* =========================================
 * 408 统考数据结构核心代码 - 排序算法 (纯 C 语言风格)
 * 考试默写要求：快速排序的 Partition 函数每年极小选择和算法涉及极多！
 * ========================================= */

// 函数：交换两者
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* -----------------------------------------
 * 【1】快速排序及其核心 Partition 划分
 * ----------------------------------------- */

// 快速排序的划分思想（408常考：求第K大，奇偶分离等变形均由此出）
int Partition(int A[], int low, int high) {
    int pivot = A[low]; // 取第一个元素作为枢轴
    while (low < high) {
        // 从后往前找比枢轴小的
        while (low < high && A[high] >= pivot) high--; 
        A[low] = A[high]; // 扔到左边
        
        // 从前往后找比枢轴大的
        while (low < high && A[low] <= pivot) low++;
        A[high] = A[low]; // 扔到右边
    }
    A[low] = pivot; // 枢轴到达最终的位置！
    return low;
}

// 快排主骨架
void QuickSort(int A[], int low, int high) {
    if (low < high) {
        int pivotpos = Partition(A, low, high); // 划分
        QuickSort(A, low, pivotpos - 1);        // 治理左边
        QuickSort(A, pivotpos + 1, high);       // 治理右边
    }
}


/* -----------------------------------------
 * 【2】起泡排序 (冒泡) / 稳定的基础大哥
 * ----------------------------------------- */
void BubbleSort(int A[], int n) {
    int flag; // 标志位用于判断某趟有没有发生交换
    for (int i = 0; i < n - 1; i++) {
        flag = 0; // 假设没交换
        for (int j = n - 1; j > i; j--) { // 从后往前冒泡
            if (A[j - 1] > A[j]) {
                swap(&A[j - 1], &A[j]);
                flag = 1;
            }
        }
        if (flag == 0) return; // 本趟全都没交换，说明已经彻底有序了！打卡下班。
    }
}


/* -----------------------------------------
 * 【3】折半插入排序 
 * ----------------------------------------- */
void InsertSortBinary(int A[], int n) {
    int i, j, low, high, mid;
    // 数组下标从1到n有效的话（A[0]作为哨兵或者辅助位保存数据）
    for (i = 2; i <= n; i++) {
        A[0] = A[i]; // 将 A[i] 暂存
        low = 1; 
        high = i - 1; 
        
        // 折半查找应该插入的位置
        while (low <= high) {
            mid = (low + high) / 2;
            if (A[mid] > A[0]) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        // 统一后移，空出位置
        for (j = i - 1; j >= high + 1; j--) {
            A[j + 1] = A[j];
        }
        A[high + 1] = A[0]; // 填入
    }
}

/* -----------------------------------------
 * 【4】归并排序 (Merge Sort) 
 * 【极度重要】需要单独开辟辅助数组 B，这是耗费空间 O(n) 的来源
 * ----------------------------------------- */
int B[100]; // 辅助数组

// 归并两个有序小段的核心机制
void Merge(int A[], int low, int mid, int high) {
    int i, j, k;
    for (k = low; k <= high; ++k) {
        B[k] = A[k]; // 临时复制到背板去
    }
    
    // i走前段表，j走后段表，看两边较小的值依次填入 A 中归并
    for (i = low, j = mid + 1, k = i; i <= mid && j <= high; ++k) {
        if (B[i] <= B[j]) {
            A[k] = B[i++]; 
        } else {
            A[k] = B[j++];
        }
    }
    
    // 如果某一段多出来了，将剩下的所有统统灌入末尾结账
    while (i <= mid)  A[k++] = B[i++];
    while (j <= high) A[k++] = B[j++];
}

// 归并排序递归骨架
void MergeSort(int A[], int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;
        MergeSort(A, low, mid);       // 排理前一半
        MergeSort(A, mid + 1, high);  // 排理后一半
        Merge(A, low, mid, high);     // 天下合并
    }
}

/* -----------------------------------------
 * 【5】408 快排核心变式应用集锦 
 * 这往往出现在 11 到 15 分的大题中，让你 O(n) 完成特殊分离！
 * ----------------------------------------- */

// 王道经典考题：把所有的奇数移向数组前端，全部偶数移向末尾，要求时间 O(N) 一趟！
// 【破局原理】直接调用 Partition 思想。
void MoveOddEven(int A[], int len) {
    int low = 0, high = len - 1;
    while(low < high) {
        // 后方找到第一个奇数
        while(low < high && A[high] % 2 == 0) high--;
        // 前方找到第一个偶数
        while(low < high && A[low] % 2 != 0) low++;
        
        if (low < high) {
            swap(&A[low], &A[high]); // 被抓到两人不符合规矩，交换换身！
        }
    }
}

