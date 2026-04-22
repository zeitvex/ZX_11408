#include <iostream>
#include <queue>
using namespace std;

/* =========================================
 * 408 统考数据结构核心代码 - 图的核心算法 (C++ 伪代码)
 * =========================================
 * C++ 可以直接使用 `#include <queue>`，在考卷上手撸 BFS
 * 在代码上会远远简洁于手写数组和队列指针的 C 版本。
 */

#define MaxVertexNum 100

// 边/弧结点
typedef struct ArcNode {
    int adjvex;               // 指向那个顶点
    struct ArcNode *nextarc;  // 指向下一条弧
    // int info;              // 如果有网，带权值
} ArcNode;

// 顶点表
typedef struct VNode {
    int data;                 
    ArcNode *firstarc;        
} VNode, AdjList[MaxVertexNum];

//图的邻接表体系
typedef struct {
    AdjList vertices;         
    int vexnum, arcnum;       
} ALGraph;

/* -----------------------------------------
 * 【1】BFS 与 DFS 的基本框架
 * ----------------------------------------- */
bool visited[MaxVertexNum];

void visit(int v) {
    cout << "V: " << v << endl; // 虚拟访问
}

// ------ 深度优先 DFS ------
void DFS(ALGraph G, int v) {
    visit(v);              
    visited[v] = true;        
    
    ArcNode *p = G.vertices[v].firstarc; 
    while (p != NULL) {
        int w = p->adjvex;
        if (!visited[w]) { 
            DFS(G, w);     
        }
        p = p->nextarc;    
    }
}

void DFSTraverse(ALGraph G) {
    for (int i = 0; i < G.vexnum; i++) visited[i] = false; 
    
    for (int i = 0; i < G.vexnum; i++) {
        if (!visited[i]) DFS(G, i);
    }
}

// ------ 广度优先 BFS (直接使用 STL) ------
void BFS(ALGraph G, int v) {
    queue<int> Q; // STL 使此处的逻辑无比好写！
    visit(v);
    visited[v] = true;
    Q.push(v); 
    
    while (!Q.empty()) {
        int target = Q.front();
        Q.pop();
        
        ArcNode *p = G.vertices[target].firstarc;
        while (p != NULL) {
            int w = p->adjvex;
            if (!visited[w]) { 
                visit(w);      
                visited[w] = true;
                Q.push(w);    
            }
            p = p->nextarc;
        }
    }
}

void BFSTraverse(ALGraph G) {
    for (int i = 0; i < G.vexnum; i++) visited[i] = false; 
    for (int i = 0; i < G.vexnum; i++) {
        if (!visited[i]) BFS(G, i);
    }
}

/* -----------------------------------------
 * 【2】Dijkstra 最短路径 (伪代码框架)
 * ----------------------------------------- */
#define INF 999999
int dist[MaxVertexNum];
int path[MaxVertexNum];
bool isFinal[MaxVertexNum];

// 传二维数组的语法在 408 经常这样书写
void Dijkstra(int curr_vex, int vexnum, int Edge[][MaxVertexNum]) {
    // 1. 初始化
    for (int i = 0; i < vexnum; i++) {
        dist[i] = Edge[curr_vex][i];
        isFinal[i] = false;     
        if (dist[i] < INF) path[i] = curr_vex;
        else path[i] = -1;
    }
    
    isFinal[curr_vex] = true; 
    dist[curr_vex] = 0;
    
    // 2. 依次寻找最短接入点
    for (int i = 0; i < vexnum - 1; i++) {
        int min = INF;
        int min_id = -1;
        
        for (int j = 0; j < vexnum; j++) {
            if (!isFinal[j] && dist[j] < min) {
                min = dist[j];
                min_id = j;
            }
        }
        
        if(min_id == -1) break; 
        isFinal[min_id] = true; 
        
        // （2）借助新接入点拉踩其余未接入点的值
        for (int j = 0; j < vexnum; j++) {
            if (!isFinal[j] && dist[min_id] + Edge[min_id][j] < dist[j]) {
                dist[j] = dist[min_id] + Edge[min_id][j];
                path[j] = min_id; 
            }
        }
    }
}
