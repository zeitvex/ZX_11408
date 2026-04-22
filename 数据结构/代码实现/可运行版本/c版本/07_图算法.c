/*
 * ============================================================
 *   408 考研数据结构 —— 图: BFS / DFS / Dijkstra (可编译运行版)
 *   文件: 07_图算法.c
 *   编译: gcc 07_图算法.c -o graph && graph
 * ============================================================
 *
 *   本文件包含:
 *     1. 邻接矩阵存储图
 *     2. ★ DFS 深度优先遍历 (递归)
 *     3. ★ BFS 广度优先遍历 (队列)
 *     4. ★ Dijkstra 最短路径 (邻接矩阵)
 *     5. ★ 拓扑排序 (邻接表, 选择题/大题常考)
 *
 *   测试图 (无向图, 5 个顶点):
 *       0 --- 1 --- 3
 *       |     |
 *       2 --- 4
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MaxV 20
#define INF 999999

/* ========== 邻接矩阵存储 ========== */
typedef struct {
    int vex[MaxV];              /* 顶点表 */
    int Edge[MaxV][MaxV];       /* 邻接矩阵 */
    int vexnum, arcnum;         /* 顶点数, 边数 */
} MGraph;

int visited[MaxV]; /* 访问标记数组 */

/* 初始化图 */
void InitGraph(MGraph *G, int n) {
    int i, j;
    G->vexnum = n;
    G->arcnum = 0;
    for (i = 0; i < n; i++) {
        G->vex[i] = i;
        for (j = 0; j < n; j++)
            G->Edge[i][j] = (i == j) ? 0 : INF;
    }
}

/* 添加无向边 (权值默认为 1) */
void AddEdge(MGraph *G, int u, int v, int w) {
    G->Edge[u][v] = w;
    G->Edge[v][u] = w;
    G->arcnum++;
}


/* ========== DFS 深度优先遍历 ========== */
/*
 * 递归原理:
 *   1. 访问当前顶点, 标记
 *   2. 遍历邻接矩阵找未访问的邻居, 递归进去
 */
void DFS(MGraph *G, int v) {
    printf("%d ", v);
    visited[v] = 1;

    int w;
    for (w = 0; w < G->vexnum; w++) {
        if (G->Edge[v][w] != 0 && G->Edge[v][w] != INF && !visited[w])
            DFS(G, w);
    }
}

void DFSTraverse(MGraph *G) {
    int i;
    for (i = 0; i < G->vexnum; i++) visited[i] = 0;
    for (i = 0; i < G->vexnum; i++)
        if (!visited[i]) DFS(G, i);
}


/* ========== BFS 广度优先遍历 ========== */
/*
 * 队列实现:
 *   1. 源点入队, 标记
 *   2. 出队, 遍历邻居, 未访问的入队并标记
 */
void BFS(MGraph *G, int v) {
    int queue[MaxV], front = 0, rear = 0;

    printf("%d ", v);
    visited[v] = 1;
    queue[rear++] = v;

    while (front != rear) {
        int u = queue[front++];
        int w;
        for (w = 0; w < G->vexnum; w++) {
            if (G->Edge[u][w] != 0 && G->Edge[u][w] != INF && !visited[w]) {
                printf("%d ", w);
                visited[w] = 1;
                queue[rear++] = w;
            }
        }
    }
}

void BFSTraverse(MGraph *G) {
    int i;
    for (i = 0; i < G->vexnum; i++) visited[i] = 0;
    for (i = 0; i < G->vexnum; i++)
        if (!visited[i]) BFS(G, i);
}


/* ========== Dijkstra 最短路径 ========== */
/*
 * 从源点 src 到其余各点的最短路径
 * dist[i]: 源点到 i 的当前最短距离
 * path[i]: i 的前驱顶点 (用于输出路径)
 * final[i]: 顶点 i 是否已经确定最短路
 */
