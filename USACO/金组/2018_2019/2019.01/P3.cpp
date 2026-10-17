#include <cstdio>
#include <vector>
#include <queue>
#include <utility>
#include <functional>

using namespace std;

const int MAXN = 10005;
const long long INF = 1LL << 60;

struct Edge {
    int to;
    int w;
};

int n, m;
long long shortcut; // 近道的通行时间 T
long long cows[MAXN]; // 每个农场的奶牛数
vector<Edge> adj[MAXN];

long long dist[MAXN];   // dist[v]: v 到谷仓(1 号农场)的最短时间
int nxt[MAXN];          // 最短路树上 v 朝谷仓方向的下一跳
long long through[MAXN];// 需要经过农场 v 的奶牛数(含 v 自己的牛)
vector<int> child[MAXN];
int order[MAXN]; // 最短路树的遍历顺序
int stk[MAXN];   // 手工栈, 避免递归

int main() {
    if (scanf("%d %d %lld", &n, &m, &shortcut) != 3) return 0;
    for (int i = 1; i <= n; i++) scanf("%lld", &cows[i]);
    for (int i = 0; i < m; i++) {
        int a, b, t;
        scanf("%d %d %d", &a, &b, &t);
        Edge e1 = {b, t}, e2 = {a, t};
        adj[a].push_back(e1);
        adj[b].push_back(e2);
    }

    // Dijkstra: 以谷仓 1 为源点求最短路
    for (int i = 1; i <= n; i++) dist[i] = INF;
    dist[1] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int> >,
                   greater<pair<long long, int> > > pq;
    pq.push(make_pair(0LL, 1));
    while (!pq.empty()) {
        pair<long long, int> cur = pq.top();
        pq.pop();
        int u = cur.second;
        if (cur.first > dist[u]) continue;
        for (size_t i = 0; i < adj[u].size(); i++) {
            int v = adj[u][i].to;
            long long nd = cur.first + adj[u][i].w;
            if (nd < dist[v]) {
                dist[v] = nd;
                pq.push(make_pair(nd, v));
            }
        }
    }

    // 并列最短路时奶牛取"字典序最小"的那条: 从起点出发逐头比较农场编号.
    // 于是下一跳直接取满足 dist[u] + w == dist[v] 的编号最小的邻居即可
    // (第一条边就分出大小, 后面的部分再按同样的规则递归)
    for (int v = 2; v <= n; v++) {
        int best = 0;
        for (size_t i = 0; i < adj[v].size(); i++) {
            int u = adj[v][i].to;
            if (dist[u] + adj[v][i].w == dist[v] && (best == 0 || u < best)) best = u;
        }
        nxt[v] = best;
        child[best].push_back(v);
    }

    // 奶牛 x 沿 x -> nxt[x] -> ... -> 1 走, 所以"经过 v 的牛"就是最短路树上 v 子树的牛
    int total = 0, top = 0;
    stk[top++] = 1;
    while (top > 0) {
        int u = stk[--top];
        order[total++] = u;
        for (size_t i = 0; i < child[u].size(); i++) stk[top++] = child[u][i];
    }
    for (int i = total - 1; i >= 0; i--) {
        int u = order[i];
        through[u] += cows[u];
        if (u != 1) through[nxt[u]] += through[u];
    }

    // 把近道修到 v: 只有原本就要经过 v 的牛会顺便走它, 每头省 dist[v]-shortcut;
    // 花的时间反而更多(或持平)时奶牛仍走原路, 相当于省 0
    long long best = 0;
    for (int v = 2; v <= n; v++) {
        if (dist[v] <= shortcut) continue;
        long long save = through[v] * (dist[v] - shortcut);
        if (save > best) best = save;
    }
    printf("%lld\n", best);
    return 0;
}
