#include <cstdio>

using namespace std;

const int MAXN = 100005;
const int LOG = 17;  // 2^17 > 100000, 足够跳完任何一条到根的路径

int head[MAXN], nxt[2 * MAXN], to[2 * MAXN], edgeCnt = 0;

void addEdge(int u, int v) {
    to[edgeCnt] = v;
    nxt[edgeCnt] = head[u];
    head[u] = edgeCnt;
    edgeCnt++;
}

int n;
int parent[MAXN];      // 根的父亲指向自己, 倍增上跳时不会跳出树
int depth[MAXN];
int tin[MAXN];         // 先序 DFS 序: 每棵子树对应序列上一段连续区间
int subSize[MAXN];
int order[MAXN];       // 先序序列本身, 逆序累加子树大小用
int up[LOG][MAXN];     // 倍增祖先
int bit[MAXN];         // 差分点加, 区间求和

// 在 DFS 序位置 i 上加 v
void bitAdd(int i, int v) {
    for (; i <= n; i += i & -i) {
        bit[i] += v;
    }
}

// DFS 序位置 1..i 的前缀和
int bitSum(int i) {
    int s = 0;
    for (; i > 0; i -= i & -i) {
        s += bit[i];
    }
    return s;
}

int lca(int a, int b) {
    if (depth[a] < depth[b]) {
        int t = a;
        a = b;
        b = t;
    }
    int diff = depth[a] - depth[b];
    for (int k = 0; k < LOG; k++) {
        if ((diff >> k) & 1) {
            a = up[k][a];
        }
    }
    if (a == b) {
        return a;
    }
    for (int k = LOG - 1; k >= 0; k--) {
        if (up[k][a] != up[k][b]) {
            a = up[k][a];
            b = up[k][b];
        }
    }
    return parent[a];
}

int main() {
    int m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) {
        head[i] = -1;
    }
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        scanf("%d%d", &u, &v);
        addEdge(u, v);
        addEdge(v, u);
    }

    // 迭代式先序遍历. 根先入栈, 每个点出栈时把儿子压栈;
    // 这样产生的先序序列里, 每棵子树仍然是一段连续区间
    int st[MAXN];
    int top = 0;
    parent[1] = 1;
    depth[1] = 0;
    st[top++] = 1;
    int timer = 0;
    while (top > 0) {
        int u = st[--top];
        timer++;
        tin[u] = timer;
        order[timer] = u;
        for (int e = head[u]; e != -1; e = nxt[e]) {
            int v = to[e];
            if (v == parent[u]) {
                continue;  // 不走回父亲
            }
            parent[v] = u;
            depth[v] = depth[u] + 1;
            st[top++] = v;
        }
    }

    // 逆先序累加子树大小: 轮到某个点时, 它的所有子孙都已经加到自己身上
    for (int v = 1; v <= n; v++) {
        subSize[v] = 1;
    }
    for (int i = n; i >= 2; i--) {
        int u = order[i];
        subSize[parent[u]] += subSize[u];
    }

    for (int v = 1; v <= n; v++) {
        up[0][v] = parent[v];
    }
    for (int k = 1; k < LOG; k++) {
        for (int v = 1; v <= n; v++) {
            up[k][v] = up[k - 1][up[k - 1][v]];
        }
    }

    for (int q = 0; q < m; q++) {
        char type;
        int a, b;
        scanf(" %c %d %d", &type, &a, &b);
        int l = lca(a, b);
        if (type == 'P') {
            // 记边 (x 的父亲, x) 为"边 x". 对一条边 x 来说, 路径 a-b 经过它
            // 当且仅当 a, b 中恰有一个落在子树 x 里. 于是在子树 x 上求和
            // [a 在子树内] + [b 在子树内] - 2 * [lca 在子树内] 就是答案;
            // 而"某个点落在子树 x 内"等价于该点落在先序序列的一段区间里,
            // 所以把 +1, +1, -2 打在三个点上, 查询时做区间求和即可.
            bitAdd(tin[a], 1);
            bitAdd(tin[b], 1);
            bitAdd(tin[l], -2);
        } else {
            // 询问边 x: a, b 相邻, 取较深的那个作为 x
            int x = (depth[a] > depth[b]) ? a : b;
            int lo = tin[x];
            int hi = tin[x] + subSize[x] - 1;
            printf("%d\n", bitSum(hi) - bitSum(lo - 1));
        }
    }
    return 0;
}
