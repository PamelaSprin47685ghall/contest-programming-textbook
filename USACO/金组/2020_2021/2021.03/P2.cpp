#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100000;

struct Edge {
    int u, v, cost;
};

int n;
Edge edges[3 * MAXN];  // 每个顶点贡献两条 0 费边（原有配对）和一条 c_v 费边（拆开重连）
int parent[2 * MAXN + 1];

bool cmpEdge(const Edge& a, const Edge& b) {
    return a.cost < b.cost;
}

int findRoot(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

int main() {
    scanf("%d", &n);
    int m = 0;
    for (int v = 1; v <= n; ++v) {
        int c, p1, p2, p3, p4;
        scanf("%d %d %d %d %d", &c, &p1, &p2, &p3, &p4);
        // 把每个传送门看成图上的一个点：顶点 v 当前把 p1 与 p2、p3 与 p4 配对，
        // 于是全图是若干互不相交的环。花费 c_v 重新配对，可以让 p1 所在分量与 p3 所在分量合并，
        // 因此"用什么代价把所有环连起来"就是在这张 2N 点图上求最小生成树。
        edges[m].u = p1; edges[m].v = p2; edges[m].cost = 0; ++m;
        edges[m].u = p3; edges[m].v = p4; edges[m].cost = 0; ++m;
        edges[m].u = p1; edges[m].v = p3; edges[m].cost = c;  ++m;  // 跨过两配对组的重连代价
    }
    sort(edges, edges + m, cmpEdge);
    for (int i = 1; i <= 2 * n; ++i) parent[i] = i;
    long long ans = 0;
    for (int i = 0; i < m; ++i) {
        int ru = findRoot(edges[i].u);
        int rv = findRoot(edges[i].v);
        if (ru != rv) {
            parent[ru] = rv;
            ans += edges[i].cost;
        }
    }
    printf("%lld\n", ans);
    return 0;
}