void Dijkstra(MGraph *G, int src) {
    int dist[MaxV], path[MaxV], final[MaxV];
    int i, j;

    /* 初始化 */
    for (i = 0; i < G->vexnum; i++) {
        dist[i] = G->Edge[src][i];
        final[i] = 0;
        path[i] = (dist[i] < INF) ? src : -1;
    }
    dist[src] = 0;
    final[src] = 1;

    /* 循环 n-1 次, 每次确定一个最短路顶点 */
    for (i = 0; i < G->vexnum - 1; i++) {
        int min = INF, u = -1;

        /* 找未确定的最短距离顶点 */
        for (j = 0; j < G->vexnum; j++) {
            if (!final[j] && dist[j] < min) {
                min = dist[j];
                u = j;
            }
        }
        if (u == -1) break; /* 不连通 */
        final[u] = 1;

        /* 用 u 更新其邻居 (松弛操作) */
        for (j = 0; j < G->vexnum; j++) {
            if (!final[j] && G->Edge[u][j] < INF &&
                dist[u] + G->Edge[u][j] < dist[j]) {
                dist[j] = dist[u] + G->Edge[u][j];
                path[j] = u;
            }
        }
    }

    /* 输出结果 */
    printf("Dijkstra (源点 %d):\n", src);
    for (i = 0; i < G->vexnum; i++) {
        if (dist[i] >= INF)
            printf("  到 %d: 不可达\n", i);
        else
            printf("  到 %d: 最短距离 = %d, 前驱 = %d\n", i, dist[i], path[i]);
    }
}


/* ========== 拓扑排序 (邻接矩阵简化版) ========== */
/*
 * 算法:
 *   1. 先统计每个顶点的入度
 *   2. 入度为 0 的入栈(或队)
 *   3. 弹出一个, 输出, 将其所有邻居的入度 -1
 *      如果新的入度为 0 则入栈
 *   4. 若输出顶点数 < 总数, 则有环
 */
int TopologicalSort(MGraph *G) {
    int indegree[MaxV] = {0};
    int stack[MaxV], top = -1;
    int i, j, count = 0;

    /* 统计入度 (邻接矩阵: Edge[i][j]!=0 且 !=INF 说明有边 i->j) */
    for (j = 0; j < G->vexnum; j++)
        for (i = 0; i < G->vexnum; i++)
            if (G->Edge[i][j] != 0 && G->Edge[i][j] != INF)
                indegree[j]++;

    /* 入度为 0 的入栈 */
    for (i = 0; i < G->vexnum; i++)
        if (indegree[i] == 0) stack[++top] = i;

    printf("拓扑序列: ");
    while (top != -1) {
        int v = stack[top--]; /* 出栈 */
        printf("%d ", v);
        count++;

        for (j = 0; j < G->vexnum; j++) {
            if (G->Edge[v][j] != 0 && G->Edge[v][j] != INF) {
                indegree[j]--;
                if (indegree[j] == 0)
                    stack[++top] = j;
            }
        }
    }
    printf("\n");

    if (count < G->vexnum) {
        printf("存在环, 拓扑排序失败!\n");
        return 0;
    }
    return 1;
}


/* ========== main 演示 ========== */
int main() {
    printf("===== 408 图算法 可运行演示 =====\n\n");

    /* 构建无向图:
     *   0 --- 1 --- 3
     *   |     |
     *   2 --- 4
     */
    MGraph G;
    InitGraph(&G, 5);
    AddEdge(&G, 0, 1, 1);
    AddEdge(&G, 0, 2, 1);
    AddEdge(&G, 1, 3, 1);
    AddEdge(&G, 1, 4, 1);
    AddEdge(&G, 2, 4, 1);

    printf("--- DFS ---\n");
    DFSTraverse(&G);
    printf("\n\n");

    printf("--- BFS ---\n");
    BFSTraverse(&G);
    printf("\n\n");

    /* 构建带权有向图测试 Dijkstra:
     *   0 --(1)--> 1 --(3)--> 3
     *   |          |
     *  (6)        (2)
     *   v          v
     *   2 --(1)--> 4
     */
    printf("--- Dijkstra ---\n");
    MGraph G2;
    InitGraph(&G2, 5);
    G2.Edge[0][1] = 1; G2.Edge[0][2] = 6;
    G2.Edge[1][3] = 3; G2.Edge[1][4] = 2;
    G2.Edge[2][4] = 1;
    Dijkstra(&G2, 0);
    printf("\n");

    /* 拓扑排序 (有向无环图):
     *   0 --> 1 --> 3
     *   |     |
     *   v     v
     *   2 --> 4
     */
    printf("--- 拓扑排序 ---\n");
    TopologicalSort(&G2);

    return 0;
}
