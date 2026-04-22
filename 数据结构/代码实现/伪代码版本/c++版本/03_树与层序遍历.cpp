#include <iostream>
#include <stack>
#include <queue>
using namespace std;

/* =========================================
 * 408 统考数据结构核心代码 - 树与遍历 (C++ 风格)
 * 利用了 C++ 内置的 <stack> 与 <queue> STL
 * 在考场中如果题目没有明文禁止，使用 STL 能极大的缩减非递归书写的复杂度！
 * ========================================= */

typedef int ElemType;

// 二叉树链式存储定义
typedef struct BiTNode {
    ElemType data;
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

/* -----------------------------------------
 * 【1】递归遍历系列
 * ----------------------------------------- */

void visit(BiTNode *p) {
    cout << p->data << " ";
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

/* -----------------------------------------
 * 【2】借助 C++ STL stack 的非递归中序遍历
 * （在 408 算法真题中极其管用）
 * ----------------------------------------- */
void InOrderNonRecursive(BiTree T) {
    stack<BiTNode*> S;
    BiTree p = T;
    
    while (p != NULL || !S.empty()) {
        if (p != NULL) {
            S.push(p);       // 左路一条道走到黑入栈
            p = p->lchild;
        } else {
            p = S.top();     // 获取栈顶
            S.pop();         // 弹出栈顶
            visit(p);        // 访问它
            p = p->rchild;   // 开辟新航线
        }
    }
}

/* -----------------------------------------
 * 【3】借助 C++ STL queue 的层序遍历 (BFS)
 * ----------------------------------------- */
void LevelOrder(BiTree T) {
    queue<BiTNode*> Q;
    BiTree p;
    
    if(T != NULL) {
        Q.push(T); // 根节点入队
    }
    
    while(!Q.empty()) {
        p = Q.front(); // 取队头
        Q.pop();       // 出队
        visit(p);
        
        if (p->lchild != NULL) {
            Q.push(p->lchild);
        }
        if (p->rchild != NULL) {
            Q.push(p->rchild);
        }
    }
}

/* -----------------------------------------
 * 【4】求二叉树深度 (常考应用小操作)
 * ----------------------------------------- */
int TreeDepth(BiTree T) {
    if (T == NULL) return 0;
    
    int ldepth = TreeDepth(T->lchild);
    int rdepth = TreeDepth(T->rchild);
    
    // 树的高度 = 左右子树最大的加自己本身(1)
    return (ldepth > rdepth ? ldepth : rdepth) + 1;
}

// 5. 判别是否是平衡二叉树 (AVL 树)
// C++ 可以直接使用 & 引用回传深度信息，使得代码比 C 纯指针更加清晰直白
bool isBalanced(BiTree T, int &depth) {
    if (T == NULL) {
        depth = 0;
        return true; 
    }
    
    int ldepth, rdepth;
    // 分别检查左、右孩子是否平衡，同时获取它们的深度
    if (isBalanced(T->lchild, ldepth) && isBalanced(T->rchild, rdepth)) {
        int diff = ldepth - rdepth;
        if (diff >= -1 && diff <= 1) {
            depth = (ldepth > rdepth ? ldepth : rdepth) + 1; 
            return true; 
        }
    }
    return false; // 不平衡
}

/* -----------------------------------------
 * 【5】并查集 (Union-Find) 极高频考点
 * ----------------------------------------- */

#define SIZE 100
int UFSets[SIZE]; 

// 初始化
void Initial(int S[]) {
    for (int i = 0; i < SIZE; i++) S[i] = -1; 
}

// 并查集 Find：附带路径压缩
int Find(int S[], int x) {
    int root = x;
    while (S[root] >= 0) root = S[root]; // 寻根
    
    // 路径压缩：将这一路上的所有人都直接挂在最大的老祖宗名下
    int curr = x, temp;
    while (curr != root) {
        temp = S[curr]; 
        S[curr] = root; 
        curr = temp;
    }
    return root; 
}

// 并查集 Union：小集合合并到大集合
void Union(int S[], int Root1, int Root2) {
    if (Root1 == Root2) return; 
    
    // 我们约定 S[] 当它是根时，存入的是该集合拥有的结点总数（取负）
    if (S[Root2] < S[Root1]) { // Root2 人数更多 (负的更大)
        S[Root2] += S[Root1];  // 人口吸收
        S[Root1] = Root2;      // Root1 的爹变成 Root2
    } else {
        S[Root1] += S[Root2];
        S[Root2] = Root1;
    }
}

