#include <stdio.h>
#include <stdlib.h>

/* =========================================
 * 408 统考数据结构核心代码 - 图的核心算法 (C 版本)
 * 考研热点：基于邻接表的极简 BFS/DFS 搜索！
 * ========================================= */

#define MaxVertexNum 100

// 边/弧结点
typedef struct ArcNode {
    int adjvex;               // 指向那个顶点
    struct ArcNode *nextarc;  // 指向下一条弧
    // int info;              // 如果有网，这里可以带权值
} ArcNode;

// 顶点结构体
typedef struct VNode {
    int data;                 // 顶点信息
    ArcNode *firstarc;        // 指向第一个依附该顶点的弧
} VNode, AdjList[MaxVertexNum];

//图的邻接表体系
typedef struct {
    AdjList vertices;         // 存放全部顶点的表头
    int vexnum, arcnum;       // 目前的顶点数和弧数
} ALGraph;

/* -----------------------------------------
 * 【1】BFS 与 DFS 的基本框架 (常考大题防丢分模板)
 * ----------------------------------------- */
int visited[MaxVertexNum];

// 访问函数
void visit(int v) {
    printf("访问到了节点 %d\n", v);
}

// ------ 深度优先 DFS ------
void DFS(ALGraph *G, int v) {
    visit(v);              // 1. 访问它
    visited[v] = 1;        // 2. 打点标记
    
    ArcNode *p = G->vertices[v].firstarc; // 拿邻居
    while (p != NULL) {
        int w = p->adjvex;
        if (!visited[w]) { // 如果该邻居没去过
            DFS(G, w);     // 递归往里扎
        }
        p = p->nextarc;    // 退出来后找下一个兄弟遍历
    }
}

// 处理非连通图的全盘唤醒调度器
void DFSTraverse(ALGraph *G) {
    for (int i = 0; i < G->vexnum; i++) visited[i] = 0; // 清场
    
    for (int i = 0; i < G->vexnum; i++) {
        if (!visited[i]) {
            DFS(G, i);
        }
    }
}

// ------ 广度优先 BFS (借助外部队列) ------
// 伪代码展示队列行为，由于是 C 所以手撸一根简易队伍
int queue[100];
int front = 0, rear = 0;
void EnQueue(int x) { queue[rear++] = x; }
int DeQueue() { return queue[front++]; }
int QueueEmpty() { return front == rear; }

void BFS(ALGraph *G, int v) {
    visit(v);
    visited[v] = 1;
    EnQueue(v); // 入队
    
    while (!QueueEmpty()) {
        int target = DeQueue();
        ArcNode *p = G->vertices[target].firstarc;
        
        while (p != NULL) {
            int w = p->adjvex;
            if (!visited[w]) { // 如果没走过
                visit(w);      // 访问
                visited[w] = 1;// 标记
                EnQueue(w);    // 扔进队伍做后续扩展
            }
            p = p->nextarc;
        }
    }
}

void BFSTraverse(ALGraph *G) {
    for (int i = 0; i < G->vexnum; i++) visited[i] = 0; 
    front = rear = 0; // 重置队伍
    
    for (int i = 0; i < G->vexnum; i++) {
        if (!visited[i]) BFS(G, i);
    }
}

/* -----------------------------------------
 * 【2】Dijkstra 最短路径 (伪代码框架)
 * 考研大题常见变式：在邻接矩阵图上寻求源点到各点最低票价
 * ----------------------------------------- */
#define INF 999999
int dist[MaxVertexNum];
int path[MaxVertexNum];
int final[MaxVertexNum];

// G.Edge 即为邻接矩阵的距阵二阶表
void Dijkstra(int curr_vex, int vexnum, int Edge[][MaxVertexNum]) {
    // 1. 初始化
    for (int i = 0; i < vexnum; i++) {
        dist[i] = Edge[curr_vex][i];
        final[i] = 0;     // 初始全在 P-S 集合
        if (dist[i] < INF) {
            path[i] = curr_vex;
        } else {
            path[i] = -1;
        }
    }
    
    final[curr_vex] = 1; 
    dist[curr_vex] = 0;
    
    // 2. 将剩下的所有点归位
    for (int i = 0; i < vexnum - 1; i++) {
        int min = INF;
        int min_id = -1;
        
        // （1）找 S 外缘跟 S 结合最紧密的那个小弟
        for (int j = 0; j < vexnum; j++) {
            if (final[j] == 0 && dist[j] < min) {
                min = dist[j];
                min_id = j;
            }
        }
        
        if(min_id == -1) break; // 已经找无可找了
        
        final[min_id] = 1; // 开光加入神坛集合 S
        
        // （2）借助由于此小弟加入导致的新路，将原来其他非神坛的人距离拉踩刷新
        for (int j = 0; j < vexnum; j++) {
            if (final[j] == 0 && dist[min_id] + Edge[min_id][j] < dist[j]) {
                dist[j] = dist[min_id] + Edge[min_id][j];
                path[j] = min_id; // 修改他通往神坛的接入点
            }
        }
    }
}
