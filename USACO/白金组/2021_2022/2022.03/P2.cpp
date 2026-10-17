#include <cstdio>
#include <vector>
#include <unordered_set>
using namespace std;

const int MAXN = 100005;

int par[MAXN];
vector<unordered_set<int> > adj;  // adj[u]：u 的出边（元素可能是已合并节点的旧编号，用 find 还原）
vector<unordered_set<int> > rev;  // rev[u]：u 的入边，合并时小并大
vector<int> stk;
char ans[MAXN];

// 带路径压缩的并查集；被淘汰（能走到死路）的节点令 par=0，于是 find 返回 0 表示"必败"
int findRoot(int u) {
    while (par[u] != par[par[u]]) par[u] = par[par[u]];
    return par[u];
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    adj.assign(n + 1, unordered_set<int>());
    rev.assign(n + 1, unordered_set<int>());
    for (int i = 0; i <= n; i++) par[i] = i;
    for (int i = 0; i < m; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        adj[a].insert(b);
        rev[b].insert(a);
    }

    // 第一步：反复删掉出度为 0 的点。被删掉的点意味着它的一切走法最终都会撞进死路，
    // 于是只要某一枚棋子落在这样的点上，brain 必胜。
    for (int a = 1; a <= n; a++) {
        if (adj[a].empty()) stk.push_back(a);
    }
    while (!stk.empty()) {
        int a = stk.back();
        stk.pop_back();
        par[a] = 0;
        for (unordered_set<int>::iterator it = rev[a].begin(); it != rev[a].end(); ++it) {
            int b = *it;
            adj[b].erase(a);
            if (adj[b].empty()) stk.push_back(b);
        }
    }

    // 第二步：出度恰为 1 的点 u（唯一出边 u->v）可以和 v 合并成一个点：
    // 若 brain 能把两枚棋子逼到 u，那么让每枚棋子再走一步就能逼到 v，两种局面等价。
    // 合并时把入边集合小的并进大的，每条边因此至多被搬动 O(log N) 次。
    for (int a = 1; a <= n; a++) {
        if (adj[a].size() == 1) stk.push_back(a);
    }
    while (!stk.empty()) {
        int a = stk.back();
        stk.pop_back();
        int c = 0;
        for (unordered_set<int>::iterator it = adj[a].begin(); it != adj[a].end(); ++it) {
            c = *it;
        }
        a = findRoot(a);
        c = findRoot(c);
        if (a != c) {
            if (rev[a].size() > rev[c].size()) {
                int tmp = a;
                a = c;
                c = tmp;
            }
            for (unordered_set<int>::iterator it = rev[a].begin(); it != rev[a].end(); ++it) {
                int b = *it;
                adj[b].erase(a);
                adj[b].insert(c);
                if (adj[b].size() == 1) {
                    // b 现在只剩一条出边（指向 c），入边先记在 c 上会重复，等 b 自己被合并时再处理
                    rev[c].erase(b);
                    stk.push_back(b);
                } else {
                    rev[c].insert(b);
                }
            }
            par[a] = c;
        }
    }

    int q;
    scanf("%d", &q);
    for (int i = 0; i < q; i++) {
        int x, y;
        scanf("%d %d", &x, &y);
        int u = findRoot(x);
        int v = findRoot(y);
        if (u == 0 || v == 0 || u == v) ans[i] = 'B';
        else ans[i] = 'H';
    }
    ans[q] = '\0';
    printf("%s\n", ans);
    return 0;
}
