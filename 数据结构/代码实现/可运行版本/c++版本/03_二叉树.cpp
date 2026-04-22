/*
 * ============================================================
 *   408 考研数据结构 —— 二叉树 (可运行 C++ 版)
 *   文件: 03_二叉树.cpp
 *   编译: g++ 03_二叉树.cpp -o test && ./test
 * ============================================================
 *
 *   覆盖内容:
 *     1. 二叉链表存储与手工建树
 *     2. 先序/中序/后序 递归遍历
 *     3. ★ 中序非递归遍历 (栈实现)
 *     4. ★ 层序遍历 (队列实现, BFS 原型)
 *     5. 求深度 / 统计叶子
 *     6. ★ 2014 真题: 求 WPL (带权路径长度)
 *
 *   测试树:
 *          1
 *        /   \
 *       2     3
 *      / \   / \
 *     4   5 6   7
 * ============================================================
 */

#include <cstdio>
#include <cstdlib>

typedef int ElemType;

/* ---- 二叉链表定义 ---- */
typedef struct BiTNode {
    ElemType data;
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

/* 创建新结点 */
BiTNode *NewNode(ElemType val) {
    BiTNode *p = (BiTNode *)malloc(sizeof(BiTNode));
    p->data = val;
    p->lchild = p->rchild = NULL;
    return p;
}


/* ========== 一、递归遍历 ========== */

void PreOrder(BiTree T) {
    if (T != NULL) {
        printf("%d ", T->data);     // 先访问根
        PreOrder(T->lchild);        // 再左
        PreOrder(T->rchild);        // 再右
    }
}

void InOrder(BiTree T) {
    if (T != NULL) {
        InOrder(T->lchild);         // 先左
        printf("%d ", T->data);     // 再根
        InOrder(T->rchild);         // 再右
    }
}

void PostOrder(BiTree T) {
    if (T != NULL) {
        PostOrder(T->lchild);       // 先左
        PostOrder(T->rchild);       // 再右
        printf("%d ", T->data);     // 最后根
    }
}


/* ========== 二、中序非递归 (栈实现, 考研大题重点) ========== */
/*
 * 思路:
 *   1. 从当前结点一路向左走到底, 沿途入栈
 *   2. 走到空了, 弹栈访问
 *   3. 转向右子树, 重复步骤 1
 */
void InOrderNonRecursive(BiTree T) {
    BiTNode *stack[50];
    int top = -1;
    BiTNode *p = T;

    while (p != NULL || top != -1) {
        if (p != NULL) {
            stack[++top] = p;       // 入栈
            p = p->lchild;         // 一路向左
        } else {
            p = stack[top--];      // 弹栈
            printf("%d ", p->data); // 访问
            p = p->rchild;         // 转向右子树
        }
    }
}


/* ========== 三、层序遍历 (队列实现) ========== */
void LevelOrder(BiTree T) {
    if (T == NULL) return;

    BiTNode *queue[50];
    int front = 0, rear = 0;

    queue[rear++] = T;  // 根入队

    while (front != rear) {
        BiTNode *p = queue[front++];    // 出队
        printf("%d ", p->data);          // 访问

        if (p->lchild != NULL)
            queue[rear++] = p->lchild;   // 左孩子入队
        if (p->rchild != NULL)
            queue[rear++] = p->rchild;   // 右孩子入队
    }
}


/* ========== 四、常用操作 ========== */

/* 求深度: max(左深, 右深) + 1 */
int TreeDepth(BiTree T) {
    if (T == NULL) return 0;
    int ld = TreeDepth(T->lchild);
    int rd = TreeDepth(T->rchild);
    return (ld > rd ? ld : rd) + 1;
}

/* 统计叶子: 左右孩子都空就是叶子 */
int CountLeaves(BiTree T) {
    if (T == NULL) return 0;
    if (T->lchild == NULL && T->rchild == NULL)
        return 1;
    return CountLeaves(T->lchild) + CountLeaves(T->rchild);
}


/* ========== 五、2014 真题: 求 WPL ========== */
/*
 * WPL = 所有叶子的 (权值 × 深度) 之和
 * 叶子结点: 返回 weight * depth
 * 非叶子: 递归左右求和
 */
int CalcWPL(BiTree T, int depth) {
    if (T == NULL) return 0;
    if (T->lchild == NULL && T->rchild == NULL)
        return T->data * depth;       // 叶子: 权值 × 深度
    return CalcWPL(T->lchild, depth + 1)
         + CalcWPL(T->rchild, depth + 1);
}


/* ========== main 演示 ========== */
int main() {
    printf("===== 二叉树 C++ 可运行版 =====\n\n");

    /* 手工建树 */
    BiTNode *n1 = NewNode(1), *n2 = NewNode(2), *n3 = NewNode(3);
    BiTNode *n4 = NewNode(4), *n5 = NewNode(5);
    BiTNode *n6 = NewNode(6), *n7 = NewNode(7);
    n1->lchild = n2;  n1->rchild = n3;
    n2->lchild = n4;  n2->rchild = n5;
    n3->lchild = n6;  n3->rchild = n7;

    printf("先序: ");       PreOrder(n1);            printf("\n");
    printf("中序: ");       InOrder(n1);             printf("\n");
    printf("后序: ");       PostOrder(n1);           printf("\n");
    printf("中序非递归: "); InOrderNonRecursive(n1); printf("\n");
    printf("层序: ");       LevelOrder(n1);          printf("\n");

    printf("\n深度 = %d\n", TreeDepth(n1));
    printf("叶子数 = %d\n", CountLeaves(n1));
    printf("WPL = %d\n", CalcWPL(n1, 0));

    return 0;
}
