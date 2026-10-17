#include <cstdio>

using namespace std;

const int MAXN = 100005;
const int MAXE = 2 * MAXN;

int n;
int head[MAXN], eNext[MAXE], eTo[MAXE], eW[MAXE], edgeCnt;

bool removed[MAXN];    // 分治时已经被删掉的点
int subSize[MAXN];     // 当前连通块内以某点为根的子树大小
int par[MAXN];         // 当前连通块内的父亲
int order[MAXN];       // 迭代 DFS 的访问顺序

// 把 Charcolais 记为 -1, Angus 记为 +1, 于是"两半里两种牧群数量相等"就是"两半的权值和为 0".
// 一条合法路径等价于: 总权值为 0, 且路径内部存在一个顶点, 它到某一端的权值和为 0.
// 以分治中心为根时, 设终点到中心的部分和为 w, 则另一端必须是 -w.
// 下面两张表按 w 分类计数: first 表示"路径上出现过同样的部分和"(即存在可用的休息站),
// second 表示"没出现过".
int Bfirst[2 * MAXN + 1], Bsecond[2 * MAXN + 1];  // 已经处理过的子树 + 中心自己
int Cfirst[2 * MAXN + 1], Csecond[2 * MAXN + 1];  // 正在处理的子树
int touched[2 * MAXN + 1];                        // 本子树碰到的下标, 用来快速清空
int off;                                          // 下标偏移: 部分和可能是负数

int stNode[MAXN], stPar[MAXN], stW[MAXN], stMn[MAXN], stMx[MAXN];

void addEdge(int u, int v, int w) {
    eTo[edgeCnt] = v;
    eW[edgeCnt] = w;
    eNext[edgeCnt] = head[u];
    head[u] = edgeCnt;
    edgeCnt++;
}

// 迭代 DFS 求连通块大小, 同时记录每个点的子树大小
int computeSizes(int root) {
    int cnt = 0;
    order[cnt++] = root;
    par[root] = 0;
    // 队列式遍历: 保证父亲先于孩子入队
    for (int qh = 0; qh < cnt; qh++) {
        int u = order[qh];
        for (int e = head[u]; e != -1; e = eNext[e]) {
            int v = eTo[e];
            if (removed[v] || v == par[u]) continue;
            par[v] = u;
            order[cnt++] = v;
        }
    }
    // 逆序遍历即可先算完孩子再算父亲
    for (int i = cnt - 1; i >= 0; i--) {
        int u = order[i];
        subSize[u] = 1;
        for (int e = head[u]; e != -1; e = eNext[e]) {
            int v = eTo[e];
            if (removed[v] || v == par[u]) continue;
            subSize[u] += subSize[v];
        }
    }
    return cnt;
}

// 从 root 出发走到重心: 只要有孩子的子树超过一半就往里走
int findCentroid(int root, int total) {
    int u = root, p = 0;
    while (true) {
        int heavy = 0;
        for (int e = head[u]; e != -1; e = eNext[e]) {
            int v = eTo[e];
            if (removed[v] || v == p) continue;
            if (subSize[v] > total / 2) {
                heavy = v;
                break;
            }
        }
        if (heavy == 0) return u;
        p = u;
        u = heavy;
    }
}

// 统计所有"经过中心 c"的合法路径
long long countThrough(int c, int compSize) {
    long long result = 0;

    // 中心自己的部分和是 0, 且它到自己的路径上没有中间顶点
    for (int w = -compSize; w <= compSize; w++) {
        Bfirst[w + off] = 0;
        Bsecond[w + off] = 0;
    }
    Bsecond[off] = 1;

    for (int e = head[c]; e != -1; e = eNext[e]) {
        int v = eTo[e];
        if (removed[v]) continue;
        int ew = eW[e];

        // 迭代遍历这棵子树, 同时维护路径上的最小 / 最大部分和:
        // 若当前部分和恰是新的极值, 说明之前没出现过同样的值 (second);
        // 否则必然在极值之间"路过"过同样的值 (first).
        int touchedCnt = 0;
        int sp = 0;
        stNode[sp] = v; stPar[sp] = c; stW[sp] = ew; stMn[sp] = ew; stMx[sp] = ew; sp++;
        while (sp > 0) {
            sp--;
            int u = stNode[sp], pu = stPar[sp], w = stW[sp], mn = stMn[sp], mx = stMx[sp];
            int idx = w + off;
            int nmn, nmx;
            if (w == mn || w == mx) {
                if (Cfirst[idx] + Csecond[idx] == 0) touched[touchedCnt++] = idx;
                Csecond[idx]++;
                nmn = w - 1 < mn ? w - 1 : mn;
                nmx = w + 1 > mx ? w + 1 : mx;
            } else {
                if (Cfirst[idx] + Csecond[idx] == 0) touched[touchedCnt++] = idx;
                Cfirst[idx]++;
                nmn = mn;
                nmx = mx;
            }
            for (int ee = head[u]; ee != -1; ee = eNext[ee]) {
                int x = eTo[ee];
                if (x == pu || removed[x]) continue;
                stNode[sp] = x; stPar[sp] = u; stW[sp] = w + eW[ee];
                stMn[sp] = nmn; stMx[sp] = nmx; sp++;
            }
        }

        // 两端分别落在不同子树(或一端是中心)里的路径: 两端部分和必须互为相反数,
        // 并且至少有一端"路上出现过同样的部分和", 那一处就是休息站.
        for (int i = 0; i < touchedCnt; i++) {
            int idx = touched[i];
            int w = idx - off;
            int jdx = off - w;
            result += (long long)Cfirst[idx] * (Bfirst[jdx] + Bsecond[jdx]);
            result += (long long)Csecond[idx] * Bfirst[jdx];
        }
        // 部分和为 0 的两端可以拿中心当休息站; 但要排掉"另一端就是中心"的情况,
        // 那时路径上没有中间顶点可用.
        result += (long long)Csecond[off] * (Bsecond[off] - 1);

        for (int i = 0; i < touchedCnt; i++) {
            int idx = touched[i];
            Bfirst[idx] += Cfirst[idx];
            Bsecond[idx] += Csecond[idx];
            Cfirst[idx] = 0;
            Csecond[idx] = 0;
        }
    }
    return result;
}

long long decompose(int root) {
    int compSize = computeSizes(root);
    int c = findCentroid(root, compSize);
    long long result = countThrough(c, compSize);
    removed[c] = true;
    for (int e = head[c]; e != -1; e = eNext[e]) {
        int v = eTo[e];
        if (!removed[v]) result += decompose(v);
    }
    return result;
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) head[i] = -1;
    for (int i = 1; i < n; i++) {
        int a, b, t;
        scanf("%d %d %d", &a, &b, &t);
        int w = t == 0 ? -1 : 1;
        addEdge(a, b, w);
        addEdge(b, a, w);
    }
    off = n + 1;  // 部分和的绝对值不超过连通块大小, 偏移后一定落在表内
    printf("%lld\n", decompose(1));
    return 0;
}
