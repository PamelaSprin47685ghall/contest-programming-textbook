// USACO 2025 US Open Contest, Platinum, Problem 3: Package Pickup
// 做法：把数轴按长度 M 分成若干"块"，块内所有牛/包裹的位置都由各区间左端点 mod M 决定。
// 状态压缩成 5 个，相邻事件之间是"距离 × 单价"的线性代价，于是整段转移可以写成 5×5 的 min-plus 矩阵；
// 连续若干内容相同的块用矩阵快速幂跳过。线段树按块内名次维护当前块的点，支持区间矩阵合并查询。
#include <cstdio>
#include <algorithm>
using namespace std;

typedef long long ll;

const int MAXT = 40000;                 // n + p 的上限
const int MAXS = 2 * MAXT;              // 叶子数上限（每个区间在块内贡献两个偏移副本）
const ll INF = 4000000000000000000LL;   // 4e18：大于任何合法答案（不超过 2e18），且两两相加不溢出

// 状态含义（沿用官方分析）：0/1 表示某头牛"已经向右走到"当前点，单价 1/2；
// 2/3 表示某头牛"将来会向左走到"当前点，单价 1/2；4 表示当前没有任何进行中的路径。

struct Ev {
    ll blk;     // 所在块号
    int rank;   // 块内名次（按偏移排序后的下标）
    int kind;   // 0 = 包裹点，1 = 奶牛点，2 = 区间结束（等价于无事件）
    bool operator<(const Ev &o) const {
        if (blk != o.blk) return blk < o.blk;
        if (rank != o.rank) return rank < o.rank;
        return kind < o.kind;
    }
};

int n, p, S;
ll m;
ll lo[MAXT], hi[MAXT];                  // 前 n 个是奶牛区间，后 p 个是包裹区间
pair<ll, int> ps[MAXS + 1];             // (块内偏移, 区间下标)：排序后给出块内名次
Ev ev[MAXS + 1];                        // 2(n+p)+1 个事件（含起点哨兵）
ll tree[2 * MAXS + 1][5][5];            // 线段树：行 = 区间左端状态，列 = 区间右端状态

// “先走完 a 再走完 b”的 min-plus 合成：c[i][j] = min_k a[i][k] + b[k][j]
// 允许 c 与 a/b 相同（内部先存到临时数组）
void mul(const ll a[5][5], const ll b[5][5], ll c[5][5]) {
    ll t[5][5];
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            ll best = INF;
            for (int k = 0; k < 5; k++) {
                ll v = a[i][k] + b[k][j];
                if (v > INF) v = INF;   // 饱和处理：不可达的项始终留在 INF，避免反复相加把值放大到溢出
                if (v < best) best = v;
            }
            t[i][j] = best;
        }
    }
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++) c[i][j] = t[i][j];
}

// 构造块内名次为 idx 的叶子的矩阵：先在当前点上做状态切换，再付走到下一个点的路程费
void buildLeaf(ll mat[5][5], int idx, int kind) {
    ll gap = ps[idx + 1].first - ps[idx].first;   // 到块内下一个点的距离（最后一片叶子取到值 0，但永不被查询）
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++) mat[i][j] = INF;
    if (kind == 0) {
        // 包裹点：当前点必须被覆盖，牛可以在这里继续向右走（0→0、1→1）或在此结束（→4）；
        // 也可以从"空闲"开始一段将来向左走的路径（4→2、4→3）
        mat[0][4] = mat[0][0] = mat[1][4] = mat[1][1] = 0;
        mat[4][2] = mat[2][2] = mat[4][3] = mat[3][3] = 0;
    } else if (kind == 1) {
        // 奶牛点：路径的起点或终点。向右走的路径在此换向：左段单价 1 则右段单价 2（反之亦然）；
        // 4→2 表示左段为空（只向右走），0→4 表示路径到此结束
        mat[4][4] = mat[2][1] = mat[3][0] = mat[2][4] = mat[4][0] = 0;
    } else {
        // 无事件：状态不变，只是越过这段距离
        for (int i = 0; i < 5; i++) mat[i][i] = 0;
    }
    // 跨越间隙的单价由"到达该间隙后所处的状态"（列）决定：状态 0/2 单倍，1/3 双倍，4 不计费
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 4; j++)
            if (mat[i][j] < INF) mat[i][j] += gap * (1 + (j & 1));
}

