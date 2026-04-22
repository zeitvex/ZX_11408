#include <iostream>
using namespace std;

/* =========================================
 * 408 统考数据结构核心代码 - 串与查找 (C++ 伪代码)
 * ========================================= */

// 定义传统的考试专用 SString
typedef struct {
    char ch[100]; 
    int length;
} SString;

/* -----------------------------------------
 * 【1】字符串匹配：KMP 算法核心
 * 注意：由于 C++ 可以使用引用，我们可以用 `&`
 * ----------------------------------------- */

// 1. 推导 next 数组的灵魂逻辑
void get_next(SString T, int next[]) {
    int i = 1, j = 0;
    next[1] = 0; // 王道定义第一位就是 0
    
    while (i < T.length) {
        if (j == 0 || T.ch[i] == T.ch[j]) {
            ++i; ++j;
            next[i] = j; 
        } else {
            j = next[j]; // 发生失配，神级跳跃
        }
    }
}

// 2. 推导加强版容错 nextval 数组 (防坑利器)
void get_nextval(SString T, int nextval[]) {
    int i = 1, j = 0;
    nextval[1] = 0;
    
    while (i < T.length) {
        if (j == 0 || T.ch[i] == T.ch[j]) {
            ++i; ++j;
            // 如果新匹配上的那个字符，居然和旧的失配字符是同一个字！
            // 那么既然刚刚都失配了，你再让他拿同样的字去配也是必然失配！
            // 故直接跨过它，继承它的 next 归宿。
            if (T.ch[i] != T.ch[j]) {
                nextval[i] = j;
            } else {
                nextval[i] = nextval[j]; 
            }
        } else {
            j = nextval[j];
        }
    }
}

// 3. KMP 主干匹配计算
int Index_KMP(SString S, SString T, int next[]) {
    int i = 1, j = 1; 
    
    while (i <= S.length && j <= T.length) {
        if(j == 0 || S.ch[i] == T.ch[j]) {
            ++i; ++j; 
        } else {
            j = next[j]; 
        }
    }
    
    if (j > T.length) return i - T.length; // 返回子串在主串中的起始下标
    else return 0;
}

/* -----------------------------------------
 * 【2】折半查找 (Binary Search)
 * ----------------------------------------- */

// 折半查找：找得到就返回数组下标对应的人，找不到滚回 -1
int Binary_Search(int A[], int len, int key) {
    int low = 0, high = len - 1, mid;
    
    while (low <= high) {
        mid = (low + high) / 2;
        
        if (A[mid] == key) return mid;  
        else if (A[mid] > key) high = mid - 1; 
        else low = mid + 1;  
    }
    
    return -1; 
}
