// USACO 2016 US Open Silver P3 - Closing the Farm
// 思路：直接模拟「删点求连通」很难维护，于是把关闭顺序倒过来看：从空农场出发逐个「重新开放」谷仓，
// 每加入一个谷仓，就把它与已经开放（即在关闭顺序中排在它后面）的邻居合并。这正好是并查集擅长的
// 加边操作。用 comps 维护当前连通块个数，加入每个谷仓时先 +1，每成功合并一次 -1，则
// comps <= 1 就等价于「剩下的谷仓两两可达」。
#include <cstdio>
#include <vector>
using namespace std;

const int MAXN = 3005;
int n, m;
vector<int> adj[MAXN];
int order[MAXN];          // 关闭顺序
int par[MAXN];
bool open_[MAXN];         // 该谷仓此刻是否已经（重新）开放
bool ans[MAXN];           // ans[i]：只剩 order[i..n-1] 这些谷仓时的连通性

int findRoot(int x) {
    while (par[x] != x) {
        par[x] = par[par[x]];  // 路径压缩
        x = par[x];
    }
    return x;
}

int main() {
    scanf("%d%d", &n, &m);
    for (int i = 0; i < m; i++) {
        int u, v;
        scanf("%d%d", &u, &v);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for (int i = 0; i < n; i++) scanf("%d", &order[i]);
    for (int i = 1; i <= n; i++) par[i] = i;

    int comps = 0;
    // 逆序处理：i 越小代表越早的时刻（i = 0 就是最初全部谷仓都开着的状态）
    for (int i = n - 1; i >= 0; i--) {
        int u = order[i];
        open_[u] = true;
        comps++;  // 新加入的谷仓自己先算一个连通块
        for (int j = 0; j < (int)adj[u].size(); j++) {
            int v = adj[u][j];
            if (!open_[v]) continue;  // 只和已开放的邻居相连
            int a = findRoot(u), b = findRoot(v);
            if (a != b) {
                par[a] = b;
                comps--;
            }
        }
        ans[i] = (comps <= 1);
    }

    for (int i = 0; i < n; i++) printf("%s\n", ans[i] ? "YES" : "NO");
    return 0;
}
