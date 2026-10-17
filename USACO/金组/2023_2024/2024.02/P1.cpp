#include <cstdio>
#include <queue>
#include <vector>

using namespace std;

const int MAXN = 50005;
const int MAXK = 12;

struct Edge {
    int to;
    int len;
};

struct Pending {
    long long dist;
    int node;
    int root;
    // 优先队列默认是大根堆，这里把比较反过来，让距离小的先出队
    bool operator<(const Pending &other) const {
        return dist > other.dist;
    }
};

int n, m, c, k;
long long r;
vector<Edge> adj[MAXN];

// visits[v]：v 已经"接待"过的充电站数量（最多累计到 k）
// visitorList[v]：记录是哪些充电站访问过 v。必须去重，否则同一个充电站
// 通过两条不同的路都能到达 v 时，会被当成两个不同的充电站重复计数。
int visits[MAXN];
int visitorList[MAXN][MAXK];

bool reachedBy(int node, int root) {
    for (int i = 0; i < visits[node]; ++i) {
        if (visitorList[node][i] == root) return true;
    }
    return false;
}

int main() {
    scanf("%d %d %d %lld %d", &n, &m, &c, &r, &k);
    for (int i = 0; i < m; ++i) {
        int u, v, len;
        scanf("%d %d %d", &u, &v, &len);
        --u;
        --v;
        adj[u].push_back({v, len});
        adj[v].push_back({u, len});
    }

    // 把 C 个充电站全部作为起点塞进堆里，相当于同时跑 C 趟 Dijkstra；
    // 堆元素额外带上 root（它属于哪一趟），这样每个点才知道自己被哪些充电站访问过
    priority_queue<Pending> pq;
    for (int i = 0; i < c; ++i) {
        pq.push({0, i, i});
    }

    int wellConnected = 0;
    while (!pq.empty()) {
        Pending cur = pq.top();
        pq.pop();
        int u = cur.node;
        // 一个点被 k 个充电站访问过之后就不必再被访问了：
        // 后到的充电站到 u 的距离不小于前面 k 个，凡是能借 u 中转到的目的地，
        // 前面那 k 个充电站也能同样中转到达，因此 u 的后续访问不可能再"证明"新的目的地
        if (visits[u] == k || reachedBy(u, cur.root)) continue;
        visitorList[u][visits[u]] = cur.root;
        ++visits[u];
        if (u >= c && visits[u] == k) ++wellConnected;
        for (size_t i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i].to;
            long long nd = cur.dist + adj[u][i].len;
            // 只保留总长不超过 R 的路径（电车从充电站出发最远只能开 R 英里）
            if (nd <= r && visits[v] < k) pq.push({nd, v, cur.root});
        }
    }

    printf("%d\n", wellConnected);
    for (int i = c; i < n; ++i) {
        if (visits[i] == k) printf("%d\n", i + 1);
    }
    return 0;
}
