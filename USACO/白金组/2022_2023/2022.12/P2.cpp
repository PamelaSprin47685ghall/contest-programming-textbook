#include <cstdio>
#include <set>

using namespace std;

const int MAXN = 200005;

// adj[u]：与 u 为朋友、且编号比 u 大的那些牛。
// 编号比 u 小的牛一定在 u 离场之前就离开了，任何时候都用不到它们。
set<int> adj[MAXN];

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int t = 0; t < m; ++t) {
        int u, v;
        scanf("%d%d", &u, &v);
        if (u > v) { int tmp = u; u = v; v = tmp; }
        adj[u].insert(v); // 一对朋友只存一份（小编号认领）
    }

    // 答案 = Σ_i |adj[i]| - M：
    // 每对"最终成为朋友"的牛 (u, v)（u < v）都恰好被 |adj[u]| 统计一次——
    // 它一定在 u 离场之前（也就是 u 还活着的时候）被建立，
    // 而初始就存在的 M 对被多算了一遍，减掉即可。
    long long total = -(long long)m;
    for (int i = 1; i <= n; ++i) {
        total += (long long)adj[i].size();
        if (!adj[i].empty()) {
            // 牛 i 离场时，只需要把它的朋友里编号最小的 j 与其余朋友相连；
            // 其余朋友之间的边会在它们当中编号较小者离场时由同一规则补上，
            // 所以不必真的两两相连——这保证了总代价可以套用"小的并进大的"摊还分析。
            int j = *adj[i].begin();
            adj[i].erase(j);
            if (adj[i].size() > adj[j].size()) adj[i].swap(adj[j]); // 交换后 adj[i] 是较短的一份
            for (set<int>::iterator it = adj[i].begin(); it != adj[i].end(); ++it)
                adj[j].insert(*it);
            adj[i].clear(); // i 已经离场，这份列表之后不会再被访问
        }
    }
    printf("%lld\n", total);
    return 0;
}
