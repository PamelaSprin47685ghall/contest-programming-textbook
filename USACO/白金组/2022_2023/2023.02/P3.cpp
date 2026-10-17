#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 200005;
const int MAXS = 2 * MAXN + 5; // 骨架至多 2*激活点数-1 个节点
const long long INF = 1000000000000000LL; // 1e15，远大于任何真实花费

int N;
char bitStr[MAXN];
int active[MAXN];
int activeCnt;

int ea[MAXN], eb[MAXN];
int head[MAXN], nxt[2 * MAXN], to[2 * MAXN], edgeCnt;

int par[MAXN], depthArr[MAXN], bfsOrder[MAXN], bfsCnt, queueArr[MAXN];

// 所有激活点的最小连通子树（"细版"），再压缩掉度为 2 的非激活点得到骨架
int cntAct[MAXN];   // 子树内激活点数量
int childBr[MAXN];  // 子树内含激活点的儿子个数
int steiner[MAXN];  // 是否在所有激活点的最小连通子树上
int keep[MAXN];     // 压缩后保留的节点：激活点或分叉点

int skelPar[MAXN], pdist[MAXN]; // 骨架上的父亲、边长（原树上的距离）
int skelHead[MAXN], skelNxt[MAXN];
int skelNodes[MAXS], skelSize;
int nearDist[MAXN], secondDist[MAXN]; // 子树内最近、次近的激活点距离
int aboveDist[MAXN];                  // 父方向的最近激活点距离
int wEdge[MAXN];                      // 父边变成"必选"的最小 k（经过它的最短激活点对距离）
int wNode[MAXN];                      // 节点变成"必选"的最小 k
int wIrrel[MAXN];                     // 所有相邻边都必选（节点在 DP 中变得无关紧要）的最小 k

int cntNodeAt[MAXN + 2];  // wNode == k 的节点数
int cntEdgeAt[MAXN + 2];  // wEdge == k 的节点数（每个非根节点对应父边）
int irrelStart[MAXN + 2]; // wIrrel <= k 的节点数（前缀和）
int irrelItems[MAXS];     // 按 wIrrel 分组后的节点
int sortedByDepth[MAXS];  // 按深度降序的骨架节点
int linkNext[MAXN], linkPrev[MAXN]; // free 节点的双向链表，保持深度降序
long long dp0[MAXN], dp1[MAXN];
long long ansArr[MAXN];

int cmpDepth(const int &a, const int &b) { return depthArr[a] > depthArr[b]; }

