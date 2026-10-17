#include <algorithm>
#include <cstdio>
#include <cstdlib>

using namespace std;

const int MAXN = 505;
const int INF = 1000000000;

int n, k;
int xs[MAXN], ys[MAXN];
// dp[skipped][i]：恰好跳过 skipped 个检查点、最后停在检查点 i 时的最小路程。
int dp[MAXN][MAXN];

int manhattan(int a, int b) {
    return abs(xs[a] - xs[b]) + abs(ys[a] - ys[b]);
}

int main() {
    scanf("%d %d", &n, &k);
    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &xs[i], &ys[i]);
    }
    for (int skipped = 0; skipped <= k; ++skipped) {
        for (int i = 0; i < n; ++i) {
            dp[skipped][i] = INF;
        }
    }
    // 起点和终点不允许跳过，所以真正能跳过的检查点数不超过 n-2 个。
    int maxSkip = min(k, n - 2);
    dp[0][0] = 0;

    // 只记录"必须停留的检查点"序列，枚举上一个停留点 i 与下一个停留点 l，
    // 二者之间被跳过的个数就是 l-i-1。这样按停留点转移，天然保证顺序。
    for (int i = 0; i < n; ++i) {
        for (int skipped = 0; skipped <= maxSkip; ++skipped) {
            if (dp[skipped][i] == INF) {
                continue;
            }
            for (int l = i + 1; l < n; ++l) {
                int nextSkipped = skipped + (l - i - 1);
                if (nextSkipped > maxSkip) {
                    break;  // l 再大只会跳得更多，后面都不可能合法
                }
                int candidate = dp[skipped][i] + manhattan(i, l);
                if (candidate < dp[nextSkipped][l]) {
                    dp[nextSkipped][l] = candidate;
                }
            }
        }
    }

    // 题目允许"至多跳过 k 个"，所以把恰好跳过 0..maxSkip 个的答案一起取最小。
    int answer = INF;
    for (int skipped = 0; skipped <= maxSkip; ++skipped) {
        answer = min(answer, dp[skipped][n - 1]);
    }
    printf("%d\n", answer);
    return 0;
}
