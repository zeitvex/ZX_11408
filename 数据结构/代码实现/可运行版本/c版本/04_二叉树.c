/*
 * ============================================================
 *   408 考研数据结构 —— 二叉树 (可编译运行版)
 *   文件: 04_二叉树.c
 *   编译: gcc 04_二叉树.c -o btree && btree
 * ============================================================
 *
 *   本文件包含:
 *     1. 二叉链表存储结构的定义与手工建树
 *     2. 先序/中序/后序 递归遍历
 *     3. ★ 中序非递归遍历 (栈实现, 考研大题重点)
 *     4. ★ 层序遍历 (队列实现, BFS 的原型)
 *     5. ★ 求二叉树深度 (递归)
 *     6. ★ 统计叶子结点个数
 *     7. ★ 2014 真题: 求 WPL (带权路径长度)
 *     8. ★ 判断是否是完全二叉树
 *
 *   手工建树说明:
 *     为了让你真正运行起来看到效果, 本文件手工构造一棵树:
 *          1
 *        /   \
 *       2     3
 *      / \   / \
 *     4   5 6   7
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef int ElemType;

/* ---- 二叉树链式存储定义 ---- */
typedef struct BiTNode {
    ElemType data;
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

/* 辅助: 创建新结点 */
BiTNode *NewNode(ElemType val) {
    BiTNode *p = (BiTNode *)malloc(sizeof(BiTNode));
    p->data = val;
    p->lchild = p->rchild = NULL;
    return p;
}


/* ========== 递归遍历 ========== */

void PreOrder(BiTree T) {
    if (T != NULL) {
        printf("%d ", T->data);
        PreOrder(T->lchild);
        PreOrder(T->rchild);
    }
}

void InOrder(BiTree T) {
    if (T != NULL) {
        InOrder(T->lchild);
        printf("%d ", T->data);
        InOrder(T->rchild);
    }
}

void PostOrder(BiTree T) {
    if (T != NULL) {
        PostOrder(T->lchild);
        PostOrder(T->rchild);
        printf("%d ", T->data);
    }
}


/* ========== 非递归中序遍历 (大题重点) ========== */
/*
 * 算法思路:
 *   1. 从根结点开始, 一路向左走到底, 沿途入栈
 *   2. 走到空了, 弹栈, 访问该结点
 *   3. 再转向该结点的右子树, 重复步骤 1
 *   循环条件: 当前指针不空 或 栈不空
 */
void InOrderNonRecursive(BiTree T) {
    BiTNode *stack[50];
    int top = -1;
    BiTNode *p = T;

    while (p != NULL || top != -1) {
        if (p != NULL) {
            stack[++top] = p;  /* 入栈 */
            p = p->lchild;     /* 一路向左 */
        } else {
            p = stack[top--];  /* 出栈 */
            printf("%d ", p->data); /* 访问 */
            p = p->rchild;     /* 转向右子树 */
        }
    }
}


/* ========== 层序遍历 (BFS, 队列实现) ========== */
void LevelOrder(BiTree T) {
    if (T == NULL) return;

    BiTNode *queue[50];
    int front = 0, rear = 0;

    queue[rear++] = T; /* 根入队 */

    while (front != rear) {
        BiTNode *p = queue[front++]; /* 出队 */
        printf("%d ", p->data);

        if (p->lchild != NULL) queue[rear++] = p->lchild;
        if (p->rchild != NULL) queue[rear++] = p->rchild;
    }
}


/* ========== 求二叉树深度 ========== */
int TreeDepth(BiTree T) {
    if (T == NULL) return 0;
    int ld = TreeDepth(T->lchild);
    int rd = TreeDepth(T->rchild);
    return (ld > rd ? ld : rd) + 1;
}


/* ========== 统计叶子结点个数 ========== */
int CountLeaves(BiTree T) {
    if (T == NULL) return 0;
    if (T->lchild == NULL && T->rchild == NULL)
        return 1; /* 没有孩子就是叶子 */
    return CountLeaves(T->lchild) + CountLeaves(T->rchild);
}


/* ========== 2014 真题: 求 WPL ========== */
/*
 * WPL = 所有叶子结点的 (权值 × 深度) 之和
 * 递归: 如果是叶子就返回 weight*depth, 否则递归左右子树
 * 注意: 这里 depth 从 0 开始 (根在第 0 层)
 */
int CalcWPL(BiTree T, int depth) {
    if (T == NULL) return 0;
    if (T->lchild == NULL && T->rchild == NULL)
        return T->data * depth;
    return CalcWPL(T->lchild, depth + 1) +
           CalcWPL(T->rchild, depth + 1);
}


/* ========== 判断是否为完全二叉树 ========== */
/*
 * 层序遍历,遇到空结点后不应再出现非空结点
 */
int IsComplete(BiTree T) {
    if (T == NULL) return 1;

    BiTNode *queue[50];
    int front = 0, rear = 0;
    int reachedNull = 0; /* 标记是否遇到过空 */

    queue[rear++] = T;
    while (front != rear) {
        BiTNode *p = queue[front++];

        if (p == NULL) {
            reachedNull = 1;
        } else {
            if (reachedNull) return 0; /* 空之后又遇到非空, 不是完全二叉树 */
            queue[rear++] = p->lchild; /* 即使是空也入队 */
            queue[rear++] = p->rchild;
        }
    }
    return 1;
}


/* ========== main 演示 ========== */
int main() {
    printf("===== 408 二叉树 可运行演示 =====\n\n");

    /* 手工建立一棵二叉树:
     *         1
     *       /   \
     *      2     3
     *     / \   / \
     *    4   5 6   7
     */
    BiTNode *n1 = NewNode(1);
    BiTNode *n2 = NewNode(2);
    BiTNode *n3 = NewNode(3);
    BiTNode *n4 = NewNode(4);
    BiTNode *n5 = NewNode(5);
    BiTNode *n6 = NewNode(6);
    BiTNode *n7 = NewNode(7);
    n1->lchild = n2; n1->rchild = n3;
    n2->lchild = n4; n2->rchild = n5;
    n3->lchild = n6; n3->rchild = n7;

    printf("先序遍历: "); PreOrder(n1); printf("\n");
    printf("中序遍历: "); InOrder(n1); printf("\n");
    printf("后序遍历: "); PostOrder(n1); printf("\n");

    printf("\n中序非递归: "); InOrderNonRecursive(n1); printf("\n");
    printf("层序遍历:   "); LevelOrder(n1); printf("\n");

    printf("\n树的深度 = %d\n", TreeDepth(n1));
    printf("叶子结点数 = %d\n", CountLeaves(n1));

    /* WPL: 叶子是 4(深度2), 5(深度2), 6(深度2), 7(深度2) */
    printf("\nWPL = %d (叶子权×深度之和)\n", CalcWPL(n1, 0));

    printf("是否完全二叉树? %s\n", IsComplete(n1) ? "是" : "否");

    /* 构造一棵不完全的树测试 */
    BiTNode *m1 = NewNode(1);
    BiTNode *m2 = NewNode(2);
    BiTNode *m3 = NewNode(3);
    m1->lchild = m2; m1->rchild = m3;
    m3->rchild = NewNode(4); /* 右子树有右孩子, 左子树无 -> 不完全 */
    printf("不完全二叉树判断? %s\n", IsComplete(m1) ? "是" : "否");

    return 0;
}
