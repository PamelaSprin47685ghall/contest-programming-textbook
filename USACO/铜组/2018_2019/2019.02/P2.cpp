#include <cstdio>
#include <vector>

using namespace std;

int main() {
    int n, m;
    scanf("%d%d", &n, &m);

    // 邻接表记录牧场间的相连关系 (1-based)
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // color[i]: 牧场 i 种植的草种编号 (1..4)
    vector<int> color(n + 1, 0);

    for (int i = 1; i <= n; ++i) {
        bool used[5] = {};
        for (int neighbor : adj[i]) {
            if (color[neighbor] != 0) {
                used[color[neighbor]] = true;
            }
        }
        // 贪心选取编号最小的可用草种 1..4
        for (int c = 1; c <= 4; ++c) {
            if (!used[c]) {
                color[i] = c;
                break;
            }
        }
    }

    for (int i = 1; i <= n; ++i) {
        printf("%d", color[i]);
    }
    printf("\n");
    return 0;
}
