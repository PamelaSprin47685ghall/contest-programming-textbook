#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 405;
const long long INF = 1000000000000000000LL;   // 1e18, 比任何合法花费都大且相加不会溢出

int n, k;
int a[MAXN];
long long dp[MAXN][MAXN];   // dp[m][j]: 抓完前 m 组、恰好换 j 次网, 所有网大小之和的最小值

int main() {
    scanf("%d %d", &n, &k);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);

    long long total = 0;      // 蛇的总数, 最后从网大小之和里减掉
    int prefixMax = 0;        // max(a[1..m])
    for (int m = 1; m <= n; m++) {
        total += a[m];
        if (a[m] > prefixMax) prefixMax = a[m];
        // 一次都不换: 前 m 组共用一张网, 网大小必须是不小于所有组的下界, 即前缀最大值
        dp[m][0] = (long long)prefixMax * m;
        for (int j = 1; j <= k && j < m; j++) {
            // 最后一段是 a[i+1..m], 它前面的前缀已经用 j-1 次换网解决
            // 从 i = m-1 往左扫, 顺便维护 a[i+1..m] 的最大值, 每次转移 O(1)
            long long best = INF;
            int segMax = a[m];
            for (int i = m - 1; i >= j; i--) {
                long long cand = dp[i][j - 1] + (long long)segMax * (m - i);
                if (cand < best) best = cand;
                if (a[i] > segMax) segMax = a[i];
            }
            dp[m][j] = best;
        }
    }
    // 浪费的空间 = 网大小之和 - 蛇的总数(每一组都恰好浪费 网大小 - 组内蛇数)
    printf("%lld\n", dp[n][k] - total);
    return 0;
}
