#include <cstdio>

using namespace std;

const int MAXN = 12;
const int MAXM = 10005;
const int INF = 1000000000;

int n, m;
int side[MAXN];              // 原有的瓷砖边长 A_1..A_N
int best[MAXM][MAXN];        // best[i][j]: 用前 j 块瓷砖凑出总面积 i 的最小花费

int main() {
    scanf("%d %d", &n, &m);
    for (int j = 1; j <= n; j++) scanf("%d", &side[j]);

    // 边界: 一块瓷砖都不用, 只能凑出面积 0
    best[0][0] = 0;
    for (int i = 1; i <= m; i++) best[i][0] = INF;

    for (int j = 1; j <= n; j++) {
        for (int i = 0; i <= m; i++) {
            best[i][j] = INF;
            // 第 j 块瓷砖换成边长为 k 的 (边长至少为 1, 且面积 k*k 不超过 i);
            // 花费为交换代价 (side[j]-k)^2 加上前 j-1 块凑出面积 i-k*k 的最小花费.
            // 因为每块瓷砖只交换一次(不允许二次交换), 按瓷砖逐个决策就覆盖了所有方案.
            for (int k = 1; k * k <= i; k++) {
                int diff = side[j] - k;
                int cost = diff * diff + best[i - k * k][j - 1];
                if (cost < best[i][j]) best[i][j] = cost;
            }
        }
    }

    if (best[m][n] >= INF) printf("-1\n");
    else printf("%d\n", best[m][n]);
    return 0;
}
