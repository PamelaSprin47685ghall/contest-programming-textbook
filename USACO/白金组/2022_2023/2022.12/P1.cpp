#include <cstdio>

using namespace std;

const int MAXN = 305;                     // 顶点数上限
const int MAXH = 4;                       // K 的一半最多是 4，所以只维护到 4 条边
const long long INF = 1000000000000000LL; // 远大于任何真实路径长度（最多 8 × 10^8）

int n, k;
long long w[MAXN][MAXN];       // 边的原始权值
long long dp1[MAXH + 1][MAXN]; // dp1[e][v]：1 走到 v、恰好用 e 条边的最小花费
long long dpN[MAXH + 1][MAXN]; // dpN[e][v]：v 走到 n、恰好用 e 条边的最小花费
long long bet[3][MAXN][MAXN];  // bet[e][u][v]：u 走到 v、恰好用 e（≤ 2）条边的最小花费
int remI[MAXN * MAXN], remJ[MAXN * MAXN]; // 拆除顺序（按输入顺序）
long long res[MAXN * MAXN];    // 每次拆除后的答案

// 加入一条权值为 wab 的边 (a, b)。
// 关键性质：边只会被加上（倒着看拆除过程），所以任何信息都只减不增，
// 并且"新变短"的路径必然用到了这条新边——按新边在路径里的位置分类枚举即可。
void addEdge(int a, int b, long long wab) {
    // ---- bet[1]、bet[2] ----
    if (wab < bet[1][a][b]) bet[1][a][b] = wab;
    for (int v = 1; v <= n; ++v) {
        long long x = (bet[1][b][v] >= INF ? INF : wab + bet[1][b][v]); // 新边是两条中的第一条
        if (x < bet[2][a][v]) bet[2][a][v] = x;
        long long y = (bet[1][v][a] >= INF ? INF : bet[1][v][a] + wab); // 新边是两条中的第二条
        if (y < bet[2][v][b]) bet[2][v][b] = y;
    }

    // ---- dp1：路径形如 1 -> …… -> a -> b -> …… -> v ----
    // 新边排在第 (e - j + 1) 条时，前面的 e - j 条边是 dp1[e - j][a]，
    // 后面的 j - 1 条边用 bet[j - 1][b][v] 表示。j = 1、2、3 都能 O(N) 枚举；
    // e = 4 且新边是第一条（此时必有 a = 1）时后面剩 3 条，拆成 1 + 2 条 O(N^2) 枚举。
    for (int e = 1; e <= MAXH; ++e) {
        long long cand = dp1[e - 1][a] + wab; // 新边是最后一条
        if (cand < dp1[e][b]) dp1[e][b] = cand;
        for (int v = 1; v <= n; ++v) {
            if (e >= 2) {
                cand = dp1[e - 2][a] + wab + bet[1][b][v]; // 新边后面还剩 1 条边
                if (cand < dp1[e][v]) dp1[e][v] = cand;
            }
            if (e >= 3) {
                cand = dp1[e - 3][a] + wab + bet[2][b][v]; // 新边后面还剩 2 条边
                if (cand < dp1[e][v]) dp1[e][v] = cand;
            }
        }
        if (e == MAXH && a == 1) { // 新边是 4 条边中的第一条：b 到 v 的 3 条边拆成 1 + 2
            for (int i = 1; i <= n; ++i) {
                if (bet[1][b][i] >= INF) continue; // 剪枝：这条路根本不存在
                long long base = wab + bet[1][b][i]; // 从 b 先走到 i
                for (int v = 1; v <= n; ++v) {
                    if (bet[2][i][v] >= INF) continue;
                    cand = base + bet[2][i][v];
                    if (cand < dp1[MAXH][v]) dp1[MAXH][v] = cand;
                }
            }
        }
    }

    // ---- dpN：路径形如 v -> …… -> a -> b -> …… -> n，对称处理 ----
    for (int e = 1; e <= MAXH; ++e) {
        long long cand = dpN[e - 1][b] + wab; // 新边是通往 n 的最后一条
        if (cand < dpN[e][a]) dpN[e][a] = cand;
        for (int v = 1; v <= n; ++v) {
            if (e >= 2) {
                cand = dpN[e - 2][b] + wab + bet[1][v][a]; // 新边前面还剩 1 条边（v -> a）
                if (cand < dpN[e][v]) dpN[e][v] = cand;
            }
            if (e >= 3) {
                cand = dpN[e - 3][b] + wab + bet[2][v][a]; // 新边前面还剩 2 条边
                if (cand < dpN[e][v]) dpN[e][v] = cand;
            }
        }
        if (e == MAXH && b == n) { // 新边是 4 条边中的最后一条：v 到 a 的 3 条边拆成 2 + 1
            long long last[MAXN]; // last[j]：从 j 一步走到 a，再走新边到 b
            for (int j = 1; j <= n; ++j)
                last[j] = (bet[1][j][a] >= INF ? INF : bet[1][j][a] + wab);
            for (int v = 1; v <= n; ++v) {
                long long best = INF;
                for (int j = 1; j <= n; ++j) {
                    if (bet[2][v][j] >= INF) continue;
                    cand = bet[2][v][j] + last[j];
                    if (cand < best) best = cand;
                }
                if (best < dpN[MAXH][v]) dpN[MAXH][v] = best;
            }
        }
    }
}

int main() {
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j) scanf("%lld", &w[i][j]);
    int m = n * n;
    for (int t = 0; t < m; ++t) scanf("%d%d", &remI[t], &remJ[t]);

    // 初值：所有"走 e 条边"的信息都是 INF（不存在），空路径花费 0
    for (int e = 0; e <= MAXH; ++e)
        for (int v = 1; v <= n; ++v) dp1[e][v] = dpN[e][v] = INF;
    for (int e = 0; e <= 2; ++e)
        for (int u = 1; u <= n; ++u)
            for (int v = 1; v <= n; ++v) bet[e][u][v] = INF;
    dp1[0][1] = 0; // 从 1 出发、零条边停在 1 号点
    dpN[0][n] = 0; // 停在 n 号点、再走零条边到 n

    int half = k / 2, rest = k - half; // 把 K 条边从中间切成两半，枚举中间点
    // 倒着处理：一开始图是空的（等于拆完 N^2 条边之后），每次"加回"一条边
    for (int t = m - 1; t >= 0; --t) {
        long long best = INF;
        for (int v = 1; v <= n; ++v) {
            long long cand = dp1[half][v] + dpN[rest][v]; // 前半段 + 后半段，拼成 K 条边
            if (cand < best) best = cand;
        }
        res[t] = (best >= INF ? -1 : best); // 当前状态正是"拆掉前 t 条边"之后
        addEdge(remI[t], remJ[t], w[remI[t]][remJ[t]]);
    }

    for (int t = 0; t < m; ++t) printf("%lld\n", res[t]);
    return 0;
}
