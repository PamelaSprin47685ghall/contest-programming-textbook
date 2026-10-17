#include <cstdio>
#include <vector>
using namespace std;

const int MAXN = 200005;   // 谷仓数的上限

int n, m;
vector<int> adj[MAXN];     // 邻接表
int order[MAXN];           // order[i]: 第 i+1 个被关闭的谷仓 (0 起下标)
int par[MAXN], sz[MAXN];   // 并查集: 父节点与集合大小
bool opened[MAXN];         // 逆序处理时该谷仓是否已经"复活"
bool ans[MAXN];            // ans[t]: 关闭前 t 个谷仓后农场是否完全连通

int findRoot(int x) {
    while (par[x] != x) {
        par[x] = par[par[x]];   // 路径压缩
        x = par[x];
    }
    return x;
}

// 合并两个集合; 返回是否真的发生了合并
bool unite(int a, int b) {
    a = findRoot(a);
    b = findRoot(b);
    if (a == b) return false;
    if (sz[a] < sz[b]) {   // 按大小合并, 保证树高为对数级
        int t = a;
        a = b;
        b = t;
    }
    par[b] = a;
    sz[a] += sz[b];
    return true;
}

int main() {
    scanf("%d%d", &n, &m);
    for (int i = 0; i < m; i++) {
        int a, b;
        scanf("%d%d", &a, &b);
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    for (int i = 0; i < n; i++) {
        scanf("%d", &order[i]);
    }

    // 正向删除点/边难以维护, 于是把整个过程倒过来看: 谷仓按关闭顺序的逆序"复活",
    // 复活时与已复活的邻点合并。处理完 order[t..n-1] 后, 开通的恰好是
    // "关闭前 t 个谷仓之后"剩下的集合, 此刻连通块个数为 1 即为答案。
    int comps = 0;   // 当前开通谷仓的连通块个数
    for (int t = n - 1; t >= 0; t--) {
        int v = order[t];
        par[v] = v;
        sz[v] = 1;
        opened[v] = true;
        comps++;
        for (int i = 0; i < (int)adj[v].size(); i++) {
            int u = adj[v][i];
            // 只连已经复活的邻点: 还没复活的谷仓此刻并不存在
            if (opened[u] && unite(v, u)) comps--;
        }
        ans[t] = (comps == 1);
    }

    // 输出初始状态与关闭前 n-1 个谷仓之后的状态, 共 n 行
    for (int t = 0; t < n; t++) {
        printf("%s\n", ans[t] ? "YES" : "NO");
    }
    return 0;
}