// 把名次 idx 处的叶子重置为 kind 类型并向上更新
void upd(int idx, int kind) {
    buildLeaf(tree[S + idx], idx, kind);
    for (int v = (S + idx) >> 1; v >= 1; v >>= 1)
        mul(tree[2 * v], tree[2 * v + 1], tree[v]);
}

// 查询名次区间 [l, r]（闭区间）的合成矩阵
void qry(int l, int r, ll res[5][5]) {
    ll lft[5][5], rgt[5][5];
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++) lft[i][j] = rgt[i][j] = (i == j ? 0 : INF);
    for (int a = l + S, b = r + S + 1; a < b; a >>= 1, b >>= 1) {
        if (a & 1) { mul(lft, tree[a], lft); a++; }
        if (b & 1) { b--; mul(tree[b], rgt, rgt); }
    }
    mul(lft, rgt, res);
}

int main() {
    scanf("%lld %d %d", &m, &n, &p);
    int t = n + p;
    for (int i = 0; i < t; i++) scanf("%lld %lld", &lo[i], &hi[i]);

    // 每个区间 [L, R] 在它覆盖的每一块里恰好放一个点，块内偏移为 L mod M；
    // 于是块内所有点的相对次序只由偏移决定，排序后即得到"名次"。
    for (int i = 0; i < t; i++) ps[i] = make_pair(lo[i] % m, i);
    sort(ps, ps + t);
    for (int i = 0; i < t; i++) ps[t + i] = make_pair(ps[i].first + m, ps[i].second);

    // 事件：区间从 L/M 块开始在块内偏移处分出现，到 R/M + 1 块不再出现；
    // 名次 i 与 i + t 是同一偏移在相邻两块中的两份副本，查询时靠 t 的偏移完成"绕块一圈"。
    for (int i = 0; i < t; i++) {
        int id = ps[i].second;
        ev[2 * i].blk = lo[id] / m;       ev[2 * i].rank = i;     ev[2 * i].kind = (id < n ? 1 : 0);
        ev[2 * i + 1].blk = hi[id] / m + 1; ev[2 * i + 1].rank = i; ev[2 * i + 1].kind = 2;
    }
    ev[2 * t].blk = -1; ev[2 * t].rank = 0; ev[2 * t].kind = 0;   // 起点哨兵：从第 -1 块的起点出发
    sort(ev, ev + 2 * t + 1);

    S = 2 * t;
    for (int i = 0; i < S; i++) buildLeaf(tree[S + i], i, 2);
    for (int i = S - 1; i >= 1; i--) mul(tree[2 * i], tree[2 * i + 1], tree[i]);

    ll ans[5][5];
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++) ans[i][j] = (i == 4 && j == 4) ? 0 : INF;

    for (int i = 1; i <= 2 * t; i++) {
        // 从上一个事件位置走到当前事件位置：同一块内按名次前进，跨块时多绕一份副本
        int wrap = (ev[i - 1].blk < ev[i].blk) ? t : 0;
        ll seg[5][5];
        qry(ev[i - 1].rank, ev[i].rank - 1 + wrap, seg);
        mul(ans, seg, ans);
        // 两个事件之间可能还有若干内容完全相同的完整块，用快速幂一次跳过
        ll skip = ev[i].blk - ev[i - 1].blk - 1;
        if (skip > 0) {
            ll b[5][5];
            qry(ev[i].rank, ev[i].rank - 1 + t, b);   // 一整个块（从名次 rank 出发绕回同一名次）的转移
            while (skip) {
                if (skip & 1) mul(ans, b, ans);
                skip >>= 1;
                if (skip) mul(b, b, b);
            }
        }
        // 当前块的事件在"穿过该块之后"才生效，所以先查询再更新
        upd(ev[i].rank, ev[i].kind);
        upd(ev[i].rank + t, ev[i].kind);
    }

    printf("%lld\n", ans[4][4]);
    return 0;
}
