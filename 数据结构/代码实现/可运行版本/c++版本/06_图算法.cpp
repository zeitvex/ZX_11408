/*
 * ============================================================
 *   408 考研数据结构 —— 图算法 (可运行 C++ 版)
 *   文件: 06_图算法.cpp
 *   编译: g++ 06_图算法.cpp -o test && ./test
 * ============================================================
 *
 *   覆盖内容:
 *     1. 邻接矩阵存储图
 *     2. ★ DFS 深度优先遍历 (递归)
 *     3. ★ BFS 广度优先遍历 (队列)
 *     4. ★ Dijkstra 最短路径
 *     5. ★ 拓扑排序
 * ============================================================
 */

#include <cstdio>
#include <cstdlib>

#define MaxV 20
#define INF 999999

/* ========== 邻接矩阵存储 ========== */
typedef struct {
    int vex[MaxV];
    int Edge[MaxV][MaxV];
    int vexnum, arcnum;
} MGraph;

bool visited[MaxV];   // 访问标记数组

/* 初始化图 */
void InitGraph(MGraph &G, int n) {
    G.vexnum = n;
    G.arcnum = 0;
    for (int i = 0; i < n; i++) {
        G.vex[i] = i;
        for (int j = 0; j < n; j++)
            G.Edge[i][j] = (i == j) ? 0 : INF;
    }
}

/* 添加无向边 */
void AddEdge(MGraph &G, int u, int v, int w) {
    G.Edge[u][v] = w;
    G.Edge[v][u] = w;
    G.arcnum++;
}


/* ========== DFS 深度优先遍历 ========== */
/*
 * 递归: 访问 v, 标记, 遍历邻居递归
 */
void DFS(MGraph &G, int v) {
    printf("%d ", v);
    visited[v] = true;

    for (int w = 0; w < G.vexnum; w++) {
        if (G.Edge[v][w] != 0 && G.Edge[v][w] != INF && !visited[w])
            DFS(G, w);
    }
}

void DFSTraverse(MGraph &G) {
    for (int i = 0; i < G.vexnum; i++)
        visited[i] = false;
    for (int i = 0; i < G.vexnum; i++)
        if (!visited[i]) DFS(G, i);
}


/* ========== BFS 广度优先遍历 ========== */
/*
 * 队列: 源点入队标记, 出队遍历邻居, 未访问的入队
 */
void BFS(MGraph &G, int v) {
    int queue[MaxV];
    int front = 0, rear = 0;

    printf("%d ", v);
    visited[v] = true;
    queue[rear++] = v;

    while (front != rear) {
        int u = queue[front++];   // 出队

        for (int w = 0; w < G.vexnum; w++) {
            if (G.Edge[u][w] != 0 && G.Edge[u][w] != INF && !visited[w]) {
                printf("%d ", w);
                visited[w] = true;
                queue[rear++] = w;
            }
        }
    }
}

void BFSTraverse(MGraph &G) {
    for (int i = 0; i < G.vexnum; i++)
        visited[i] = false;
    for (int i = 0; i < G.vexnum; i++)
        if (!visited[i]) BFS(G, i);
}


/* ========== Dijkstra 最短路径 ========== */
/*
 * 从源点 src 到其余各点的最短距离
 * dist[i]: 源到 i 的当前最短距离
 * path[i]: i 的前驱 (用于输出路径)
 * fin[i]:  i 是否已确定最短路
 */
void Dijkstra(MGraph &G, int src) {
    int dist[MaxV], path[MaxV];
    bool fin[MaxV];

    // 初始化
    for (int i = 0; i < G.vexnum; i++) {
        dist[i] = G.Edge[src][i];
        fin[i] = false;
        path[i] = (dist[i] < INF) ? src : -1;
    }
    dist[src] = 0;
    fin[src] = true;

    // 循环 n-1 次
    for (int i = 0; i < G.vexnum - 1; i++) {
        // 找未确定的最近顶点
        int minDist = INF, u = -1;
        for (int j = 0; j < G.vexnum; j++) {
            if (!fin[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }
        if (u == -1) break;   // 不连通
        fin[u] = true;

        // 松弛操作: 用 u 更新其邻居
        for (int j = 0; j < G.vexnum; j++) {
            if (!fin[j] && G.Edge[u][j] < INF
                && dist[u] + G.Edge[u][j] < dist[j]) {
                dist[j] = dist[u] + G.Edge[u][j];
                path[j] = u;
            }
        }
    }

    // 输出结果
    printf("Dijkstra (源点 %d):\n", src);
    for (int i = 0; i < G.vexnum; i++) {
        if (dist[i] >= INF)
            printf("  到 %d: 不可达\n", i);
        else
            printf("  到 %d: 距离 = %d, 前驱 = %d\n", i, dist[i], path[i]);
    }
}


/* ========== 拓扑排序 ========== */
/*
 * 1. 统计入度
 * 2. 入度为 0 的入栈
 * 3. 弹出→输出→邻居入度-1→新入度0则入栈
 * 4. 若输出数 < 顶点数, 有环
 */
int TopologicalSort(MGraph &G) {
    int indegree[MaxV] = {0};
    int stack[MaxV], top = -1;
    int count = 0;

    // 统计入度
    for (int j = 0; j < G.vexnum; j++)
        for (int i = 0; i < G.vexnum; i++)
            if (G.Edge[i][j] != 0 && G.Edge[i][j] != INF)
                indegree[j]++;

    // 入度为 0 的入栈
    for (int i = 0; i < G.vexnum; i++)
        if (indegree[i] == 0)
            stack[++top] = i;

    printf("拓扑序列: ");
    while (top != -1) {
        int v = stack[top--];   // 出栈
        printf("%d ", v);
        count++;

        for (int j = 0; j < G.vexnum; j++) {
            if (G.Edge[v][j] != 0 && G.Edge[v][j] != INF) {
                indegree[j]--;
                if (indegree[j] == 0)
                    stack[++top] = j;
            }
        }
    }
    printf("\n");

    if (count < G.vexnum) {
        printf("存在环, 拓扑排序失败!\n");
        return 0;
    }
    return 1;
}


/* ========== main 演示 ========== */
int main() {
    printf("===== 图算法 C++ 可运行版 =====\n\n");

    /* 无向图:
     *   0---1---3
     *   |   |
     *   2---4
     */
    MGraph G;
    InitGraph(G, 5);
    AddEdge(G, 0, 1, 1);
    AddEdge(G, 0, 2, 1);
    AddEdge(G, 1, 3, 1);
    AddEdge(G, 1, 4, 1);
    AddEdge(G, 2, 4, 1);

    printf("DFS: ");
    DFSTraverse(G);
    printf("\n");
    printf("BFS: ");
    BFSTraverse(G);
    printf("\n\n");

    /* 带权有向图:
     *   0--(1)-->1--(3)-->3
     *   |        |
     *  (6)      (2)
     *   v        v
     *   2--(1)-->4
     */
    MGraph G2;
    InitGraph(G2, 5);
    G2.Edge[0][1] = 1;
    G2.Edge[0][2] = 6;
    G2.Edge[1][3] = 3;
    G2.Edge[1][4] = 2;
    G2.Edge[2][4] = 1;

    Dijkstra(G2, 0);
    printf("\n");

    printf("--- 拓扑排序 ---\n");
    TopologicalSort(G2);

    return 0;
}
