#include <algorithm>
#include <cstdio>
#include <cstdlib>

using namespace std;

const int MAXN = 1005;

int topSide[MAXN];
int bottomSide[MAXN];
// dp[i][j]: 只看上边前 i 块地和下边前 j 块地时, 最多能画多少条互不相交的斑马线.
int dp[MAXN][MAXN];

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &topSide[i]);
    }
    for (int j = 1; j <= n; ++j) {
        scanf("%d", &bottomSide[j]);
    }

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            // 斑马线不相交当且仅当两端的场地编号同增, 所以按编号从小到大决策:
            // 要么放弃上边第 i 块地, 要么放弃下边第 j 块地
            int best = max(dp[i - 1][j], dp[i][j - 1]);
            // 也可以把这两块地配成一条斑马线, 前缀里剩下的斑马线都在它左边, 不会交叉
            if (abs(topSide[i] - bottomSide[j]) <= 4) {
                best = max(best, dp[i - 1][j - 1] + 1);
            }
            dp[i][j] = best;
        }
    }
    printf("%d\n", dp[n][n]);
    return 0;
}
