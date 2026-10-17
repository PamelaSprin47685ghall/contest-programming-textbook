#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 50000;
const int MAXM = 50000;
const int MAXLOG = 17; // 2^16 = 65536 > 50000，往上跳 2^16 层足以对齐任意两点

int n, m;

// 原树的邻接表，每条原边按输入顺序编号
int head[MAXN + 1];
int nextEdge[2 * MAXN];
int edgeTo[2 * MAXN];
int edgeNumber[2 * MAXN];
int edgeCount = 0;

int parent[MAXN + 1];
int depth[MAXN + 1];
int upEdgeId[MAXN + 1]; // 每个点连向父亲的那条原边的编号
int ancestor[MAXLOG][MAXN + 1];

// 额外道路，按长度升序使用
int extraP[MAXM], extraQ[MAXM], extraR[MAXM];
int extraOrder[MAXM];

int answer[MAXN];   // 按输入顺序存每条原边的最短替代道路，保持 -1 表示没有
int jump[MAXN + 1]; // 并查集：沿着父亲方向一个个跳过"已经定过答案"的点

void addTreeEdge(int u, int v, int id) {
    edgeTo[edgeCount] = v;
    edgeNumber[edgeCount] = id;
    nextEdge[edgeCount] = head[u];
    head[u] = edgeCount;
    edgeCount++;
}

bool byWeight(int a, int b) {
    return extraR[a] < extraR[b];
}

// 返回从 x 出发沿父亲方向能到达的、离 x 最近的"还没定答案"的点
int findJump(int x) {
    int root = x;
    while (jump[root] != root) root = jump[root];
    while (jump[x] != root) {
        int next = jump[x];
        jump[x] = root;
        x = next;
    }
    return root;
}

int lowestCommonAncestor(int u, int v) {
    if (depth[u] < depth[v]) {
        int tmp = u;
        u = v;
        v = tmp;
    }
    for (int level = MAXLOG - 1; level >= 0; --level) {
        if (depth[u] - (1 << level) >= depth[v]) u = ancestor[level][u];
    }
    if (u == v) return u;
    for (int level = MAXLOG - 1; level >= 0; --level) {
        if (ancestor[level][u] != ancestor[level][v]) {
            u = ancestor[level][u];
            v = ancestor[level][v];
        }
    }
    return parent[u];
}

int main() {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) head[i] = -1;
    for (int i = 0; i < n - 1; ++i) {
        int p, q;
        scanf("%d%d", &p, &q);
        addTreeEdge(p, q, i);
        addTreeEdge(q, p, i);
    }
    for (int i = 0; i < m; ++i) {
        scanf("%d%d%d", &extraP[i], &extraQ[i], &extraR[i]);
        extraOrder[i] = i;
    }

    // 以 1 为根做 BFS，同时把倍增祖先表填好
    int bfsOrder[MAXN + 1];
    int queueHead = 0, queueTail = 0;
    parent[1] = 1;
    depth[1] = 0;
    for (int level = 0; level < MAXLOG; ++level) ancestor[level][1] = 1;
    bfsOrder[queueTail++] = 1;
    while (queueHead < queueTail) {
        int u = bfsOrder[queueHead++];
        for (int e = head[u]; e != -1; e = nextEdge[e]) {
            int v = edgeTo[e];
            if (v == parent[u]) continue;
            parent[v] = u;
            depth[v] = depth[u] + 1;
            upEdgeId[v] = edgeNumber[e];
            ancestor[0][v] = u;
            for (int level = 1; level < MAXLOG; ++level) {
                ancestor[level][v] = ancestor[level - 1][ancestor[level - 1][v]];
            }
            bfsOrder[queueTail++] = v;
        }
    }

    for (int i = 0; i < n - 1; ++i) answer[i] = -1;
    for (int i = 1; i <= n; ++i) jump[i] = i;

    // 一条额外道路能替代原边 e，当且仅当它的两个端点分居 e 的两侧，也就是它的
    // 树上路径经过 e。把额外道路按长度升序处理，第一次覆盖到 e 的就是最短替代。
    sort(extraOrder, extraOrder + m, byWeight);
    for (int i = 0; i < m; ++i) {
        int id = extraOrder[i];
        int a = extraP[id], b = extraQ[id], r = extraR[id];
        int common = lowestCommonAncestor(a, b);
        // 从 a、b 分别往 lca 走，把路径上还没定答案的原边都定成 r。每条原边只被
        // 定一次，之后并查集让它往上跳过，所以全部道路合起来也只扫 O(n) 次。
        int x = findJump(a);
        while (depth[x] > depth[common]) {
            answer[upEdgeId[x]] = r;
            jump[x] = parent[x];
            x = findJump(x);
        }
        x = findJump(b);
        while (depth[x] > depth[common]) {
            answer[upEdgeId[x]] = r;
            jump[x] = parent[x];
            x = findJump(x);
        }
    }

    for (int i = 0; i < n - 1; ++i) printf("%d\n", answer[i]);
    return 0;
}
