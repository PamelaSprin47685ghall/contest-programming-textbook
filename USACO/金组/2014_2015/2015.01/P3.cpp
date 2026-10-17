#include <cstdio>

using namespace std;

const int MAXN = 100005;
const int MAXM = 100005;

int n, m;

// 原图与反图的邻接表（前向星）
int headG[MAXN], toG[MAXM], nxtG[MAXM], gCnt;
int headR[MAXN], toR[MAXM], nxtR[MAXM], rCnt;

int vis[MAXN];
int iter[MAXN];   // 迭代式 DFS 中每个点当前处理到的边
int stk[MAXN];
int order[MAXN], ordCnt;

int comp[MAXN];       // 每个点所属的强连通分量编号（从 1 开始，0 表示未访问）
int compSize[MAXN];
int compCnt;

// 缩点后的 DAG：只保留跨越分量的边
int dHead[MAXN], dTo[MAXM], dNxt[MAXM], dCnt;
int irHead[MAXN], irTo[MAXM], irNxt[MAXM], irCnt;

int bestTo[MAXN];    // bestTo[c]：从 c 走到 1 所在分量的最大点权和（不计 1 的分量）；-1 表示走不到
int bestFrom[MAXN];  // bestFrom[c]：从 1 所在分量走到 c 的最大点权和（不计 1 的分量）；-1 表示不可达

void addEdge(int* head, int* to, int* nxt, int& cnt, int u, int v) {
    to[cnt] = v;
    nxt[cnt] = head[u];
    head[u] = cnt;
    ++cnt;
}

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; ++i) {
        headG[i] = -1;
        headR[i] = -1;
    }
    for (int i = 0; i < m; ++i) {
        int u, v;
        scanf("%d %d", &u, &v);
        addEdge(headG, toG, nxtG, gCnt, u, v);
        addEdge(headR, toR, nxtR, rCnt, v, u);
    }

    // Kosaraju 第一遍：在原图上求完成时间序。路径可能长达 1e5，用显式栈代替递归
    for (int s = 1; s <= n; ++s) {
        if (vis[s]) continue;
        int top = 0;
        vis[s] = 1;
        iter[s] = headG[s];
        stk[top++] = s;
        while (top > 0) {
            int u = stk[top - 1];
            int e = iter[u];
            if (e == -1) {
                order[ordCnt++] = u;
                --top;
            } else {
                iter[u] = nxtG[e];  // 先推进，保证每条边只被看一次
                int v = toG[e];
                if (!vis[v]) {
                    vis[v] = 1;
                    iter[v] = headG[v];
                    stk[top++] = v;
                }
            }
        }
    }

    // 第二遍：在反图上按完成时间从晚到早 DFS，得到的编号天然是 DAG 的拓扑序（边从小编号指向大编号）
    for (int i = ordCnt - 1; i >= 0; --i) {
        int s = order[i];
        if (comp[s]) continue;
        ++compCnt;
        int top = 0;
        comp[s] = compCnt;
        stk[top++] = s;
        int size = 0;
        while (top > 0) {
            int u = stk[--top];
            ++size;
            for (int e = headR[u]; e != -1; e = nxtR[e]) {
                int v = toR[e];
                if (!comp[v]) {
                    comp[v] = compCnt;
                    stk[top++] = v;
                }
            }
        }
        compSize[compCnt] = size;
    }

    for (int c = 1; c <= compCnt; ++c) {
        dHead[c] = -1;
        irHead[c] = -1;
    }
    for (int u = 1; u <= n; ++u) {
        for (int e = headG[u]; e != -1; e = nxtG[e]) {
            int v = toG[e];
            if (comp[u] == comp[v]) continue;
            addEdge(dHead, dTo, dNxt, dCnt, comp[u], comp[v]);       // 分量 comp[u] -> comp[v]
            addEdge(irHead, irTo, irNxt, irCnt, comp[v], comp[u]);   // 反向，用来查入边
        }
    }

    int c1 = comp[1];

    // 倒着扫：算"从 c 出发回到 1 所在分量"的最长路，c 的权值先加进去
    for (int c = compCnt; c >= 1; --c) {
        if (c == c1) {
            bestTo[c] = 0;
            continue;
        }
        int best = -1;
        for (int e = dHead[c]; e != -1; e = dNxt[e]) {
            int d = dTo[e];  // 拓扑序保证 d > c，已经算好
            if (bestTo[d] > best) best = bestTo[d];
        }
        bestTo[c] = (best < 0) ? -1 : best + compSize[c];
    }

    // 正着扫：算"从 1 所在分量走到 c"的最长路，c 的权值先加进去
    for (int c = 1; c <= compCnt; ++c) {
        if (c == c1) {
            bestFrom[c] = 0;
            continue;
        }
        int best = -1;
        for (int e = irHead[c]; e != -1; e = irNxt[e]) {
            int a = irTo[e];  // a -> c，拓扑序保证 a < c，已经算好
            if (bestFrom[a] > best) best = bestFrom[a];
        }
        bestFrom[c] = (best < 0) ? -1 : best + compSize[c];
    }

    // 逆行一条 c -> d 的边，等价于路线先从 1 走到 d，再跳到 c 后走回 1。
    // DAG 中 1 所在分量之外的任何分量不可能既能从 1 到达又能回到 1，
    // 所以两条路径除 1 所在分量外不会重叠，可以直接相加。
    int answer = compSize[c1];
    for (int c = 1; c <= compCnt; ++c) {
        for (int e = dHead[c]; e != -1; e = dNxt[e]) {
            int d = dTo[e];
            if (bestFrom[d] < 0 || bestTo[c] < 0) continue;
            int cand = compSize[c1] + bestFrom[d] + bestTo[c];
            if (cand > answer) answer = cand;
        }
    }
    printf("%d\n", answer);
    return 0;
}
