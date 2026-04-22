/*
 * ============================================================
 *   408 考研数据结构 —— 栈与队列 (可运行 C++ 版)
 *   文件: 02_栈与队列.cpp
 *   编译: g++ 02_栈与队列.cpp -o test && ./test
 * ============================================================
 *
 *   覆盖内容:
 *     1. 顺序栈: 初始化/进栈/出栈/判空
 *     2. 循环队列 (牺牲一个单元法)
 *     3. ★ 括号匹配算法
 *     4. ★ 后缀表达式求值
 * ============================================================
 */

#include <cstdio>

#define MaxSize 50
typedef int ElemType;


/* ========== 一、顺序栈 ========== */

typedef struct {
    ElemType data[MaxSize];
    int top;   // 栈顶指针, 初始 -1
} SqStack;

void InitStack(SqStack &S) {
    S.top = -1;
}

bool StackEmpty(SqStack S) {
    return S.top == -1;
}

/* 进栈: 先移指针再存数据 */
bool Push(SqStack &S, ElemType x) {
    if (S.top == MaxSize - 1)
        return false;          // 栈满
    S.data[++S.top] = x;
    return true;
}

/* 出栈: 先取数据再移指针 */
bool Pop(SqStack &S, ElemType &x) {
    if (S.top == -1)
        return false;          // 栈空
    x = S.data[S.top--];
    return true;
}


/* ========== 二、循环队列 (牺牲一个单元法) ========== */

typedef struct {
    ElemType data[MaxSize];
    int front, rear;
} SqQueue;

void InitQueue(SqQueue &Q) {
    Q.front = Q.rear = 0;
}

bool QueueEmpty(SqQueue Q) {
    return Q.front == Q.rear;
}

/* 入队: rear 的下一个是 front 就满了 */
bool EnQueue(SqQueue &Q, ElemType x) {
    if ((Q.rear + 1) % MaxSize == Q.front)
        return false;  // 队满
    Q.data[Q.rear] = x;
    Q.rear = (Q.rear + 1) % MaxSize;
    return true;
}

/* 出队: 从 front 端取 */
bool DeQueue(SqQueue &Q, ElemType &x) {
    if (Q.front == Q.rear)
        return false;  // 队空
    x = Q.data[Q.front];
    Q.front = (Q.front + 1) % MaxSize;
    return true;
}


/* ========== 三、栈的应用 ========== */

/*
 * ★ 括号匹配
 *   左括号入栈, 右括号检查栈顶是否对称
 *   最终栈必须为空
 */
bool BracketMatch(const char *str) {
    SqStack S;
    InitStack(S);

    for (int i = 0; str[i] != '\0'; i++) {
        char ch = str[i];

        // 遇到左括号: 入栈
        if (ch == '(' || ch == '[' || ch == '{') {
            Push(S, ch);
        }
        // 遇到右括号: 检查配对
        else if (ch == ')' || ch == ']' || ch == '}') {
            if (StackEmpty(S))
                return false;       // 没有左括号可配

            ElemType top;
            Pop(S, top);

            if (ch == ')' && top != '(') return false;
            if (ch == ']' && top != '[') return false;
            if (ch == '}' && top != '{') return false;
        }
    }
    return StackEmpty(S);  // 栈空则全部匹配
}

/*
 * ★ 后缀表达式求值 (操作数为个位数, 简化演示)
 *   规则: 数字入栈, 遇运算符弹两个计算后压回
 *   注意: 先弹出的是右操作数!
 */
int EvalPostfix(const char *expr) {
    SqStack S;
    InitStack(S);
    ElemType a, b, result;

    for (int i = 0; expr[i] != '\0'; i++) {
        if (expr[i] >= '0' && expr[i] <= '9') {
            Push(S, expr[i] - '0');   // 数字字符转整数入栈
        } else if (expr[i] == ' ') {
            continue;                  // 跳过空格
        } else {
            Pop(S, b);   // 先弹出的是右操作数
            Pop(S, a);   // 再弹出左操作数

            switch (expr[i]) {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/': result = a / b; break;
                default:  result = 0;
            }
            Push(S, result);
        }
    }
    Pop(S, result);
    return result;
}


/* ========== main 演示 ========== */
int main() {
    printf("===== 栈与队列 C++ 可运行版 =====\n\n");

    /* --- 顺序栈基础 --- */
    printf("--- 顺序栈 ---\n");
    SqStack S;
    InitStack(S);
    Push(S, 10);
    Push(S, 20);
    Push(S, 30);
    ElemType val;
    Pop(S, val);
    printf("弹出: %d\n", val);   // 30

    /* --- 循环队列 --- */
    printf("\n--- 循环队列 ---\n");
    SqQueue Q;
    InitQueue(Q);
    EnQueue(Q, 1);
    EnQueue(Q, 2);
    EnQueue(Q, 3);
    DeQueue(Q, val);
    printf("出队: %d\n", val);   // 1

    /* --- 括号匹配 --- */
    printf("\n--- 括号匹配 ---\n");
    printf("\"(a+b)*[c-d]\" 匹配? %s\n",
           BracketMatch("(a+b)*[c-d]") ? "是" : "否");
    printf("\"(a+b]*c\"     匹配? %s\n",
           BracketMatch("(a+b]*c") ? "是" : "否");

    /* --- 后缀表达式 --- */
    printf("\n--- 后缀表达式求值 ---\n");
    printf("\"34+5*\" = %d (即(3+4)*5)\n", EvalPostfix("34+5*"));
    printf("\"512+4*+3-\" = %d (即5+(1+2)*4-3)\n", EvalPostfix("512+4*+3-"));

    return 0;
}
