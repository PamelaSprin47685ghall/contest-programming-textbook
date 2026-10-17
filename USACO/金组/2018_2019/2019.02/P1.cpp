#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 100005;
const int LOG = 18;      // 2^17 > 10^5, 足够表示任意祖先跳跃

int n, q;
int enjoy[MAXN];         // 每个景点的娱乐值 e_i
vector<int> edge[MAXN];

int depth[MAXN];         // 深度(根为 1)
int up[MAXN][LOG];       // 倍增祖先, up[v][d] 是 v 的第 2^d 级祖先(根的父亲记为 0)
int tin[MAXN], tout[MAXN];   // DFS 序: 子树 v 对应区间 [tin[v], tout[v]]
int bitBase[MAXN];       // bitBase[p] = 根到 DFS 序第 p 个点的路径异或(初始值)
int bit[MAXN];           // 差分数组的树状数组, 支持"区间异或 + 单点询问"
int rootXor[MAXN];       // 每个结点到根的路径异或(DFS 过程中求出)

// 树状数组: 单点异或
void bitUpdate(int pos, int val) {
    for (int i = pos; i <= n; i += i & -i) bit[i] ^= val;
}

// 树状数组: 前缀异或(即第 pos 个位置当前的取值)
int bitQuery(int pos) {
    int res = 0;
    for (int i = pos; i > 0; i -= i & -i) res ^= bit[i];
    return res;
}

int lca(int a, int b) {
    if (depth[a] < depth[b]) {
        int t = a;
        a = b;
        b = t;
    }
    // 先把较深的点抬到同一深度
    for (int d = LOG - 1; d >= 0; d--)
        if (depth[a] - (1 << d) >= depth[b]) a = up[a][d];
    if (a == b) return a;
    // 一起往上跳, 停在 LCA 的下一层
    for (int d = LOG - 1; d >= 0; d--)
        if (up[a][d] != up[b][d]) {
            a = up[a][d];
            b = up[b][d];
        }
    return up[a][0];
}

int main() {
    scanf("%d %d", &n, &q);
    for (int i = 1; i <= n; i++) scanf("%d", &enjoy[i]);
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        edge[a].push_back(b);
        edge[b].push_back(a);
    }

    // 迭代式 DFS(避免 10^5 深度的递归爆栈), 同时求出深度、倍增祖先和 DFS 序
    int dfsStack[MAXN];      // 手工栈
    int edgeIter[MAXN] = {0};    // 每个结点已经枚举到第几条出边
    int top = 0;
    int timer = 0;
    depth[1] = 0;
    up[1][0] = 0;
    rootXor[0] = 0;
    dfsStack[++top] = 1;
    while (top > 0) {
        int u = dfsStack[top];
        if (edgeIter[u] == 0) {
            // 第一次走到 u: 记录 DFS 序与根到 u 的路径异或
            timer++;
            tin[u] = timer;
            rootXor[u] = rootXor[up[u][0]] ^ enjoy[u];
            bitBase[timer] = rootXor[u];
        }
        if (edgeIter[u] < (int)edge[u].size()) {
            int v = edge[u][edgeIter[u]++];
            if (v == up[u][0]) continue;   // 不回头走父亲
            up[v][0] = u;
            depth[v] = depth[u] + 1;
            dfsStack[++top] = v;
        } else {
            tout[u] = timer;   // 子树 u 的区间在此处闭合
            top--;
        }
    }
    for (int d = 1; d < LOG; d++)
        for (int v = 1; v <= n; v++) up[v][d] = up[up[v][d - 1]][d - 1];

    // 把"根到各点的路径异或"用差分形式装进树状数组:
    // 修改一个点的值会让它整棵子树的路径异或都改变 delta, 于是变成区间异或 + 单点询问
    for (int p = 1; p <= n; p++) bit[p] ^= bitBase[p] ^ bitBase[p - 1];
    for (int p = 1; p <= n; p++) {
        int nxt = p + (p & -p);
        if (nxt <= n) bit[nxt] ^= bit[p];
    }

    for (int i = 0; i < q; i++) {
        int type, a, b;
        scanf("%d %d %d", &type, &a, &b);
        if (type == 1) {
            int delta = enjoy[a] ^ b;
            enjoy[a] = b;
            if (delta == 0) continue;
            bitUpdate(tin[a], delta);
            if (tout[a] + 1 <= n) bitUpdate(tout[a] + 1, delta);
        } else {
            // 路径异或 = 根到 a 的异或 ^ 根到 b 的异或 ^ e[lca]
            // (lca 在两条根路径里各出现一次, 异或后抵消, 需要单独补回来)
            int r = lca(a, b);
            printf("%d\n", bitQuery(tin[a]) ^ bitQuery(tin[b]) ^ enjoy[r]);
        }
    }
    return 0;
}
