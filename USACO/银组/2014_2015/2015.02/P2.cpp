#include <cstdio>

using namespace std;

const int MAXR = 105;
const int MOD = 1000000007;

int R, C, K;
int color[MAXR][MAXR];   // 每个格子的颜色 (编号)
int dp[MAXR][MAXR];      // dp[i][j]: 从左上角跳到 (i,j) 的方案数

int main() {
    scanf("%d%d%d", &R, &C, &K);
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++)
            scanf("%d", &color[i][j]);

    dp[0][0] = 1;
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            if (dp[i][j] == 0) continue;
            // 从 (i,j) 只能往右下方跳: 行更大且列更大;
            // 颜色相同的格子不允许落点, 所以直接跳过.
            for (int k = i + 1; k < R; k++) {
                for (int l = j + 1; l < C; l++) {
                    if (color[i][j] == color[k][l]) continue;
                    // 两个 dp 值都小于 MOD, 相加不会溢出 int, 用减法代替取模更快.
                    int sum = dp[k][l] + dp[i][j];
                    if (sum >= MOD) sum -= MOD;
                    dp[k][l] = sum;
                }
            }
        }
    }
    printf("%d\n", dp[R - 1][C - 1]);
    return 0;
}
