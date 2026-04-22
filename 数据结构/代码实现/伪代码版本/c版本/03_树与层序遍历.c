#include <stdio.h>
#include <stdlib.h>

/* =========================================
 * 408 统考数据结构核心代码 - 树与遍历 (纯 C 语言风格)
 * 考试默写要求：超极高！前中后序的非递归大题神级利器！
 * ========================================= */

typedef int ElemType;

// 二叉树链式存储定义
typedef struct BiTNode {
    ElemType data;
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

/* -----------------------------------------
 * 【1】必背神迹：各类递归遍历
 * ----------------------------------------- */

void visit(BiTNode *p) {
    printf("%d ", p->data);
}

// 先序遍历 (NLR)
void PreOrder(BiTree T) {
    if (T != NULL) {
        visit(T);               // 访问根结点
        PreOrder(T->lchild);    // 遍历左子树
        PreOrder(T->rchild);    // 遍历右子树
    }
}

// 中序遍历 (LNR)
void InOrder(BiTree T) {
    if (T != NULL) {
        InOrder(T->lchild);     
        visit(T);               
        InOrder(T->rchild);     
    }
}

// 后序遍历 (LRN)
void PostOrder(BiTree T) {
    if (T != NULL) {
        PostOrder(T->lchild);
        PostOrder(T->rchild);
        visit(T);
    }
}

/* -----------------------------------------
 * 【2】找双亲、找路径必备核心：DFS 加辅助栈的非递归 (中序演示)
 * 该方法属于 408 最常见的大题压轴解题利器！务必熟练。
 * ----------------------------------------- */

// 简化栈定义供演示
typedef struct {
    BiTNode* data[100];
    int top;
} BiStack;
// ... (此处省略 InitStack, Push, Pop 等基础实现)
int IsEmpty(BiStack *S) { return S->top == -1; }
void Push(BiStack *S, BiTNode* p) { S->data[++S->top] = p; }
void Pop(BiStack *S, BiTNode** p) { *p = S->data[S->top--]; }

// 非递归中序遍历
void InOrderNonRecursive(BiTree T) {
    BiStack S;
    S.top = -1;
    BiTree p = T;
    
    while (p != NULL || !IsEmpty(&S)) {
        if (p != NULL) {
            // 一头扎到最左边
            Push(&S, p);
            p = p->lchild;
        } else {
            // 左边走到头了，出栈访问，然后切入右边
            Pop(&S, &p);
            visit(p);
            p = p->rchild;
        }
    }
}

/* -----------------------------------------
 * 【3】层序遍历 (BFS) - 也是图的 BFS 与最短路的核心算法变种
 * ----------------------------------------- */
typedef struct {
    BiTNode* data[100];
    int front, rear;
} BiQueue;
// ... 假装实现了队列基本操作
void EnQueue(BiQueue* Q, BiTNode* p) { Q->data[Q->rear++] = p; }
BiTNode* DeQueue(BiQueue* Q) { return Q->data[Q->front++]; }
int QueueEmpty(BiQueue* Q) { return Q->front == Q->rear; }

void LevelOrder(BiTree T) {
    BiQueue Q;
    Q.front = Q.rear = 0;
    BiTree p;
    
    if(T != NULL) {
        EnQueue(&Q, T); // 根节点入队
    }
    
    while(!QueueEmpty(&Q)) {
        p = DeQueue(&Q);
        visit(p);
        
        if (p->lchild != NULL) {
            EnQueue(&Q, p->lchild);
        }
        if (p->rchild != NULL) {
            EnQueue(&Q, p->rchild);
        }
    }
}

/* -----------------------------------------
 * 【4】二叉树常见常考高级大题操作
 * ----------------------------------------- */

// 1. 求二叉树深度 (经典递归)
int TreeDepth(BiTree T) {
    if (T == NULL) return 0;
    
    int ldepth = TreeDepth(T->lchild);
    int rdepth = TreeDepth(T->rchild);
    
    // 树的高度 = 左右子树最大的加自己本身(1)
    return (ldepth > rdepth ? ldepth : rdepth) + 1;
}

// 2. 判别是否是平衡二叉树 (AVL 树)
// 利用后序遍历一边求深度一边判别，大幅度降级时间复杂度
int isBalanced(BiTree T, int *depth) {
    if (T == NULL) {
        *depth = 0;
        return 1; // 空树自然是平衡的
    }
    
    int ldepth, rdepth;
    // 分别检查左右孩子是否平衡
    if (isBalanced(T->lchild, &ldepth) && isBalanced(T->rchild, &rdepth)) {
        int diff = ldepth - rdepth;
        // 左右子树高低差不能超过 1
        if (diff >= -1 && diff <= 1) {
            *depth = (ldepth > rdepth ? ldepth : rdepth) + 1; // 更新给上一层
            return 1; 
        }
    }
    return 0; // 不平衡
}

/* -----------------------------------------
 * 【5】并查集 (Union-Find) 极高频考点
 * 一般用双亲表示法的结构数组来实现
 * ----------------------------------------- */

#define SIZE 100
int UFSets[SIZE]; // 并查集实质是个双亲指针数组

// 初始化并查集
void Initial(int S[]) {
    for (int i = 0; i < SIZE; i++) {
        S[i] = -1; // 初始时每个人都是自己的祖先，值为 -1
    }
}

// 并查集核心操作 1：Find 找祖宗
// （常考优化版本：路径压缩。直接顺带把沿途结点的爹全改成最大的祖宗）
int Find(int S[], int x) {
    int root = x;
    while (S[root] >= 0) { // 如果大于等于 0，说明有爹
        root = S[root];
    }
    // 路径压缩 (408重点：写出这步能极大的防丢分保满分)
    int curr = x, ptr;
    while (curr != root) {
        ptr = S[curr]; // 保存原来爹
        S[curr] = root; // 把直接爹改成祖宗
        curr = ptr;
    }
    return root; // 返回祖宗下标
}

// 并查集核心操作 2：Union 合并两个集合
// （传入的是祖宗节点，要求：小树合并入大树）
void Union(int S[], int Root1, int Root2) {
    if (Root1 == Root2) return; 
    
    // 我们约定 S[] 存的负数的绝对值为该集合拥有的结点总数
    if (S[Root2] < S[Root1]) { // Root2 人数更多 (负的更多)
        S[Root2] += S[Root1]; // 把人家吞并了人数加过来
        S[Root1] = Root2;     // Root1 的爹变成 Root2
    } else {
        S[Root1] += S[Root2];
        S[Root2] = Root1;
    }
}

