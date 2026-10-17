#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 105;
const int INF = 1000000000;

int n;
int logValue[MAXN];       // 记录中第 i 天的读数(下标从 0 开始)
// dp[i][j][k]: 只考虑前 i+1 天, 共发生 k 次破门且最后一次破门在第 j 天时,
// 至少需要篡改多少条记录
int dp[MAXN][MAXN][MAXN];

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &logValue[i]);

    for (int i = 0; i < n; i++)
        for (int j = 0; j <= i; j++)
            for (int k = 0; k <= n; k++)
                dp[i][j][k] = INF;

    // 记录从破门当天开始, 所以第 1 天一定破门, 计数器读数必须是 0
    dp[0][0][1] = (logValue[0] == 0 ? 0 : 1);

    for (int i = 1; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            for (int k = 1; k <= i + 1; k++) {
                if (j < i) {
                    // 第 i 天不是破门日: 前 i 天里最后一次破门仍然是第 j 天
                    dp[i][j][k] = dp[i - 1][j][k];
                } else {
                    // 第 i 天破门: 前 i 天里最后一次破门可以是任意一天, 次数少 1
                    for (int j1 = 0; j1 < i; j1++) {
                        if (dp[i - 1][j1][k - 1] < dp[i][j][k]) {
                            dp[i][j][k] = dp[i - 1][j1][k - 1];
                        }
                    }
                }
                // 最后一次破门在第 j 天, 那么第 i 天的计数必然是 i-j;
                // 若记录上不是这个数, 就必须篡改这一天
                if (dp[i][j][k] < INF && logValue[i] != i - j) dp[i][j][k]++;
            }
        }
    }

    for (int k = 1; k <= n; k++) {
        int best = INF;
        for (int j = 0; j < n; j++) {
            if (dp[n - 1][j][k] < best) best = dp[n - 1][j][k];
        }
        printf("%d\n", best);
    }
    return 0;
}
