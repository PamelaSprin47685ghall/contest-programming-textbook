#include <algorithm>
#include <cstdio>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

const int MAXN = 255;
const int MAXM = 25005;
const int INF = 1000000000;

int n, m;
int eu[MAXM], ev[MAXM], elen[MAXM];  // 每条边的两个端点和长度
vector<int> adj[MAXN];               // adj[u]: 与点 u 相邻的所有边的编号
int prevEdge[MAXN];                  // 最短路中每个点最后经过的边

// 从 0 号点出发跑 Dijkstra, 长度用当前(可能已被翻倍的) elen, 返回到 n-1 号点的距离.
int Dijkstra() {
    int dist[MAXN];
    for (int i = 0; i < n; ++i) {
        dist[i] = INF;
        prevEdge[i] = -1;
    }
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
    dist[0] = 0;
    pq.push(make_pair(0, 0));
    while (!pq.empty()) {
        int d = pq.top().first, u = pq.top().second;
        pq.pop();
        if (d > dist[u]) {
            continue;  // 过期的堆元素
        }
        for (int i = 0; i < (int)adj[u].size(); ++i) {
            int id = adj[u][i];
            int v = eu[id] + ev[id] - u;  // 边 id 的另一个端点
            int nd = d + elen[id];
            if (nd < dist[v]) {
                dist[v] = nd;
                prevEdge[v] = id;
                pq.push(make_pair(nd, v));
            }
        }
    }
    return dist[n - 1];
}

int main() {
    scanf("%d%d", &n, &m);
    for (int i = 0; i < m; ++i) {
        scanf("%d%d%d", &eu[i], &ev[i], &elen[i]);
        --eu[i];
        --ev[i];
        adj[eu[i]].push_back(i);
        adj[ev[i]].push_back(i);
    }

    int original = Dijkstra();

    // 把一条原始最短路上经过的边都收集起来.
    vector<int> pathEdges;
    for (int v = n - 1; v != 0; v = eu[prevEdge[v]] + ev[prevEdge[v]] - v) {
        pathEdges.push_back(prevEdge[v]);
    }

    // 翻倍不在最短路上的边时, 原来的最短路原封不动, 距离不会增加;
    // 所以只需枚举最短路上的每条边, 翻倍后重跑 Dijkstra.
    int best = original;
    for (int i = 0; i < (int)pathEdges.size(); ++i) {
        int id = pathEdges[i];
        elen[id] *= 2;
        best = max(best, Dijkstra());
        elen[id] /= 2;  // 还原, 继续尝试下一条边
    }
    printf("%d\n", best - original);
    return 0;
}
