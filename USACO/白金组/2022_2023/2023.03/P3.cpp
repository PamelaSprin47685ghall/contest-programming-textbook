#include <cstdio>

using namespace std;

const int MAXN = 200005;

int n;
int head[MAXN], to[2 * MAXN], nxt[2 * MAXN], etot;  // 原树邻接表（边从 1 开始编号，0 表示空）
int chead[MAXN], cto[MAXN], cnxt[MAXN], ctot;      // 以 n 为根后，每个点的孩子链表
int par[MAXN];                                     // 每个点的父亲（以 n 为根）
int dsu[MAXN];                                     // 白点的并查集
long long mass[MAXN];                 // mass[u]：白点 u 正下方（相邻）黑点的个数
long long combinedMass[MAXN + 1];     // 黑点 x 所有孩子白点的 mass 之和
long long combinedCombinedMass[MAXN]; // 白点 a 下方“距离 2 的白点”的 mass 之和
long long answer = 0;

// 每个非根点 u 对应一个白点 u，代表朋友关系 (par[u], u)：
// 树上结构是 黑(par[u]) - 白(u) - 黑(u)。
// 两头牛是朋友 <=> 两个黑点都与同一个白点相邻，
// 于是“有序三元组 (a,b,c)”就是黑白黑白黑五元路径 (a,b,c,d,e)。
int find(int u) {
    int r = u;
    while (dsu[r] != r) r = dsu[r];
    while (dsu[u] != r) {
        int nx = dsu[u];
        dsu[u] = r;
        u = nx;
    }
    return r;
}

// 把白点 u 的 mass 加上 delta，并按增量修正答案。
// 五元路径按中间黑点 c 的位置分三类，这三类都会随 mass 变化而变：
//   一类：b = d，即 (mass+1)*mass*(mass-1)（黑邻居里任取三个不同点，中间那个当 c）；
//   二类：c 同时在 b、d 上方，即 2*Σ_{b<d 是 c 的孩子白点} mass[b]*mass[d]；
//   三类：b、d 里恰有一个在 c 上方，即 2*Σ_a mass[a]*combinedCombinedMass[a]。
void update(int u, long long delta) {
    int p = par[u];
    // 二类：u 与 p 的其它孩子白点配对
    answer += 2LL * (combinedMass[p] - mass[u]) * delta;
    combinedMass[p] += delta;

    if (p != n) {
        int a = find(p);  // 黑点 p 上方的白点
        // 三类：u 是白点 a 下方距离 2 的白点，与 a 配对
        answer += 2LL * mass[a] * delta;
        combinedCombinedMass[a] += delta;
    }

    // 三类：u 自己作为上方白点，与下方距离 2 的白点配对
    answer += 2LL * combinedCombinedMass[u] * delta;

    // 一类：u 的 mass 变了，它的贡献要重算
    answer -= (mass[u] + 1LL) * mass[u] * (mass[u] - 1LL);
    mass[u] += delta;
    answer += (mass[u] + 1LL) * mass[u] * (mass[u] - 1LL);
}

// 把白点 u 合并进它上方的白点（对应 par[u] 这头牛离开：它周围的友谊关系并成一个团）。
// 合并后白点 u 消失，它的黑孩子（牛 u 自己以及更下方那些）改挂到新白点下。
void mergeNode(int u) {
    int p = par[u];    // u 上方的黑点
    int a = find(p);   // 合并的目标：黑点 p 上方的白点
    combinedMass[p] -= mass[u];
    answer -= 2LL * combinedMass[p] * mass[u];
    combinedCombinedMass[a] -= mass[u];
    answer -= 2LL * mass[a] * mass[u];
    // u 作为上方白点的三类贡献搬到 a 上
    answer -= 2LL * mass[u] * combinedCombinedMass[u];
    answer += 2LL * mass[a] * combinedCombinedMass[u];
    combinedCombinedMass[a] += combinedCombinedMass[u];
    answer -= (mass[u] + 1LL) * mass[u] * (mass[u] - 1LL);
    update(a, mass[u]);  // 把 u 的黑孩子（连同 mass）交给 a
    dsu[u] = a;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n - 1; ++i) {
        int a, b;
        scanf("%d %d", &a, &b);
        ++etot;
        to[etot] = b;
        nxt[etot] = head[a];
        head[a] = etot;
        ++etot;
        to[etot] = a;
        nxt[etot] = head[b];
        head[b] = etot;
    }

    // 以牛 n 为根做 BFS，顺便建出孩子链表
    int queue_[MAXN];
    int qh = 0, qt = 0;
    par[n] = -1;
    queue_[qt++] = n;
    while (qh < qt) {
        int a = queue_[qh++];
        for (int e = head[a]; e; e = nxt[e]) {
            int b = to[e];
            if (b == par[a]) continue;
            par[b] = a;
            queue_[qt++] = b;
        }
    }
    for (int v = 1; v < n; ++v) {
        ++ctot;
        cto[ctot] = v;
        cnxt[ctot] = chead[par[v]];
        chead[par[v]] = ctot;
    }

    for (int a = 1; a < n; ++a) dsu[a] = a;
    // 一开始每个白点 u 正下方只有牛 u 自己
    for (int a = 1; a < n; ++a) update(a, 1);

    printf("%lld\n", answer);
    for (int a = 1; a < n; ++a) {
        // 牛 a 离开：它所有的白邻居先并成一个白点
        for (int e = chead[a]; e; e = cnxt[e]) mergeNode(cto[e]);
        // 再把牛 a 自己这个黑点删掉
        update(find(a), -1);
        printf("%lld\n", answer);
    }
    return 0;
}
