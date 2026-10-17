#include <cstdio>
#include <vector>

int main() {
    int n;
    scanf("%d", &n);

    // outDegree[i]: 站点 i 的出度 (以此为起点的单向传送带数量)
    std::vector<int> outDegree(n + 1, 0);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        outDegree[u]++;
    }

    // 在由 N-1 条边构成的连通有向无环图中, 能从所有其他站点到达的终点, 其出度必须为 0.
    // 且当且仅当恰好有且仅有 1 个站点的出度为 0 时, 该站点为唯一汇点.
    int sink = -1;
    for (int i = 1; i <= n; ++i) {
        if (outDegree[i] == 0) {
            if (sink != -1) {
                // 存在多个出度为 0 的站点, 彼此无法互相到达
                printf("-1\n");
                return 0;
            }
            sink = i;
        }
    }

    printf("%d\n", sink);
    return 0;
}