int main() {
    scanf("%d", &N);
    scanf("%s", bitStr);
    for (int i = 1; i <= N; i++) {
        active[i] = (bitStr[i - 1] == '1') ? 1 : 0;
        if (active[i]) activeCnt++;
    }
    for (int i = 1; i <= N; i++) head[i] = -1;
    for (int i = 1; i < N; i++) {
        scanf("%d %d", &ea[i], &eb[i]);
        int u = ea[i], v = eb[i];
        to[edgeCnt] = v;
        nxt[edgeCnt] = head[u];
        head[u] = edgeCnt++;
        to[edgeCnt] = u;
        nxt[edgeCnt] = head[v];
        head[v] = edgeCnt++;
    }

    // 只有一个激活点：最优解就是只买它自己，代价 1 + k
    if (activeCnt == 1) {
        for (int k = 1; k <= N; k++) printf("%lld\n", (long long)k + 1);
        return 0;
    }

    // 以最后一个激活点为根（保证根是激活点），BFS 定序：父亲一定先于儿子被访问
    int root = 0;
    for (int i = 1; i <= N; i++)
        if (active[i]) root = i;
    int qh = 0, qt = 0;
    queueArr[qt++] = root;
    par[root] = 0;
    depthArr[root] = 0;
    while (qh < qt) {
        int v = queueArr[qh++];
        bfsOrder[bfsCnt++] = v;
        for (int e = head[v]; e != -1; e = nxt[e]) {
            int u = to[e];
            if (u == par[v]) continue;
            par[u] = v;
            depthArr[u] = depthArr[v] + 1;
            queueArr[qt++] = u;
        }
    }

    for (int i = 1; i <= N; i++) cntAct[i] = active[i];
    for (int i = bfsCnt - 1; i >= 1; i--) {
        int v = bfsOrder[i];
        cntAct[par[v]] += cntAct[v];
    }
    for (int i = 1; i < bfsCnt; i++) {
        int v = bfsOrder[i];
        if (cntAct[v] > 0) childBr[par[v]]++;
    }

    int nearKeptParent[MAXN], distToKept[MAXN];
    for (int i = 1; i <= N; i++) {
        steiner[i] = 0;
        keep[i] = 0;
        skelHead[i] = -1;
    }
    nearKeptParent[root] = 0;
    distToKept[root] = 0;
    steiner[root] = 1;
    keep[root] = 1; // 根是激活点，必须先确定，后面的递推要用到它
    for (int i = 1; i < bfsCnt; i++) {
        int v = bfsOrder[i];
        int p = par[v];
        // 最近保留祖先只需自上而下递推，BFS 序保证父节点已经算过
        if (keep[p]) {
            nearKeptParent[v] = p;
            distToKept[v] = 1;
        } else {
            nearKeptParent[v] = nearKeptParent[p];
            distToKept[v] = distToKept[p] + 1;
        }
        // 在最小连通子树上：子树里有激活点，且（至少两个方向含激活点，或者子树外还有激活点）
        int br = childBr[v] + active[v];
        if (cntAct[v] >= 1 && (br >= 2 || (br >= 1 && cntAct[v] < activeCnt))) steiner[v] = 1;
        // 压缩：只有激活点和分叉点需要保留，其余度为 2 的非激活点压成一条长边
        if (steiner[v] && (active[v] || childBr[v] >= 2)) keep[v] = 1;
    }
    skelPar[root] = 0;
    pdist[root] = 0;
    skelNodes[skelSize++] = root;

    for (int i = 1; i < bfsCnt; i++) {
        int v = bfsOrder[i];
        if (!keep[v]) continue;
        skelPar[v] = nearKeptParent[v];
        pdist[v] = distToKept[v];
        int p = skelPar[v];
        skelNxt[v] = skelHead[p];
        skelHead[p] = v;
        skelNodes[skelSize++] = v;
    }

    // 自底向上：子树内最近、次近的激活点距离
    for (int i = bfsCnt - 1; i >= 0; i--) {
        int v = bfsOrder[i];
        if (!keep[v]) continue;
        long long n1 = active[v] ? 0 : INF;
        long long n2 = n1;
        for (int c = skelHead[v]; c != -1; c = skelNxt[c]) {
            long long cand = nearDist[c] + pdist[c];
            if (cand < n1) {
                n2 = n1;
                n1 = cand;
            } else if (cand < n2) {
                n2 = cand;
            }
        }
        nearDist[v] = (int)(n1 > INF ? INF : n1);
        secondDist[v] = (int)(n2 > INF ? INF : n2);
    }

    // 自顶向下：父边被强制的最小 k = 经过它的最短激活点对距离
    aboveDist[root] = 0;
    wEdge[root] = 0;
    for (int i = 0; i < bfsCnt; i++) {
        int v = bfsOrder[i];
        if (!keep[v]) continue;
        for (int c = skelHead[v]; c != -1; c = skelNxt[c]) {
            long long other = (nearDist[c] + pdist[c] == nearDist[v]) ? secondDist[v] : nearDist[v];
            long long up = other < aboveDist[v] ? other : aboveDist[v];
            aboveDist[c] = (int)(up + pdist[c]);
            wEdge[c] = aboveDist[c] + nearDist[c];
        }
    }

    for (int i = 0; i < bfsCnt; i++) {
        int v = bfsOrder[i];
        if (!keep[v]) continue;
        long long nodeForce = active[v] ? 0 : INF;
        long long irrel = wEdge[v];
        for (int c = skelHead[v]; c != -1; c = skelNxt[c]) {
            if (wEdge[c] < nodeForce) nodeForce = wEdge[c];
            if (wEdge[c] > irrel) irrel = wEdge[c];
        }
        if (wEdge[v] < nodeForce) nodeForce = wEdge[v];
        wNode[v] = (int)nodeForce;
        wIrrel[v] = (int)irrel;
        cntNodeAt[wNode[v]]++;
    }
    // 压缩边变成"必选"时，链中间的那些节点与整条边的单位边数也要一起计入常量：
    // 一条长为 L 的压缩边贡献 L-1 个中间点与 L 条单位边，于是两端被并进同一个强制连通块
    for (int i = 1; i < bfsCnt; i++) {
        int v = bfsOrder[i];
        if (!keep[v]) continue;
        cntNodeAt[wEdge[v]] += pdist[v] - 1;
        cntEdgeAt[wEdge[v]] += pdist[v];
    }

    // wIrrel 分组（计数排序）
    for (int i = 0; i <= N + 1; i++) irrelStart[i] = 0;
    for (int i = 0; i < skelSize; i++) irrelStart[wIrrel[skelNodes[i]]]++;
    for (int i = 1; i <= N + 1; i++) irrelStart[i] += irrelStart[i - 1];
    {
        int fill[MAXN + 2];
        for (int i = 0; i <= N + 1; i++) fill[i] = irrelStart[i];
        for (int i = 0; i < skelSize; i++) {
            int v = skelNodes[i];
            irrelItems[--fill[wIrrel[v]]] = v;
        }
    }
    // 上面用 --fill 让每组内部保持任意顺序，区间为 [irrelStart[k-1], irrelStart[k])

    // 按深度降序建双向链表，free 节点始终按深度降序排列
    for (int i = 0; i < skelSize; i++) sortedByDepth[i] = skelNodes[i];
    sort(sortedByDepth, sortedByDepth + skelSize, cmpDepth);
    int listHead = 0;
    // 从浅到浅地"头插"，链表最终就是深度降序；这样保证儿子先于父亲被 DP 结算
    for (int i = skelSize - 1; i >= 0; i--) {
        int v = sortedByDepth[i];
        linkPrev[v] = 0;
        linkNext[v] = listHead;
        if (listHead) linkPrev[listHead] = v;
        listHead = v;
    }

    for (int i = 0; i <= N; i++) {
        dp0[i] = 0;
        dp1[i] = 0;
    }

    long long currNodesForced = cntNodeAt[0]; // k=0 时只有激活点被强制
    long long currEdgesForced = 0;

    for (int k = 1; k <= N; k++) {
        // 1) 移除所有相邻边都被强制的节点（它们的代价已经全部记在全局常量里）
        int from = (k == 1 ? 0 : irrelStart[k - 1]);
        for (int i = from; i < irrelStart[k]; i++) {
            int v = irrelItems[i];
            int pv = linkPrev[v];
            int nx = linkNext[v];
            if (pv) linkNext[pv] = nx;
            else listHead = nx;
            if (nx) linkPrev[nx] = pv;
        }
        // 2) 新被强制的点、边计入常量：强制连通块个数 = 强制点数 - 强制边数
        currNodesForced += cntNodeAt[k];
        currEdgesForced += cntEdgeAt[k];
        long long forcedComponents = currNodesForced - currEdgesForced;
        long long answer = (long long)k * forcedComponents + currNodesForced;

        // 3) 在剩余 free 节点上做树形 DP（深度降序保证儿子先于父亲结算）
        for (int a = listHead; a != 0; a = linkNext[a]) {
            long long a0 = dp0[a];
            long long a1 = dp1[a];
            if (wNode[a] <= k) {
                a0 = INF; // 这个点必选，代价已计入常量
            } else {
                a1 += k + 1; // 自己开一个账户
            }
            if (wEdge[a] <= k) {
                answer += a1; // 父边被强制：这一块并进上面的强制部分，不再另付 k
            } else {
                int p = skelPar[a];
                long long m0 = a0 < a1 ? a0 : a1;
                dp0[p] += m0;
                if (dp0[p] > INF) dp0[p] = INF;
                long long m1 = a1;
                long long merged = a1 - k + (pdist[a] - 1); // 合并：退掉 k，补上链中间的节点
                if (merged < m1) m1 = merged;
                if (a0 < m1) m1 = a0;
                dp1[p] += m1;
                if (dp1[p] > INF) dp1[p] = INF;
            }
            dp0[a] = 0;
            dp1[a] = 0;
        }
        ansArr[k] = answer;
    }

    for (int k = 1; k <= N; k++) printf("%lld\n", ansArr[k]);
    return 0;
}
