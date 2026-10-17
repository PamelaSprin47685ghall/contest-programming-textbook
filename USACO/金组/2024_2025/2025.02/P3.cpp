#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 16;
const int INF = 1000000000;

int n, m;
bool edge[MAXN][MAXN];      // 原图的边
int groupCost[1 << MAXN];   // 把 mask 单独作为一部时的代价: 组内原有的边数 - 组内原有的非边数
int dp[1 << MAXN];

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        u--; v--;
        edge[u][v] = edge[v][u] = true;
    }

    // 性质 "若 a-b 有边则任意 c 都与 a 或 b 相邻" 在补图上就是: 两条补边能传递。
    // 于是最终图的补图一定是若干个互不相交的团, 也就是说最终图必须是完全多部图:
    // 同部之间没有边, 不同部之间全是边。枚举部之间的划分即可。
    // 一组 mask 的代价 = (组内原边要删) + (组内原非边要补),
    // 而所有部内部的两两点数之和恒为 C(n,2) 减去跨部的非边数, 故可以写成
    //   (总非边数) + SUM(组内原边数 - 组内原非边数)。
    int pairs = n * (n - 1) / 2;
    groupCost[0] = 0;
    for (int mask = 1; mask < (1 << n); mask++) {
        int lowBit = mask & (-mask);
        int low = 0;
        while (!((lowBit >> low) & 1)) low++;
        int rest = mask ^ lowBit;
        int val = groupCost[rest];
        for (int j = 0; j < n; j++)
            if (rest & (1 << j)) val += edge[low][j] ? 1 : -1;
        groupCost[mask] = val;
    }

    // dp[mask]: mask 内部划分成若干部后, 总非边数 + SUM(groupCost) 的最小值
    dp[0] = pairs - m;
    for (int mask = 1; mask < (1 << n); mask++) {
        int lowBit = mask & (-mask);
        int rest = mask ^ lowBit;
        int best = INF;
        // 只枚举包含 lowBit 的那一部, 保证每种划分恰好被统计一次
        for (int sub = rest; ; sub = (sub - 1) & rest) {
            int cand = dp[mask ^ (sub | lowBit)] + groupCost[sub | lowBit];
            if (cand < best) best = cand;
            if (sub == 0) break;
        }
        dp[mask] = best;
    }

    printf("%d\n", dp[(1 << n) - 1]);
    return 0;
}
