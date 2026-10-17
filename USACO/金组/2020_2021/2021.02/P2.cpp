#include <cstdio>

using namespace std;

const int MAXN = 300;

int n;
int color[MAXN + 1];      // 目标画作每一格的颜色
int dp[MAXN + 1][MAXN + 1];  // dp[i][j]：区间 [i, j] 内最多能画出多少条"同色弧线段"

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) scanf("%d", &color[i]);

    // 把最优画法看成若干条弧线段：同一次笔刷在最终画作里连续出现的一段算一条。
    // 笔刷数 = 格数 N 减去弧线段条数，于是问题变成最大化弧线段数量。
    // 弧线段之间要么嵌套要么完全分离，正好适合区间 DP：按区间长度从小到大递推
    for (int len = 2; len <= n; ++len) {
        for (int i = 0; i + len - 1 < n; ++i) {
            int j = i + len - 1;
            int best = 0;
            // 情况一：从 i 到 j 画一条覆盖整个区间的弧（要求两端颜色相同），
            // 内部所有弧都必须落在 [i+1, j-1] 里
            if (color[i] == color[j]) {
                best = 1 + (len > 2 ? dp[i + 1][j - 1] : 0);
            }
            // 情况二：存在切点 k，使得没有弧跨越 k，两侧互不影响
            for (int k = i + 1; k < j; ++k) {
                int cand = dp[i][k] + dp[k][j];
                if (cand > best) best = cand;
            }
            dp[i][j] = best;
        }
    }
    // 单格的区间（len == 1）本身就是一条弧，只是不参与上面的枚举，故直接由 N 减去净收益
    printf("%d\n", n - dp[0][n - 1]);
    return 0;
}
