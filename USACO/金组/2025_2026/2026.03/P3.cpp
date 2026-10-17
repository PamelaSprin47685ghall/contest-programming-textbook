// P3 Random Tree Generation
// 给定重标号后的树，枚举第一步的根 r（原编号 1 那个点）：条件等价于
// "每个点都排在它到 r 的路径上的父亲之后"，这样的排列只有 (N-1)! * prod(1/sz_r) 个。
// 又第一步生成该树的概率是 1/(N-1)!，两者相乘得 q(r) = prod_w 1/sz_r(w)（以 r 为根时所有子树大小倒数之积）。
// 所以答案为 (sum_r q(r)) / (N-1)!。
// 先做一次 DFS 求出根处的 q，再用换根公式 q(孩子) = q(父亲) * sz(孩子) / (N - sz(孩子)) 推出所有点，
// 除法一律改成乘预处理好的逆元。所有遍历都用迭代写，避免 N 达到 2e5 时爆栈。
#include <cstdio>

using namespace std;

const long long MOD = 1000000007;

const int MAXN = 200005;

int n;
int head[MAXN], nxt[2 * MAXN], to[2 * MAXN], ecnt;
int par[MAXN], order[MAXN];   // BFS 序：父亲一定排在孩子前面
int sz[MAXN];
long long inv[MAXN];
long long dp[MAXN];           // dp[v]：以 v 为根时所有点子树大小倒数之积

void addEdge(int u, int v) {
    ecnt++;
    to[ecnt] = v;
    nxt[ecnt] = head[u];
    head[u] = ecnt;
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        ecnt = 0;
        for (int i = 1; i <= n; i++) head[i] = 0;
        for (int i = 0; i < n - 1; i++) {
            int u, v;
            scanf("%d %d", &u, &v);
            addEdge(u, v);
            addEdge(v, u);
        }

        // 逆元只依赖 MOD：inv[i] = -(MOD/i) * inv[MOD % i]
        inv[1] = 1;
        for (int i = 2; i <= n; i++)
            inv[i] = (MOD - MOD / i) * inv[MOD % i] % MOD;

        // 以 1 为初始根做 BFS，得到父子关系与拓扑序
        int qh = 0, qt = 0;
        par[1] = 0;
        order[qt++] = 1;
        while (qh < qt) {
            int u = order[qh++];
            for (int e = head[u]; e; e = nxt[e]) {
                int v = to[e];
                if (v == par[u]) continue;
                par[v] = u;
                order[qt++] = v;
            }
        }

        // 逆序处理：轮到 v 时它的子树已经全部算完，先把 q(v) 定型（乘上 1/sz[v]），再并进父亲
        for (int i = 1; i <= n; i++) {
            sz[i] = 1;
            dp[i] = 1;
        }
        for (int i = n - 1; i >= 0; i--) {
            int v = order[i];
            dp[v] = dp[v] * inv[sz[v]] % MOD;
            int p = par[v];
            if (p) {
                sz[p] += sz[v];
                dp[p] = dp[p] * dp[v] % MOD;
            }
        }

        // 换根：相邻两点为根时的乘积只差两个因子
        for (int i = 0; i < n; i++) {
            int u = order[i];
            for (int e = head[u]; e; e = nxt[e]) {
                int v = to[e];
                if (par[v] != u) continue;
                dp[v] = dp[u] * sz[v] % MOD * inv[n - sz[v]] % MOD;
            }
        }

        long long ways = 0;
        for (int i = 1; i <= n; i++) ways = (ways + dp[i]) % MOD;
        // 除以 (N-1)!，即乘上 1!/1 * ... 的乘积（inv 的累乘）
        for (int i = 1; i < n; i++) ways = ways * inv[i] % MOD;
        printf("%lld\n", ways);
    }
    return 0;
}
