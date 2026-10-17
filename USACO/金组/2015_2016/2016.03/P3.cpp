#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 250;   // n 的上限

int n;
int a[MAXN];
// dp[i][j]: 区间 [i, j] 若能完全合并成一个数, 该数是多少; 不能则记 -1。
// 同一区间合出来的数只与区间本身有关, 所以"能不能合、合出几"都是唯一确定的。
int dp[MAXN][MAXN];

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    int answer = 0;
    // 按区间长度递增递推: 长度 1 是边界, 更长的区间只依赖更短的子区间
    for (int len = 1; len <= n; len++) {
        for (int i = 0; i + len - 1 < n; i++) {
            int j = i + len - 1;
            if (len == 1) {
                dp[i][j] = a[i];
            } else {
                dp[i][j] = -1;
                // 最后一次操作一定是把左半截 [i, k] 与右半截 [k+1, j] 合出的数再合并
                for (int k = i; k < j; k++) {
                    if (dp[i][k] > 0 && dp[i][k] == dp[k + 1][j]) {
                        dp[i][j] = max(dp[i][j], dp[i][k] + 1);
                    }
                }
            }
            // 不要求把整个序列合完, 所以任意子区间合出的数都可能成为最终的最大值
            answer = max(answer, dp[i][j]);
        }
    }

    printf("%d\n", answer);
    return 0;
}
