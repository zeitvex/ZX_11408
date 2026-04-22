/*
 * ============================================================
 *   408 考研数据结构 —— 栈与队列 (可编译运行版)
 *   文件: 03_栈与队列.c
 *   编译: gcc 03_栈与队列.c -o stack_queue && stack_queue
 * ============================================================
 *
 *   本文件包含:
 *     1. 顺序栈: 初始化 / 进栈 / 出栈 / 判空
 *     2. 循环队列 (牺牲一个单元法): 初始化 / 入队 / 出队 / 判空
 *     3. ★ 栈的经典应用: 括号匹配
 *     4. ★ 栈的经典应用: 十进制转二进制
 *     5. ★ 栈的经典应用: 后缀表达式求值
 *
 *   考研重点:
 *     - 顺序栈 top 初始为 -1, 先加后存
 *     - 循环队列的 3 种判空判满方案 (本文件展示最常考的牺牲一个单元法)
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MaxSize 50
typedef int ElemType;

/* ========== 顺序栈 ========== */
typedef struct {
    ElemType data[MaxSize];
    int top; /* 栈顶指针, 初始化为 -1 */
} SqStack;

void InitStack(SqStack *S) { S->top = -1; }
int StackEmpty(SqStack *S) { return S->top == -1; }

int Push(SqStack *S, ElemType x) {
    if (S->top == MaxSize - 1) return 0; /* 栈满 */
    S->data[++S->top] = x;  /* 先加指针, 再存数据 */
    return 1;
}

int Pop(SqStack *S, ElemType *x) {
    if (S->top == -1) return 0; /* 栈空 */
    *x = S->data[S->top--]; /* 先取数据, 再减指针 */
    return 1;
}

int GetTop(SqStack *S, ElemType *x) {
    if (S->top == -1) return 0;
    *x = S->data[S->top]; /* 只看不取 */
    return 1;
}


/* ========== 循环队列 (牺牲一个单元法) ========== */
typedef struct {
    ElemType data[MaxSize];
    int front, rear;
} SqQueue;

void InitQueue(SqQueue *Q) { Q->front = Q->rear = 0; }
int QueueEmpty(SqQueue *Q) { return Q->front == Q->rear; }

/* 判满: rear 的下一个位置是 front 就满了 */
int EnQueue(SqQueue *Q, ElemType x) {
    if ((Q->rear + 1) % MaxSize == Q->front) return 0;
    Q->data[Q->rear] = x;
    Q->rear = (Q->rear + 1) % MaxSize;
    return 1;
}

int DeQueue(SqQueue *Q, ElemType *x) {
    if (Q->front == Q->rear) return 0;
    *x = Q->data[Q->front];
    Q->front = (Q->front + 1) % MaxSize;
    return 1;
}


/* ========== 栈的经典应用 ========== */

/*
 * ★ 括号匹配算法
 *   遇到左括号入栈, 遇到右括号检查栈顶是否匹配
 *   最终栈必须为空才算匹配成功
 */
int BracketMatch(const char *str) {
    SqStack S;
    InitStack(&S);
    int i;
    char ch;

    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] == '(' || str[i] == '[' || str[i] == '{') {
            Push(&S, str[i]); /* 左括号入栈 */
        } else if (str[i] == ')' || str[i] == ']' || str[i] == '}') {
            if (StackEmpty(&S)) return 0; /* 没有左括号可配 */
            Pop(&S, (ElemType *)&ch);

            /* 检查对称性 */
            if (str[i] == ')' && ch != '(') return 0;
            if (str[i] == ']' && ch != '[') return 0;
            if (str[i] == '}' && ch != '{') return 0;
        }
    }
    return StackEmpty(&S); /* 最终栈为空才行 */
}

/*
 * ★ 十进制转二进制
 *   反复除以 2 取余入栈, 然后弹栈得到结果
 */
void DecToBin(int n) {
    SqStack S;
    InitStack(&S);
    ElemType rem;

    if (n == 0) { printf("0"); return; }
    while (n > 0) {
        Push(&S, n % 2);
        n = n / 2;
    }
    while (!StackEmpty(&S)) {
        Pop(&S, &rem);
        printf("%d", rem);
    }
}

/*
 * ★ 后缀表达式求值 (操作数为个位数, 简化演示)
 *   规则: 遇到数字入栈, 遇到运算符弹出两个操作数计算后压回
 */
int EvalPostfix(const char *expr) {
    SqStack S;
    InitStack(&S);
    int i;
    ElemType a, b, result;

    for (i = 0; expr[i] != '\0'; i++) {
        if (expr[i] >= '0' && expr[i] <= '9') {
            Push(&S, expr[i] - '0');
        } else if (expr[i] == ' ') {
            continue; /* 跳过空格 */
        } else {
            Pop(&S, &b); /* 注意: 先弹出的是右操作数 */
            Pop(&S, &a);
            switch (expr[i]) {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/': result = a / b; break;
                default: result = 0;
            }
            Push(&S, result);
        }
    }
    Pop(&S, &result);
    return result;
}


/* ========== main 演示 ========== */
int main() {
    printf("===== 408 栈与队列 可运行演示 =====\n\n");

    /* --- 顺序栈基础 --- */
    printf("--- 顺序栈 ---\n");
    SqStack S;
    InitStack(&S);
    Push(&S, 10);
    Push(&S, 20);
    Push(&S, 30);
    ElemType val;
    GetTop(&S, &val);
    printf("栈顶: %d\n", val);
    Pop(&S, &val);
    printf("弹出: %d, 新栈顶: ", val);
    GetTop(&S, &val);
    printf("%d\n\n", val);

    /* --- 循环队列基础 --- */
    printf("--- 循环队列 ---\n");
    SqQueue Q;
    InitQueue(&Q);
    EnQueue(&Q, 1);
    EnQueue(&Q, 2);
    EnQueue(&Q, 3);
    DeQueue(&Q, &val);
    printf("出队: %d\n", val);
    DeQueue(&Q, &val);
    printf("出队: %d\n\n", val);

    /* --- 括号匹配 --- */
    printf("--- 括号匹配 ---\n");
    printf("\"(a+b)*[c-d]\" 匹配? %s\n",
           BracketMatch("(a+b)*[c-d]") ? "是" : "否");
    printf("\"(a+b]*c\"     匹配? %s\n",
           BracketMatch("(a+b]*c") ? "是" : "否");
    printf("\"((())\"        匹配? %s\n\n",
           BracketMatch("((())") ? "是" : "否");

    /* --- 十进制转二进制 --- */
    printf("--- 十进制转二进制 ---\n");
    printf("42 -> ");
    DecToBin(42);
    printf("\n13 -> ");
    DecToBin(13);
    printf("\n\n");

    /* --- 后缀表达式求值 --- */
    printf("--- 后缀表达式求值 ---\n");
    /* "3 4 + 5 *" 即 (3+4)*5 = 35 */
    int res = EvalPostfix("34+5*");
    printf("后缀 \"34+5*\" = %d (即(3+4)*5)\n", res);
    /* "5 1 2 + 4 * + 3 -" 即 5+((1+2)*4)-3 = 14 */
    res = EvalPostfix("512+4*+3-");
    printf("后缀 \"512+4*+3-\" = %d (即5+(1+2)*4-3)\n", res);

    return 0;
}
