#include <cstdio>

using namespace std;

const int MAXN = 1005;
const long long INF = 1LL << 60;

int holsteinX[MAXN];
int holsteinY[MAXN];
int guernseyX[MAXN];
int guernseyY[MAXN];

// dp[i][j][0]: 已经拜访了前 i 头 Holstein 和前 j 头 Guernsey, 此刻停在第 i 头 Holstein;
// dp[i][j][1]: 拜访情况相同, 此刻停在第 j 头 Guernsey.
// 每种牛的拜访顺序都被编号锁死, 于是"两种牛各拜访了几头"加上"现在站在哪头牛上"
// 就完整描述了历史, 不需要再记住更细的路线.
long long dp[MAXN][MAXN][2];

long long squareDistance(int x1, int y1, int x2, int y2) {
    long long dx = x1 - x2;
    long long dy = y1 - y2;
    return dx * dx + dy * dy;
}

int main() {
    int h, g;
    scanf("%d%d", &h, &g);
    for (int i = 1; i <= h; ++i) {
        scanf("%d%d", &holsteinX[i], &holsteinY[i]);
    }
    for (int j = 1; j <= g; ++j) {
        scanf("%d%d", &guernseyX[j], &guernseyY[j]);
    }

    for (int i = 0; i <= h; ++i) {
        for (int j = 0; j <= g; ++j) {
            dp[i][j][0] = INF;
            dp[i][j][1] = INF;
        }
    }
    // 出发点就是 Holstein 1, 一趟路都还没走.
    dp[1][0][0] = 0;

    for (int i = 1; i <= h; ++i) {
        for (int j = 0; j <= g; ++j) {
            for (int k = 0; k < 2; ++k) {
                if (dp[i][j][k] == INF) {
                    continue;  // 到不了的状态不必往外扩展
                }
                // k 为 0 时 i >= 1, k 为 1 时 j 只能 >= 1 (dp[*][0][1] 一直是 INF), 下标都有效.
                int currentX = (k == 0) ? holsteinX[i] : guernseyX[j];
                int currentY = (k == 0) ? holsteinY[i] : guernseyY[j];
                if (i < h) {
                    // 下一头该拜访的是 Holstein i+1, 它只能排在所有已拜访的牛之后.
                    long long cost = dp[i][j][k] +
                                     squareDistance(currentX, currentY, holsteinX[i + 1], holsteinY[i + 1]);
                    if (cost < dp[i + 1][j][0]) {
                        dp[i + 1][j][0] = cost;
                    }
                }
                if (j < g) {
                    long long cost = dp[i][j][k] +
                                     squareDistance(currentX, currentY, guernseyX[j + 1], guernseyY[j + 1]);
                    if (cost < dp[i][j + 1][1]) {
                        dp[i][j + 1][1] = cost;
                    }
                }
            }
        }
    }

    // 题目要求最后停在 Holstein H, 所以只看 dp[h][g][0].
    printf("%lld\n", dp[h][g][0]);
    return 0;
}
