#include <cstdio>

using namespace std;

const int MAXN = 18;  // 题目中 N 的上限

int n;
int likeMask[MAXN];  // likeMask[c]：奶牛 c 愿意接受的礼物集合（含它原本拿到的礼物 c）
long long ans[1 << MAXN];       // ans[mask]：mask 内的奶牛只在 mask 内部互换礼物的方案数
long long dp[1 << MAXN][MAXN];  // dp 的含义见思路

int main() {
    scanf("%d", &n);
    for (int c = 0; c < n; ++c) {
        int mask = 0;
        bool closed = false;
        for (int j = 0; j < n; ++j) {
            int g;
            scanf("%d", &g);
            --g;
            if (!closed) {
                mask |= 1 << g;  // 礼物 g 不比 c 差，c 可以接受
                if (g == c) closed = true;  // 排在 c 之后的礼物都更差，不用记录
            }
        }
        likeMask[c] = mask;
    }

    // 环分解 DP：把方案写成若干个环，每个环从最大标号开头，各环再按最大标号递增排序，
    // 就能把每种方案唯一地枚举一次。dp[mask][last] 表示：mask 的最高位 i 是当前环里
    // 最大的标号，mask\{last} 中的奶牛已经拿到了 mask\{i} 中的礼物，下一步给奶牛 last 发礼物。
    ans[0] = 1;
    for (int k = 0; k < n; ++k) dp[1 << k][k] = 1;  // 单独开一个环，其最大标号为 k
    for (int i = 0; i < n; ++i) {
        // 只处理最高位恰为 i 的 mask：此时已闭合的环的最大标号都不超过 i
        for (int mask = 1 << i; mask < 1 << (i + 1); ++mask) {
            for (int last = 0; last <= i; ++last) {
                if (!(mask >> last & 1)) continue;
                long long val = dp[mask][last];
                if (!val) continue;
                // 情况一：给 last 一个比 i 小的礼物 k，当前环还未闭合
                for (int k = 0; k < i; ++k)
                    if (!(mask >> k & 1) && (likeMask[last] >> k & 1))
                        dp[mask | (1 << k)][k] += val;
                // 情况二：把礼物 i 给 last，当前环闭合（i 是环里的最大标号）
                if (likeMask[last] >> i & 1) ans[mask] += val;
            }
            // mask 里的环全部闭合之后，才能再开一个新环，其最大标号 k 必须更大
            for (int k = i + 1; k < n; ++k)
                dp[mask | (1 << k)][k] += ans[mask];
        }
    }

    int q;
    scanf("%d", &q);
    char breed[MAXN + 5];
    for (int t = 0; t < q; ++t) {
        scanf("%s", breed);
        int gMask = 0, hMask = 0;
        for (int c = 0; c < n; ++c) {
            // 礼物只能交给同品种的奶牛，两个品种互不影响，答案直接相乘
            if (breed[c] == 'G') gMask |= 1 << c;
            else hMask |= 1 << c;
        }
        printf("%lld\n", ans[gMask] * ans[hMask]);
    }
    return 0;
}
