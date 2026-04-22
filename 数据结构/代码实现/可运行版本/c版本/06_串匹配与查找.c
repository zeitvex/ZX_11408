/*
 * ============================================================
 *   408 考研数据结构 —— KMP 串匹配与查找 (可编译运行版)
 *   文件: 06_串匹配与查找.c
 *   编译: gcc 06_串匹配与查找.c -o kmp && kmp
 * ============================================================
 *
 *   本文件包含:
 *     1. ★ KMP 算法: get_next 求 next 数组
 *     2. ★ KMP 算法: get_nextval 求 nextval 数组
 *     3. ★ KMP 主匹配函数 Index_KMP
 *     4. 折半查找 (非递归)
 *
 *   重要说明:
 *     教材中字符串下标从 1 开始, ch[0] 存长度或不用。
 *     本文件为了可运行性, 同时展示了:
 *       - 教材 1-based 风格 (考试用这个)
 *       - C 语言 0-based 风格 (理解用)
 * ============================================================
 */

#include <stdio.h>
#include <string.h>

/* ========== 1-based KMP (严格教材风格, 用于考试) ========== */

/*
 *  get_next: 从模式串 T 推导出 next 数组
 *
 *  next[j] 的含义:
 *    当 T[j] 与主串失配时, j 应该回退到 next[j] 的位置
 *    即模式串的前 next[j]-1 个字符已经和主串匹配好了
 *
 *  手算提示: next[1] 固定为 0, next[2] 固定为 1,
 *            从第 3 位开始, 找前缀和后缀的最长公共子串长度+1
 */
void get_next(char T[], int Tlen, int next[]) {
    int i = 1, j = 0;
    next[1] = 0;

    while (i < Tlen) {
        if (j == 0 || T[i] == T[j]) {
            ++i; ++j;
            next[i] = j;
        } else {
            j = next[j]; /* 回退 */
        }
    }
}

/*
 *  get_nextval: next 数组的优化版
 *
 *  当 T[i] == T[next[i]] 时, 即使回退到 next[i],
 *  同样会失配 (因为字符相同), 所以直接继承 nextval[next[i]]
 */
void get_nextval(char T[], int Tlen, int nextval[]) {
    int i = 1, j = 0;
    nextval[1] = 0;

    while (i < Tlen) {
        if (j == 0 || T[i] == T[j]) {
            ++i; ++j;
            if (T[i] != T[j])
                nextval[i] = j;        /* 不等, 正常赋值 */
            else
                nextval[i] = nextval[j]; /* 相等, 继承 */
        } else {
            j = nextval[j];
        }
    }
}

/*
 *  KMP 主匹配: 在 S 中查找 T 第一次出现的位置
 *  下标从 1 开始
 */
int Index_KMP(char S[], int Slen, char T[], int Tlen, int next[]) {
    int i = 1, j = 1;

    while (i <= Slen && j <= Tlen) {
        if (j == 0 || S[i] == T[j]) {
            ++i; ++j;
        } else {
            j = next[j]; /* 模式串回退, 主串不回溯 */
        }
    }

    if (j > Tlen)
        return i - Tlen; /* 返回匹配位置 (1-based) */
    else
        return 0;        /* 未找到 */
}


/* ========== 折半查找 ========== */
int BinarySearch(int A[], int n, int key) {
    int low = 0, high = n - 1, mid;
    while (low <= high) {
        mid = (low + high) / 2;
        if (A[mid] == key) return mid;
        else if (A[mid] > key) high = mid - 1;
        else low = mid + 1;
    }
    return -1;
}


/* ========== main 演示 ========== */
int main() {
    printf("===== 408 KMP 串匹配与查找 可运行演示 =====\n\n");

    /* --- KMP 演示 ---
     * 注意: 教材风格, S[0] 和 T[0] 不用, 有效字符从下标 1 开始
     * 所以我们在字符串前面加一个占位符 '#'
     */
    /* 主串:   "aababaabaabcac"  (14 个字符) */
    /* 模式串: "abaabcac"        (8 个字符)  */
    char S[] = "#aababaabaabcac"; /* S[1]='a', S[2]='a', ... */
    char T[] = "#abaabcac";       /* T[1]='a', T[2]='b', ... */
    int Slen = 14, Tlen = 8;

    int next[20], nextval[20];
    get_next(T, Tlen, next);
    get_nextval(T, Tlen, nextval);

    int i;
    printf("模式串 T = \"abaabcac\"\n");
    printf("位序:    ");
    for (i = 1; i <= Tlen; i++) printf("%d ", i);
    printf("\n字符:    ");
    for (i = 1; i <= Tlen; i++) printf("%c ", T[i]);
    printf("\nnext:    ");
    for (i = 1; i <= Tlen; i++) printf("%d ", next[i]);
    printf("\nnextval: ");
    for (i = 1; i <= Tlen; i++) printf("%d ", nextval[i]);
    printf("\n\n");

    int pos = Index_KMP(S, Slen, T, Tlen, next);
    printf("主串 S = \"aababaabaabcac\"\n");
    if (pos > 0)
        printf("T 在 S 中第一次出现的位置 = %d\n\n", pos);
    else
        printf("未找到\n\n");

    /* --- 折半查找 --- */
    printf("--- 折半查找 ---\n");
    int arr[] = {7, 10, 13, 16, 19, 29, 32, 33, 37, 41, 43};
    int n = 11;
    printf("有序数组: ");
    for (i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    int key = 33;
    int result = BinarySearch(arr, n, key);
    printf("查找 %d -> 下标 %d\n", key, result);

    key = 20;
    result = BinarySearch(arr, n, key);
    printf("查找 %d -> %s\n", key, result == -1 ? "未找到" : "找到了");

    return 0;
}
