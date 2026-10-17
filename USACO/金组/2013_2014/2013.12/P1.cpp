#include <cstdio>
#include <queue>

using namespace std;

const int MAXN = 20005;
const int MAXM = 20005;
const int MAXK = 205;
// 比任何真实花费都大: 一条路线最多 N 条边, 每条不超过 10000.
const int INF = 1000000000;

int n, m, k, q;
int hub[MAXK];
int headF[MAXN], headR[MAXN]; // 正向图和反向图的邻接表头
int toF[MAXM], wtF[MAXM], nxF[MAXM];
int toR[MAXM], wtR[MAXM], nxR[MAXM];
int edgeCnt;

// distIn[i][x] = 原图上 x 走到 hub[i] 的最小花费
// distOut[i][x] = 原图上 hub[i] 走到 x 的最小花费
int distIn[MAXK][MAXN];
int distOut[MAXK][MAXN];

struct State {
    int d;
    int u;

    // 优先队列默认是大根堆, 反过来比较让它变成小根堆.
    bool operator<(const State &other) const {
        return d > other.d;
    }
};

void AddEdge(int u, int v, int w) {
    edgeCnt++;
    toF[edgeCnt] = v;
    wtF[edgeCnt] = w;
    nxF[edgeCnt] = headF[u];
    headF[u] = edgeCnt;

    toR[edgeCnt] = u;
    wtR[edgeCnt] = w;
    nxR[edgeCnt] = headR[v];
    headR[v] = edgeCnt;
}

// 在给定的一张邻接表上求单源最短路, 结果写进 dist[].
void Dijkstra(int src, int head[], int to[], int wt[], int nx[], int dist[]) {
    for (int i = 1; i <= n; i++) dist[i] = INF;
    dist[src] = 0;
    priority_queue<State> pq;
    State start;
    start.d = 0;
    start.u = src;
    pq.push(start);
    while (!pq.empty()) {
        State cur = pq.top();
        pq.pop();
        // 堆里的旧状态: 该点已被更短的路径更新过, 直接丢弃.
        if (cur.d > dist[cur.u]) continue;
        for (int e = head[cur.u]; e != 0; e = nx[e]) {
            int v = to[e];
            if (cur.d + wt[e] < dist[v]) {
                dist[v] = cur.d + wt[e];
                State nxt;
                nxt.d = dist[v];
                nxt.u = v;
                pq.push(nxt);
            }
        }
    }
}

int main() {
    scanf("%d %d %d %d", &n, &m, &k, &q);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        AddEdge(u, v, w);
    }
    for (int i = 0; i < k; i++) scanf("%d", &hub[i]);

    // 每条航班至少有一端是枢纽, 所以任何一条路线都是从某个枢纽出发或落到某个
    // 枢纽上, 于是 a 到 b 的最短路一定可以拆成 a->h 加上 h->b (h 为某个枢纽).
    // 只要以每个枢纽为源点各跑一遍最短路就够了.
    for (int i = 0; i < k; i++) {
        Dijkstra(hub[i], headF, toF, wtF, nxF, distOut[i]);
        // 把边全部反向后再从 hub[i] 出发, 得到的就是原图上各点走到 hub[i] 的花费.
        Dijkstra(hub[i], headR, toR, wtR, nxR, distIn[i]);
    }

    int ok = 0;
    long long total = 0;
    for (int i = 0; i < q; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        long long best = -1;
        for (int h = 0; h < k; h++) {
            if (distIn[h][a] >= INF || distOut[h][b] >= INF) continue;
            long long cost = (long long)distIn[h][a] + distOut[h][b];
            if (best < 0 || cost < best) best = cost;
        }
        if (best >= 0) {
            ok++;
            total += best;
        }
    }
    printf("%d\n%lld\n", ok, total);
    return 0;
}
