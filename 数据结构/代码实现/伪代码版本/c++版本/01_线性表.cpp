#include <iostream>
using namespace std;

/* =========================================
 * 408 统考数据结构核心代码 - 线性表 (C++ 伪代码及引用风格)
 * =========================================
 * 说明：408 官方教材（如王道、严蔚敏版）极爱使用 C++ 的引用 `&` 传参，
 * 这也是统考卷上最主流的写法，免去了冗余繁杂的指针二级解引。
 * 建议在考研试卷上使用本写法规避由于星号和地址符混乱丢分。
 */

#define MaxSize 50
typedef int ElemType;

// 1. 顺序表结构体定义
typedef struct {
    ElemType data[MaxSize];
    int length;
} SqList;

// 2. 单链表结构体定义
typedef struct LNode {
    ElemType data;
    struct LNode *next;
} LNode, *LinkList;

/* -----------------------------------------
 * 【1】顺序表核心操作
 * ----------------------------------------- */

// 【重点】使用了 `&` 直接操作实体对象本身，无需使用指针箭头 `->`
void InitList(SqList &L) {
    L.length = 0;
}

// 顺序表插入
bool ListInsert(SqList &L, int i, ElemType e) {
    if (i < 1 || i > L.length + 1) return false; // 判断 i 的范围是否有效
    if (L.length >= MaxSize) return false;       // 存满了不能插
    
    for (int j = L.length; j >= i; j--) {
        L.data[j] = L.data[j - 1];               // 将第 i 个元素及之后的元素后移
    }
    L.data[i - 1] = e;                           // 在位置 i 放上 e
    L.length++;
    return true;
}

// 顺序表删除
// 引用传入 e 直接带回结果
bool ListDelete(SqList &L, int i, ElemType &e) {
    if (i < 1 || i > L.length) return false;     
    e = L.data[i - 1];                           
    for (int j = i; j < L.length; j++) {
        L.data[j - 1] = L.data[j];               
    }
    L.length--;
    return true;
}

/* -----------------------------------------
 * 【2】单链表核心操作
 * ----------------------------------------- */

// 头插法建立单链表（C++ `new` 关键字操作更清爽）
LinkList List_HeadInsert(LinkList &L) {
    LNode *s;
    int x;
    L = new LNode;       // 用 new 代替繁杂的 malloc
    L->next = NULL;
    
    cin >> x;
    while (x != 9999) { // 9999 为结束标志
        s = new LNode;
        s->data = x;
        
        // 核心两步
        s->next = L->next; 
        L->next = s;
        
        cin >> x;
    }
    return L;
}

// 找寻指定位置的结点
LNode* GetElem(LinkList L, int i) {
    if (i < 0) return NULL;
    if (i == 0) return L; // 返回头结点
    
    int j = 1;
    LNode *p = L->next;   // 从第一号存真实元素的结点起步
    
    while (p != NULL && j < i) {
        p = p->next;
        j++;
    }
    return p; // 到达第 i 个或者到底为空
}

// 逆置单链表（常考重点大题子操作！）
// 使用指针直接“原地逆置”法
void ReverseList(LinkList &L) {
    if (L->next == NULL || L->next->next == NULL) return; 
    
    LNode *p = L->next;       // 工作指针
    LNode *r = p->next;       // 防止断链丢掉后面的队伍
    L->next = NULL;           // 先将头结点摘下
    
    while (p != NULL) {
        r = p->next;          // 保留后路
        p->next = L->next;    // 进行头插
        L->next = p;
        p = r;                // 回到后路继续冲
    }
}
