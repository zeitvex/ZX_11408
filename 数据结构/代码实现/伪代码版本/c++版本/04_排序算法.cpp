#include <iostream>
using namespace std;

/* =========================================
 * 408 统考数据结构核心代码 - 排序算法 (C++ 风格)
 * ========================================= */

// C++ 中可以直接使用 std::swap，无需自己写指针交换方法
// 也可以写引用的 swap 如下:
void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

/* -----------------------------------------
 * 【1】快速排序 (划区王牌)
 * ----------------------------------------- */

// 快速排序的划分思想，C++伪码中更喜欢使用引用传递与直接赋值
int Partition(int A[], int low, int high) {
    int pivot = A[low]; 
    while (low < high) {
        while (low < high && A[high] >= pivot) high--; 
        A[low] = A[high]; 
        
        while (low < high && A[low] <= pivot) low++;
        A[high] = A[low]; 
    }
    A[low] = pivot; 
    return low;
}

void QuickSort(int A[], int low, int high) {
    if (low < high) {
        int pivotpos = Partition(A, low, high); 
        QuickSort(A, low, pivotpos - 1);        
        QuickSort(A, pivotpos + 1, high);       
    }
}


/* -----------------------------------------
 * 【2】起泡排序
 * ----------------------------------------- */
void BubbleSort(int A[], int n) {
    bool flag; // C++ 具有内置的 bool 数据类型，不需要用 1/0 代替
    for (int i = 0; i < n - 1; i++) {
        flag = false; 
        for (int j = n - 1; j > i; j--) { 
            if (A[j - 1] > A[j]) {
                swap(A[j - 1], A[j]); // 若使用自定义 swap，可以直接传参数
                flag = true;
            }
        }
        if (!flag) return; // 提前下班
    }
}

/* -----------------------------------------
 * 【3】堆排序 (极重要：大顶堆构建)
 * ----------------------------------------- */

// 将以 k 为根的子树调整为大顶堆 (向下调整)
void HeadAdjust(int A[], int k, int len) {
    A[0] = A[k]; // A[0] 暂存根结点 (假设数组 0 为空缺/暂存位)
    
    // i 初始为左孩子
    for (int i = 2 * k; i <= len; i *= 2) {
        // 如果右孩子更大，就把 i 指向右孩子
        if (i < len && A[i] < A[i + 1]) {
            i++; 
        }
        // 反之，要是根居然比最大的孩子还大，那就已经是个大顶堆了
        if (A[0] >= A[i]) {
            break;
        } else {
            A[k] = A[i]; // 大的给到双亲
            k = i;       // 继续往下挖
        }
    }
    A[k] = A[0]; // 将筛选结点放到最终应该在的位置
}

// 建立大顶堆
void BuildMaxHeap(int A[], int len) {
    // 从最后一个非叶子结点开始反复往下调整
    for (int i = len / 2; i > 0; i--) {
        HeadAdjust(A, i, len);
    }
}

// 整体堆排序
void HeapSort(int A[], int len) {
    BuildMaxHeap(A, len);       // 1. 初始化堆
    for (int i = len; i > 1; i--) {
        swap(A[i], A[1]);       // 2. 将堆顶（天枢最大值）摘下来放到队尾
        HeadAdjust(A, 1, i - 1);// 3. 剥削被摘走的范围内的剩余元素重新建堆
    }
}

/* -----------------------------------------
 * 【4】归并排序 (Merge Sort) 
 * 【极度重要】辅以全局或临时的辅助数组 B
 * ----------------------------------------- */
int B[100]; // 外部开辟辅助数组防止每次归并时发生反复分配的时间开销

void Merge(int A[], int low, int mid, int high) {
    int i, j, k;
    for (k = low; k <= high; ++k) B[k] = A[k]; // 复制
    
    // 双指针法在两个有序序列中抽取较小者放入原数组
    for (i = low, j = mid + 1, k = i; i <= mid && j <= high; ++k) {
        if (B[i] <= B[j]) A[k] = B[i++]; 
        else A[k] = B[j++];
    }
    
    while (i <= mid)  A[k++] = B[i++];
    while (j <= high) A[k++] = B[j++];
}

void MergeSort(int A[], int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;
        MergeSort(A, low, mid);       
        MergeSort(A, mid + 1, high);  
        Merge(A, low, mid, high);     
    }
}

/* -----------------------------------------
 * 【5】基于 QuickSort 的特殊分离应用
 * 把所有的奇数移向数组前端，全部偶数移向末尾，要求时间 O(N) 一趟！
 * ----------------------------------------- */

void MoveOddEven(int A[], int len) {
    int low = 0, high = len - 1;
    while(low < high) {
        // 后方找到第一个可以前移的奇数
        while(low < high && A[high] % 2 == 0) high--;
        // 前方找到第一个可以后移的偶数
        while(low < high && A[low] % 2 != 0) low++;
        
        if (low < high) {
            swap(A[low], A[high]); // 被抓到则交换
        }
    }
}

