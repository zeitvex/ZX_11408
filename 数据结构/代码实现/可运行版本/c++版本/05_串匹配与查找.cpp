/*
 * ============================================================
 *   408 考研数据结构 —— KMP 与查找 (可运行 C++ 版)
 *   文件: 05_串匹配与查找.cpp
 *   编译: g++ 05_串匹配与查找.cpp -o test && ./test
 * ============================================================
 *
 *   覆盖内容:
 *     1. ★ KMP: get_next 求 next 数组 (1-based, 教材风格)
 *     2. ★ KMP: get_nextval 求 nextval 数组
 *     3. ★ KMP: Index_KMP 主匹配函数
 *     4. 折半查找 (非递归)
 * ============================================================
 */

#include <cstdio>
#include <cstring>


/* ========== KMP 算法 (1-based, 教材风格) ========== */

/*
 * get_next: 求模式串 T 的 next 数组
 *
 * next[j] 含义: 当 T[j] 与主串失配时, j 应回退到 next[j]
 * 手算提示: next[1]=0, next[2]=1, 从第3位开始找最长公共前后缀+1
 */
void get_next(char T[], int Tlen, int next[]) {
    int i = 1, j = 0;
    next[1] = 0;

    while (i < Tlen) {
        if (j == 0 || T[i] == T[j]) {
            ++i;
            ++j;
            next[i] = j;
        } else {
            j = next[j];   // 回退
        }
    }
}

/*
 * get_nextval: next 的优化版
 *
 * 当 T[i] == T[next[i]] 时, 回退后仍会失配
 * 所以直接继承 nextval[next[i]]
 */
void get_nextval(char T[], int Tlen, int nextval[]) {
    int i = 1, j = 0;
    nextval[1] = 0;

    while (i < Tlen) {
        if (j == 0 || T[i] == T[j]) {
            ++i;
            ++j;
            if (T[i] != T[j])
                nextval[i] = j;            // 不等, 正常赋值
            else
                nextval[i] = nextval[j];   // 相等, 继承优化
        } else {
            j = nextval[j];
        }
    }
}

/*
 * KMP 主匹配: 在 S 中查找 T 第一次出现的位置 (1-based)
 * 主串不回溯, 模式串利用 next 跳转
 */
int Index_KMP(char S[], int Slen, char T[], int Tlen, int next[]) {
    int i = 1, j = 1;

    while (i <= Slen && j <= Tlen) {
        if (j == 0 || S[i] == T[j]) {
            ++i;
            ++j;
        } else {
            j = next[j];   // 模式串回退
        }
    }

    if (j > Tlen)
        return i - Tlen;    // 匹配成功, 返回起始位置
    else
        return 0;           // 未找到
}


/* ========== 折半查找 ========== */

/*
 * 有序数组中查找 key, 找到返回下标, 找不到返回 -1
 * 时间 O(logn)
 */
int BinarySearch(int A[], int n, int key) {
    int low = 0, high = n - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        if (A[mid] == key)
            return mid;
        else if (A[mid] > key)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return -1;  // 未找到
}


/* ========== main 演示 ========== */
int main() {
    printf("===== KMP 与查找 C++ 可运行版 =====\n\n");

    /* --- KMP 演示 ---
     * 教材风格: 下标从 1 开始, ch[0] 用 '#' 占位
     */
    char S[] = "#aababaabaabcac";   // 主串, 14 个有效字符
    char T[] = "#abaabcac";         // 模式串, 8 个有效字符
    int Slen = 14, Tlen = 8;
    int next[20], nextval[20];

    get_next(T, Tlen, next);
    get_nextval(T, Tlen, nextval);

    printf("模式串 T = \"abaabcac\"\n");
    printf("位序:    ");
    for (int i = 1; i <= Tlen; i++) printf("%d ", i);
    printf("\n字符:    ");
    for (int i = 1; i <= Tlen; i++) printf("%c ", T[i]);
    printf("\nnext:    ");
    for (int i = 1; i <= Tlen; i++) printf("%d ", next[i]);
    printf("\nnextval: ");
    for (int i = 1; i <= Tlen; i++) printf("%d ", nextval[i]);
    printf("\n\n");

    int pos = Index_KMP(S, Slen, T, Tlen, next);
    printf("主串 S = \"aababaabaabcac\"\n");
    printf("匹配位置 = %d\n\n", pos);

    /* --- 折半查找 --- */
    printf("--- 折半查找 ---\n");
    int arr[] = {7, 10, 13, 16, 19, 29, 32, 33, 37, 41, 43};
    int n = 11;
    printf("有序数组: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    printf("查找 33 -> 下标 %d\n", BinarySearch(arr, n, 33));
    printf("查找 20 -> %s\n",
           BinarySearch(arr, n, 20) == -1 ? "未找到" : "找到");

    return 0;
}
