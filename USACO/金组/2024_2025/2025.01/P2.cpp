#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 200005;

int N, M;
char s[MAXN];          // 输入的 01 串，下标 1..N
vector<int> adj[MAXN]; // 原图邻接表

int par[MAXN];  // 并查集：父指针
int sz[MAXN];   // 并查集：分量大小（按大小合并，保证树高）
int cnt[MAXN];  // cnt[root]：该分量里"在真实图中仍存在"的点数
bool on[MAXN];  // 反向过程中该点是否已加入隐式图
long long ans[MAXN];
long long pairs; // 当前所有"仍存在"的点之间的可达点对数

// 迭代式查找 + 路径压缩（避免 N=2e5 链上的递归爆栈）
int findRoot(int x) {
    int r = x;
    while (par[r] != r) {
        r = par[r];
    }
    while (par[x] != r) {
        int nx = par[x];
        par[x] = r;
        x = nx;
    }
    return r;
}

// 合并两个分量：新增的可达点对恰好是两分量内"仍存在"点数之积
void unite(int a, int b) {
    a = findRoot(a);
    b = findRoot(b);
    if (a == b) {
        return;
    }
    pairs += (long long)cnt[a] * cnt[b];
    if (sz[a] < sz[b]) { // 让 a 成为较大的根
        int t = a;
        a = b;
        b = t;
    }
    par[b] = a;
    sz[a] += sz[b];
    cnt[a] += cnt[b];
}

// 把点 v 放回图中：与所有已在图中的邻居相连（这些邻居在原图里与 v 相邻）
void activate(int v) {
    on[v] = true;
    for (size_t k = 0; k < adj[v].size(); ++k) {
        int u = adj[v][k];
        if (on[u]) {
            unite(v, u);
        }
    }
}

int main() {
    scanf("%d %d", &N, &M);
    scanf("%s", s + 1);
    for (int i = 0; i < M; ++i) {
        int u, v;
        scanf("%d %d", &u, &v);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for (int i = 1; i <= N; ++i) {
        par[i] = i;
        sz[i] = 1;
        cnt[i] = 0;
        on[i] = false;
    }

    // 关键转化：把 s[i]='1' 的点看成"只停用、不删边"。这样它的边一直有效，
    // 于是可以先把所有 '1' 点全部放进隐式图，它们之间以及和后来的点的连通性
    // 与正向过程结束时完全一致。
    for (int i = 1; i <= N; ++i) {
        if (s[i] == '1') {
            activate(i);
        }
    }

    // 正向过程只有删点/删边，倒着做就只剩加点/加边，适合并查集。
    // 处理 i 时，图中恰好有 i..N 这些"仍存在"的点（第 i 步之前的状态）。
    for (int i = N; i >= 1; --i) {
        if (s[i] == '0') {
            activate(i); // '0' 点当初是连边一起删掉的，这里把边恢复
        }
        // 点 i 本身在原图第 i 步之前也存在，把它计入分量
        int r = findRoot(i);
        pairs += cnt[r]; // 它与分量内每个已计入的点构成一对
        cnt[r] += 1;
        ans[i] = pairs;
    }

    for (int i = 1; i <= N; ++i) {
        printf("%lld\n", ans[i]);
    }
    return 0;
}
