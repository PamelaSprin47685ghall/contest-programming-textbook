#include <cstdio>

using namespace std;

// 这是一道交互题：真正的 grader 会提供下面 4 个函数（提交模板里的 grader.h 只是声明它们，
// 这里为编译方便自行声明，签名与官方一致）。本文件不包含 main，也不 include grader.h。
int getN();
int getQ();
void setFarmLocation(int ID, int X, int Y);
void addBox(int X1, int Y1, int X2, int Y2);

const int MAXN = 100005;
const int LOGN = 18;   // 2^17 > 10^5

static int roadA[MAXN], roadB[MAXN], roadCnt = 0;   // addRoad 阶段先把 N-1 条边记下来

static int head[MAXN], toNode[2 * MAXN], nextEdge[2 * MAXN], edgeCnt = 0;
static int par[MAXN], dep[MAXN], tin[MAXN], tout[MAXN], timer = 0;
static int farmN = 0;
static int dfStack[MAXN], dfPtr[MAXN];
static int up[MAXN][LOGN];

void addRoad(int a, int b) {
    ++roadCnt;
    roadA[roadCnt] = a;
    roadB[roadCnt] = b;
}

void buildFarms() {
    int n = getN();
    farmN = n;

    edgeCnt = 0;
    for (int v = 0; v < n; ++v) {
        head[v] = -1;
        tin[v] = 0;
    }
    for (int e = 1; e <= roadCnt; ++e) {
        int a = roadA[e], b = roadB[e];
        toNode[edgeCnt] = b;
        nextEdge[edgeCnt] = head[a];
        head[a] = edgeCnt++;
        toNode[edgeCnt] = a;
        nextEdge[edgeCnt] = head[b];
        head[b] = edgeCnt++;
    }

    // 迭代式 DFS（树可能退化成 10^5 深的链，递归写法容易爆栈），
    // 同时算出先序编号 tin 与子树最大先序编号 tout。
    int top = 0;
    timer = 0;
    par[0] = -1;
    dep[0] = 0;
    dfPtr[0] = head[0];
    dfStack[0] = 0;
    while (top >= 0) {
        int v = dfStack[top];
        if (tin[v] == 0) tin[v] = ++timer;
        int e = dfPtr[v];
        while (e != -1 && toNode[e] == par[v]) e = nextEdge[e];   // 树上只有一条回到父亲的边
        if (e == -1) {
            tout[v] = timer;   // 子树遍历完时，timer 恰好停在子树最后一个先序号上
            --top;
        } else {
            int w = toNode[e];
            dfPtr[v] = nextEdge[e];
            par[w] = v;
            dep[w] = dep[v] + 1;
            dfPtr[w] = head[w];
            dfStack[++top] = w;
        }
    }

    // 把农场 v 摆在 (x, y) = (tin[v], tout[v])（都在 1..n 内，且两两不同）。
    // 先序编号把每棵子树压成一段连续区间，于是"u 是 v 的祖先"等价于
    // tin[u] <= tin[v] 且 tout[u] >= tout[v]；因此矩形 {x <= tin[v]} × {y >= tout[v]}
    // 里恰好是先序意义下 v 的全体祖先，这是后面用矩形覆盖路径的基础。
    for (int v = 0; v < n; ++v) setFarmLocation(v, tin[v], tout[v]);

    // 倍增祖先表，供 O(log n) 的 LCA 使用（Q 最多 10^5，询问必须快）。
    up[0][0] = 0;
    for (int v = 1; v < n; ++v) up[v][0] = par[v];
    for (int j = 1; j < LOGN; ++j) {
        for (int v = 0; v < n; ++v) up[v][j] = up[up[v][j - 1]][j - 1];
    }

    roadCnt = 0;   // 复位，方便同一进程里重复建树
}

static int lca(int a, int b) {
    if (dep[a] < dep[b]) {
        int t = a;
        a = b;
        b = t;
    }
    int diff = dep[a] - dep[b];
    for (int j = 0; j < LOGN; ++j) {
        if ((diff >> j) & 1) a = up[a][j];
    }
    if (a == b) return a;
    for (int j = LOGN - 1; j >= 0; --j) {
        if (up[a][j] != up[b][j]) {
            a = up[a][j];
            b = up[b][j];
        }
    }
    return up[a][0];
}

void notifyFJ(int a, int b) {
    int l = lca(a, b);
    if (l == a) {
        // a 是 b 的祖先：路径就是 b 到 a 的一条祖先链。
        // 先序区间套在一条祖先链上层层包含，x 落在 [tin[a], tin[b]] 且 y >= tout[b]
        // 的农场恰好是 b 的祖先中"不早于 a"的那些，即 b..a 这条链。
        addBox(tin[a], tout[b], tin[b], farmN);
    } else if (l == b) {
        addBox(tin[b], tout[a], tin[a], farmN);
    } else {
        // 路径 = a 到 l 的链 + b 到 l 的链。用 x 下界 tin[l] 把祖先链截到 l 为止：
        // 第一条矩形装下 a..l；第二条把 x 下界抬到 tin[l]+1 去掉 l，
        // 于是两个矩形的农场集合不相交（否则会有 a、b 在 l 以下的公共祖先，与 l = LCA 矛盾）。
        addBox(tin[l], tout[a], tin[a], farmN);
        addBox(tin[l] + 1, tout[b], tin[b], farmN);
    }
}
