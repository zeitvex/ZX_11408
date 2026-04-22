#include <iostream>
using namespace std;

/* =========================================
 * 408 统考数据结构核心代码 - 栈与队列 (C++ 引用风格)
 * ========================================= */

#define MaxSize 50
typedef int ElemType;

// 1. 顺序栈定义
typedef struct {
    ElemType data[MaxSize];
    int top; // 栈顶指针，初始为 -1
} SqStack;

// 2. 循环队列定义
typedef struct {
    ElemType data[MaxSize];
    int front, rear; // front 队头，rear 队尾
} SqQueue;


/* -----------------------------------------
 * 【1】顺序栈核心操作
 * ----------------------------------------- */

// 初始化
void InitStack(SqStack &S) {
    S.top = -1; // 纯面向对象引用，没有箭头 ->
}

// 判空
bool StackEmpty(SqStack S) {
    return S.top == -1;
}

// 进栈法
bool Push(SqStack &S, ElemType x) {
    if (S.top == MaxSize - 1) return false; // 栈满报错
    
    S.top = S.top + 1; // 先加指针
    S.data[S.top] = x; // 再入元素
    return true;
}

// 出栈法
// 传引用 &x，直接带回出栈的值
bool Pop(SqStack &S, ElemType &x) {
    if (S.top == -1) return false; // 栈空报错
    
    x = S.data[S.top]; // 先取元素
    S.top = S.top - 1; // 再减指针
    return true;
}


/* -----------------------------------------
 * 【2】循环队列核心操作 (牺牲一个单元法)
 * ----------------------------------------- */

// 初始化队列
void InitQueue(SqQueue &Q) {
    Q.front = Q.rear = 0;
}

// 判空 (首尾相遇)
bool QueueEmpty(SqQueue Q) {
    return Q.front == Q.rear;
}

// 入队
bool EnQueue(SqQueue &Q, ElemType x) {
    // 【考点】判断队满条件：尾指针的下一个位置是头指针时，就是满了
    if ((Q.rear + 1) % MaxSize == Q.front) return false;
    
    Q.data[Q.rear] = x; // 放入队列
    Q.rear = (Q.rear + 1) % MaxSize; // 队尾指针加一取模
    return true;
}

// 出队
bool DeQueue(SqQueue &Q, ElemType &x) {
    if (Q.front == Q.rear) return false; // 队空报错
    
    x = Q.data[Q.front]; // 先取出当前队头元素
    Q.front = (Q.front + 1) % MaxSize; // 队头指针加一取模
    return true;
}
