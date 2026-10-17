#include <cstdio>
#include <algorithm>

using namespace std;

// N < 2*10^5，节点编号 1..N，i 的父亲是 i/2，孩子是 2*i 与 2*i+1。
const int MAXN = 200005;
// 单条路径最多约 18 个节点，每点花费不超过 10^9，真实答案远小于 10^15。
const long long INF = 1000000000000000LL;

int N, Q;
int a[MAXN];           // 每个节点的初始值
long long c[MAXN];     // 每个节点的修改花费
int f[MAXN];           // 当前查询值 m 下 p_m(a_i)：0 表示小于 m，1 表示等于 m，2 表示大于 m
long long dp[MAXN][3]; // dp[i][j]：使节点 i 的中位数近似值与 m 的大小关系为 j 的最小总花费

// 离线扫描用的事件：type=0 是节点事件（某节点的初始值等于该事件的值），type=1 是查询事件
struct Event {
    int val;
    int type;
    int idx;
};

Event ev[2 * MAXN + 5];
int ecnt;
long long ans[MAXN];

// 同一数值先处理节点事件（把它们的状态置为"等于"），再回答查询
bool cmpEvent(const Event &x, const Event &y) {
    if (x.val != y.val) return x.val < y.val;
    return x.type < y.type;
}

// 三个数（标签）的中位数：中位数运算对三个参数单调，所以标签可以参与取中位数
int med3(int x, int y, int z) {
    int lo = min(min(x, y), z);
    int hi = max(max(x, y), z);
    return x + y + z - lo - hi;
}

// 重算节点 p 的 dp。p_m 单调，故 p_m(med(u,v,w)) = med(p_m(u),p_m(v),p_m(w))，
// 于是只需在 3 个标签上做转移，与具体数值无关。
void recompute(int p) {
    long long cost[3];
    cost[0] = cost[1] = cost[2] = c[p];
    cost[f[p]] = 0; // 不改动节点 p 本身时不需要花费
    int l = 2 * p, r = 2 * p + 1;
    if (l > N) {
        // 叶子没有孩子，它的中位数近似值就是自己的值
        dp[p][0] = cost[0];
        dp[p][1] = cost[1];
        dp[p][2] = cost[2];
        return;
    }
    long long best[3] = {INF, INF, INF};
    for (int u = 0; u < 3; ++u)
        for (int v = 0; v < 3; ++v)
            for (int w = 0; w < 3; ++w) {
                int j = med3(u, v, w);
                long long val = cost[u] + dp[l][v] + dp[r][w];
                if (val < best[j]) best[j] = val;
            }
    dp[p][0] = best[0];
    dp[p][1] = best[1];
    dp[p][2] = best[2];
}

// 把节点 p 的状态改为 state，并顺着祖先一路更新到根：只有 p 到根的 dp 会变
void update(int p, int state) {
    f[p] = state;
    for (int x = p; x >= 1; x /= 2) recompute(x);
}

int main() {
    scanf("%d", &N);
    for (int i = 1; i <= N; ++i) {
        scanf("%d %lld", &a[i], &c[i]);
        f[i] = 2; // 扫描开始时 m 比所有初始值都小，各节点都"大于 m"
        ev[ecnt].val = a[i];
        ev[ecnt].type = 0;
        ev[ecnt].idx = i;
        ++ecnt;
    }
    scanf("%d", &Q);
    for (int i = 0; i < Q; ++i) {
        int m;
        scanf("%d", &m);
        ev[ecnt].val = m;
        ev[ecnt].type = 1;
        ev[ecnt].idx = i;
        ++ecnt;
    }
    sort(ev, ev + ecnt, cmpEvent);

    // 建出 m 取最小值时的 dp
    for (int p = N; p >= 1; --p) recompute(p);

    // 从小到大扫描所有出现过的数值。每跨过一个数值 v，恰好 a_i = v 的节点状态
    // 由 2 变成 0（先短暂借道 1 回答 m = v 的查询），每个节点总共只改动两次。
    int i = 0;
    while (i < ecnt) {
        int j = i;
        while (j < ecnt && ev[j].val == ev[i].val) ++j;
        int k = i;
        while (k < j && ev[k].type == 0) ++k; // [i,k) 是节点事件，[k,j) 是查询事件
        for (int t = i; t < k; ++t) update(ev[t].idx, 1);
        for (int t = k; t < j; ++t) ans[ev[t].idx] = dp[1][1]; // 根的值等于 m
        for (int t = i; t < k; ++t) update(ev[t].idx, 0);
        i = j;
    }

    for (int t = 0; t < Q; ++t) printf("%lld\n", ans[t]);
    return 0;
}
