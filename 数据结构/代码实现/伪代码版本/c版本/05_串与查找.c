#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* =========================================
 * 408 统考数据结构核心代码 - 串与查找 (C 版本)
 * KMP 算法的 next 数组推导属于选择与大题的高频常青树！
 * ========================================= */

/* -----------------------------------------
 * 【1】字符串匹配：KMP 算法核心
 * 特别注意：王道考研中，字符串通常从下标 1 开始存储，
 * S.ch[0] 用来存放真实的长度。
 * ----------------------------------------- */

typedef struct {
    char ch[100]; // 静态分配
    int length;
} SString;

// 1. 推导 next 数组的灵魂逻辑 (必背)
void get_next(SString T, int next[]) {
    int i = 1, j = 0;
    next[1] = 0;
    
    // 如果还没推到头
    while (i < T.length) {
        if (j == 0 || T.ch[i] == T.ch[j]) {
            ++i; ++j;
            next[i] = j; // 匹配上了就将匹配长度同步递增赋予给下一位
        } else {
            j = next[j]; // 没匹配上，j 回退到次级的容错庇护所
        }
    }
}

// 2. KMP 主干匹配计算
int Index_KMP(SString S, SString T, int next[]) {
    int i = 1, j = 1; // 设从 1 开始
    
    while (i <= S.length && j <= T.length) {
        if(j == 0 || S.ch[i] == T.ch[j]) {
            ++i; ++j; // 匹配就接着往下走
        } else {
            j = next[j]; // 失配借用 next 发生神级跳跃回溯！
        }
    }
    
    if (j > T.length) {
        return i - T.length; // 找到了
    } else {
        return 0; // 没找到
    }
}


/* -----------------------------------------
 * 【2】折半查找 (Binary Search)
 * 要求必须建立在有序并且拥有随机访问能力(数组)的基础上！
 * ----------------------------------------- */

// 非递归折半查找 (最常用)
int Binary_Search(int A[], int len, int key) {
    int low = 0;
    int high = len - 1;
    int mid;
    
    while (low <= high) {
        mid = (low + high) / 2;
        
        if (A[mid] == key) {
            return mid;  // 匹配成功，返回位置下标
        } else if (A[mid] > key) {
            high = mid - 1; // 查左半边
        } else {
            low = mid + 1;  // 查右半边
        }
    }
    
    return -1; // 查找失败
}

// 递归版本的折半查找 (大题中有可能会用于分治变形)
int Binary_Search_Rec(int A[], int low, int high, int key) {
    if (low > high) return -1;
    
    int mid = (low + high) / 2;
    if (A[mid] == key) return mid;
    if (A[mid] > key) {
        return Binary_Search_Rec(A, low, mid - 1, key);
    } else {
        return Binary_Search_Rec(A, mid + 1, high, key);
    }
}
