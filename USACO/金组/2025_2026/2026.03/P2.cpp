// P2 Picking Flowers
// 关键性质：漂亮路径上从 1 到 v 的那一段，长度一定等于 dist(v)（否则能拼出更短的到终点的路）。
// 于是把点按 dist 分层，一条漂亮路径每层恰取一个点。
// 记 flower[l] 为第 l 层上的花田（同一层出现两朵花田就无解）：
//   dp[v]  = 从 v 出发能否沿"每层走一个点"的方式走到某个合法终点（后缀合法性）；
//   dp2[v] = 从 1 出发能否沿同样方式走到 v（前缀合法性，必须逐层踩中花田）。
// 把 f 变成花田后存在漂亮路径 <=> 存在最短路径同时访问所有花田和 f <=> dp[f] && dp2[f]。
#include <cstdio>

using namespace std;

const int MAXN = 200005;   // 单次测试的点数上界
const int MAXM = 400005;   // 无向边存两次

int n, m, k, l;
int head[MAXN], nxt[MAXM], to[MAXM], ecnt;
int dist[MAXN], bfsQ[MAXN];
int layerStart[MAXN];      // 第 d 层的点在 layerOrd 中的区间起点
int layerCursor[MAXN];
int layerOrd[MAXN];
int flower[MAXN];          // flower[d]：第 d 层上的花田编号，0 表示该层无花田
int flowerList[MAXN], destList[MAXN];
char dp[MAXN], dp2[MAXN];

void addEdge(int u, int v) {
    ecnt++;
    to[ecnt] = v;
    nxt[ecnt] = head[u];
    head[u] = ecnt;
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d %d %d %d", &n, &m, &k, &l);
        for (int i = 1; i <= k; i++) scanf("%d", &flowerList[i]);
        for (int i = 1; i <= l; i++) scanf("%d", &destList[i]);
        ecnt = 0;
        for (int i = 1; i <= n; i++) {
            head[i] = 0;
            dist[i] = -1;
            flower[i - 1] = 0;   // 顺带把 0..n-1 层的花田表清空
            dp[i] = 0;
            dp2[i] = 0;
        }
        flower[n] = 0;
        for (int i = 0; i < m; i++) {
            int u, v;
            scanf("%d %d", &u, &v);
            addEdge(u, v);
            addEdge(v, u);
        }

        // 从 1 开始 BFS 求分层
        int qh = 0, qt = 0;
        dist[1] = 0;
        bfsQ[qt++] = 1;
        int maxD = 0;
        while (qh < qt) {
            int u = bfsQ[qh++];
            for (int e = head[u]; e; e = nxt[e]) {
                int v = to[e];
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    if (dist[v] > maxD) maxD = dist[v];
                    bfsQ[qt++] = v;
                }
            }
        }

        // 按层计数排序，把点按层号排好
        for (int d = 0; d <= maxD + 1; d++) layerStart[d] = 0;
        for (int i = 1; i <= n; i++) layerStart[dist[i] + 1]++;
        for (int d = 0; d <= maxD; d++) layerStart[d + 1] += layerStart[d];
        for (int d = 0; d <= maxD; d++) layerCursor[d] = layerStart[d];
        for (int i = 1; i <= n; i++) {
            int d = dist[i];
            layerOrd[layerCursor[d]++] = i;
        }

        int bad = 0, mx = 0;   // mx：最深的那个花田所在层
        for (int i = 1; i <= k; i++) {
            int d = dist[flowerList[i]];
            if (flower[d] != 0) bad = 1;   // 同一层两朵花田：每层只能取一个点，必然无解
            flower[d] = flowerList[i];
            if (d > mx) mx = d;
        }
        if (bad) {
            for (int i = 1; i < n; i++) putchar('0');
            putchar('\n');
            continue;
        }

        // 终点必须在所有花田之后，或者它自己就是最深的那朵花田
        for (int i = 1; i <= l; i++) {
            int v = destList[i];
            if (dist[v] > mx) dp[v] = 1;
            else if (dist[v] == mx && flower[mx] == v) dp[v] = 1;
        }

        // 后缀 dp：从深层往浅层推。下一层若有花田，就只能走进那朵花田
        for (int d = maxD - 1; d >= 0; d--) {
            int tgt = flower[d + 1];
            for (int t = layerStart[d]; t < layerStart[d + 1]; t++) {
                int u = layerOrd[t];
                for (int e = head[u]; e; e = nxt[e]) {
                    int v = to[e];
                    if (tgt) {
                        if (v == tgt) dp[u] |= dp[v];
                    } else if (dist[v] == d + 1) {
                        dp[u] |= dp[v];
                    }
                }
            }
        }

        // 前缀 dp2：从浅层往深层推，规则相同
        dp2[1] = 1;
        for (int d = 0; d < maxD; d++) {
            int tgt = flower[d + 1];
            for (int t = layerStart[d]; t < layerStart[d + 1]; t++) {
                int u = layerOrd[t];
                if (!dp2[u]) continue;
                for (int e = head[u]; e; e = nxt[e]) {
                    int v = to[e];
                    if (tgt) {
                        if (v == tgt) dp2[v] = 1;
                    } else if (dist[v] == d + 1) {
                        dp2[v] = 1;
                    }
                }
            }
        }

        for (int i = 2; i <= n; i++)
            putchar((dp[i] && dp2[i]) ? '1' : '0');
        putchar('\n');
    }
    return 0;
}
