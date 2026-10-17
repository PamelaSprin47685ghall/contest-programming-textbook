#include <cstdio>
#include <queue>
#include <vector>

using namespace std;

const int MAXN = 40005;
const long long INF = 1LL << 60;

int fieldCount, edgeCount;
vector<int> adj[MAXN];

// 边权全部为 1，从起点出发做 BFS，首次访问某个点时走过的边数就是最短距离，
// 因此这里不需要 Dijkstra。返回下标 1..fieldCount 的数组，-1 表示到不了。
vector<int> bfs(int start) {
    vector<int> dist(fieldCount + 1, -1);
    queue<int> q;
    dist[start] = 0;
    q.push(start);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int i = 0; i < (int)adj[u].size(); ++i) {
            int v = adj[u][i];
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    return dist;
}

int main() {
    int bessieCost, elsieCost, piggyCost;
    scanf("%d %d %d %d %d", &bessieCost, &elsieCost, &piggyCost, &fieldCount, &edgeCount);
    for (int i = 0; i < edgeCount; ++i) {
        int a, b;
        scanf("%d %d", &a, &b);
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    // 两头牛的路程都可以拆成两段：到汇合点之前各走各的，汇合之后一起走。
    // 各自走的路一定是最短路，否则把其中一段换成最短路只会更省能量，
    // 所以只需要三组最短路：从贝茜起点 1、从埃尔茜起点 2、从谷仓 fieldCount。
    vector<int> distFromBessie = bfs(1);
    vector<int> distFromElsie = bfs(2);
    vector<int> distFromBarn = bfs(fieldCount);

    // 枚举汇合点，三段能量之和就是这种方案的总花费，取最小。
    // 两人不汇合等价于在谷仓汇合，已经包含在内。
    long long answer = INF;
    for (int meet = 1; meet <= fieldCount; ++meet) {
        if (distFromBessie[meet] < 0 || distFromElsie[meet] < 0 || distFromBarn[meet] < 0) {
            continue;  // 三处有一段不连通，这个点不能当汇合点
        }
        // 距离和系数都能到 4*10^4，乘积最坏约 4.8*10^9，超出 int 范围，必须用 long long。
        long long cost = (long long)distFromBessie[meet] * bessieCost +
                         (long long)distFromElsie[meet] * elsieCost +
                         (long long)distFromBarn[meet] * piggyCost;
        if (cost < answer) {
            answer = cost;
        }
    }
    printf("%lld\n", answer);
    return 0;
}
