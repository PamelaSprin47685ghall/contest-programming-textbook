#include <algorithm>
#include <cstdio>
#include <map>
#include <set>
#include <utility>
#include <vector>

using namespace std;

const int MAXN = 200000;      // 田地数上限
const int MAXM = 200000;      // 道路数上限
const int INF = 1000000001;   // 比任何边权都大的"无穷大"（边权不超过 10^6）

struct Edge {
    int u, v, w;
};

bool byLength(const Edge &a, const Edge &b) {
    return a.w < b.w;
}

int n, m, k, q;
Edge edges[MAXM];
int uf[MAXN + 1];                        // 并查集
vector<pair<int, int> > mst[MAXN + 1];   // 最小生成树的邻接表：(邻居, 边权)
int color[MAXN + 1];                     // 每块田地当前的草种
int parent[MAXN + 1];                    // 生成树上每个点的父亲（1 号是根，父亲为 0）
int weightToParent[MAXN + 1];            // 每个点到父亲的边权

// 生成树以 1 为根后，每个点只需要留意自己的孩子：
// groups[u][c] 收集 "颜色为 c 的孩子 c' 与 u 之间那条边的边权"
// best[u] 则记录每个颜色组的当前最小边权，键为 (边权, 颜色)
// 那么 u 出发、连到颜色不同的孩子的最短边，就是 best[u] 里第一条颜色 != color[u] 的记录
vector<map<int, multiset<int> > > groups(MAXN + 1);
vector<multiset<pair<int, int> > > best(MAXN + 1);
multiset<pair<int, int> > globalBest;    // 所有点的该最小值，键为 (边权, 点)

int ufFind(int x) {
    while (uf[x] != x) {
        uf[x] = uf[uf[x]];   // 路径压缩
        x = uf[x];
    }
    return x;
}

// 点 u 对答案的贡献：从 u 连到某个颜色不同的孩子的边权最小值
int valueOf(int u) {
    if (best[u].empty()) return INF;
    multiset<pair<int, int> >::iterator it = best[u].begin();
    if (it->second == color[u]) ++it;   // 每种颜色在 best[u] 里只出现一次，跳过一条即可
    if (it == best[u].end()) return INF;
    return it->first;
}

// u 的贡献由 oldValue 变成了别的值，更新全局最小值集合
void refreshGlobal(int u, int oldValue) {
    globalBest.erase(globalBest.find(make_pair(oldValue, u)));
    globalBest.insert(make_pair(valueOf(u), u));
}

// 孩子 a 的颜色从 oldColor 变成 newColor，它在父亲 p 的各个结构里要跟着搬家
void moveChild(int p, int oldColor, int newColor, int w) {
    map<int, multiset<int> >::iterator oldIt = groups[p].find(oldColor);
    multiset<int> &oldSet = oldIt->second;
    int oldMin = *oldSet.begin();
    oldSet.erase(oldSet.find(w));
    if (oldSet.empty()) {
        groups[p].erase(oldIt);
        best[p].erase(make_pair(oldMin, oldColor));
    } else if (*oldSet.begin() != oldMin) {
        best[p].erase(make_pair(oldMin, oldColor));
        best[p].insert(make_pair(*oldSet.begin(), oldColor));
    }

    map<int, multiset<int> >::iterator newIt = groups[p].find(newColor);
    if (newIt == groups[p].end()) {
        groups[p][newColor].insert(w);
        best[p].insert(make_pair(w, newColor));
    } else {
        multiset<int> &newSet = newIt->second;
        int newMin = *newSet.begin();
        newSet.insert(w);
        if (w < newMin) {
            best[p].erase(make_pair(newMin, newColor));
            best[p].insert(make_pair(w, newColor));
        }
    }
}

int main() {
    scanf("%d %d %d %d", &n, &m, &k, &q);
    for (int i = 0; i < m; ++i)
        scanf("%d %d %d", &edges[i].u, &edges[i].v, &edges[i].w);

    // 最优的那条路径一定只有一条边（路径上必有两端颜色不同的边，它更短），
    // 而任何一条非树边都能在最小生成树里找到一条更短或等长的"颜色不同"的边替换它，
    // 所以只需保留最小生成树的边
    sort(edges, edges + m, byLength);
    for (int i = 1; i <= n; ++i) uf[i] = i;
    int treeEdges = 0;
    for (int i = 0; i < m && treeEdges < n - 1; ++i) {
        int ru = ufFind(edges[i].u), rv = ufFind(edges[i].v);
        if (ru == rv) continue;
        uf[ru] = rv;
        mst[edges[i].u].push_back(make_pair(edges[i].v, edges[i].w));
        mst[edges[i].v].push_back(make_pair(edges[i].u, edges[i].w));
        ++treeEdges;
    }

    // 把生成树以 1 号田地为根定向，得到父亲和到父亲的边权
    for (int i = 1; i <= n; ++i) parent[i] = -1;
    parent[1] = 0;
    int stack_[MAXN + 1];
    int top = 0;
    stack_[top++] = 1;
    while (top > 0) {
        int u = stack_[--top];
        for (int i = 0; i < (int)mst[u].size(); ++i) {
            int v = mst[u][i].first, w = mst[u][i].second;
            if (v == parent[u]) continue;
            parent[v] = u;
            weightToParent[v] = w;
            stack_[top++] = v;
        }
    }

    for (int i = 1; i <= n; ++i) scanf("%d", &color[i]);
    for (int u = 1; u <= n; ++u) {
        for (int i = 0; i < (int)mst[u].size(); ++i) {
            int v = mst[u][i].first;
            if (parent[v] == u) groups[u][color[v]].insert(mst[u][i].second);   // v 是 u 的孩子
        }
        for (map<int, multiset<int> >::iterator it = groups[u].begin(); it != groups[u].end(); ++it)
            best[u].insert(make_pair(*it->second.begin(), it->first));
    }
    for (int u = 1; u <= n; ++u) globalBest.insert(make_pair(valueOf(u), u));

    for (int i = 0; i < q; ++i) {
        int a, b;
        scanf("%d %d", &a, &b);
        int oldColor = color[a];
        if (oldColor != b) {
            int oldValueA = valueOf(a);   // 记录 a 自己改色前的贡献
            int p = parent[a];
            if (p != 0) {
                int oldValueP = valueOf(p);   // 父亲 p 的贡献也可能变
                moveChild(p, oldColor, b, weightToParent[a]);
                refreshGlobal(p, oldValueP);
            }
            color[a] = b;
            refreshGlobal(a, oldValueA);
        }
        printf("%d\n", globalBest.begin()->first);
    }
    return 0;
}
