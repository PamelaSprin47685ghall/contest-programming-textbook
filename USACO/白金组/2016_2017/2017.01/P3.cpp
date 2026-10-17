#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 50;         // 奶牛数上限
const int MAXH = 50;         // 高度上限
const int NEG = -1000000000; // "不可能"状态的哨兵，足够小，保证不会被取到

int n;
int height[MAXN];
int dp[MAXN][MAXN][MAXH + 1][MAXH + 1];

// 区间 [i, j] 内能取到的最大长度：
// k 是从区间左边已经取走的最大高度，m 是从区间右边已经取走的最小高度。
// 约定 i > j 表示空区间。
int getValue(int i, int j, int k, int m) {
    if (k > m) return NEG; // 左边已出现比右边更大的高度，凑不成递增序列
    if (i > j) return 0;
    return dp[i][j][k][m];
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) scanf("%d", &height[i]);

    // 一次子序列翻转等价于若干对下标 (i1,j1),...,(it,jt)，
    // 满足 i1 < ... < it < jt < ... < j1，即"从两端向内一层层配对"。
    // 因此按区间长度从小到大填表，每个状态只依赖更短区间的状态。
    for (int len = 1; len <= n; ++len) {
        for (int i = 0; i + len - 1 < n; ++i) {
            int j = i + len - 1;
            for (int k = 0; k <= MAXH; ++k) {
                for (int m = 0; m <= MAXH; ++m) {
                    if (k > m) {
                        dp[i][j][k][m] = NEG;
                        continue;
                    }
                    if (i == j) {
                        // 只剩一头奶牛：高度落在 k..m 之间就能取
                        dp[i][j][k][m] = (height[i] >= k && height[i] <= m) ? 1 : 0;
                        continue;
                    }
                    int best = 0; // 最差也能一个都不取
                    // 情况一：把 i、j 配成一对交换，四种取法
                    best = max(best, getValue(i + 1, j - 1, k, m));
                    if (height[j] >= k) // 交换后 a[j] 跑到左端，可以作为左侧新元素
                        best = max(best, getValue(i + 1, j - 1, height[j], m) + 1);
                    if (height[i] <= m) // 交换后 a[i] 跑到右端，可以作为右侧新元素
                        best = max(best, getValue(i + 1, j - 1, k, height[i]) + 1);
                    if (k <= height[j] && height[j] <= height[i] && height[i] <= m)
                        best = max(best, getValue(i + 1, j - 1, height[j], height[i]) + 2);
                    // 情况二：i 不参与交换，直接把它交给左侧（取或不取）
                    best = max(best, getValue(i + 1, j, k, m));
                    if (height[i] >= k)
                        best = max(best, getValue(i + 1, j, height[i], m) + 1);
                    // 情况三：j 不参与交换，直接把它交给右侧（取或不取）
                    best = max(best, getValue(i, j - 1, k, m));
                    if (height[j] <= m)
                        best = max(best, getValue(i, j - 1, k, height[j]) + 1);
                    dp[i][j][k][m] = best;
                }
            }
        }
    }

    // 初始时左侧还没取任何东西（k = 0），右侧还没有下限（m = 50，高度不超过 50）
    printf("%d\n", dp[0][n - 1][0][MAXH]);
    return 0;
}
