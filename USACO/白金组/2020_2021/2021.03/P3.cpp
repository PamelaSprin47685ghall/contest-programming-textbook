#include <cstdio>

using namespace std;

const int MAXN = 155;         // 网格边长上限
const int MOD = 1000000007;

int n;
bool grass[MAXN][MAXN];

// dp[a][b][l][r]：子集的行是连续的若干行，最后一行（下边界）取第 row 行，
// 该行选了列区间 [l, r]，并且
//   a = 0：左边界序列 L 还没出现过"严格上升"，处于非递增阶段；
//   a = 1：L 已经出现过严格上升（即已经到达谷底），之后必须非递减；
//   b = 0：右边界序列 R 还没出现过"严格下降"，处于非递减阶段；
//   b = 1：R 已经出现过严格下降（即已经越过峰顶），之后必须非递增。
// 这样每个形状的 (a, b) 取值由"第一次严格上升/下降"唯一确定，不会重复计数。
int dp[2][2][MAXN][MAXN];
int nxt[2][2][MAXN][MAXN];
int pre[2][2][MAXN][MAXN];    // dp 的二维前缀和

// 矩形和：sum over l1<=l<=l2, r1<=r<=r2 of dp[a][b][l][r]
int rectSum(int a, int b, int l1, int l2, int r1, int r2) {
    if (l1 > l2 || r1 > r2) return 0;
    long long res = pre[a][b][l2][r2];
    if (r1 > 0) res -= pre[a][b][l2][r1 - 1];
    if (l1 > 0) res -= pre[a][b][l1 - 1][r2];
    if (l1 > 0 && r1 > 0) res += pre[a][b][l1 - 1][r1 - 1];
    res %= MOD;
    if (res < 0) res += MOD;
    return (int)res;
}

int main() {
    scanf("%d", &n);
    char line[MAXN + 5];
    for (int i = 0; i < n; ++i) {
        scanf("%s", line);
        for (int j = 0; j < n; ++j) grass[i][j] = (line[j] == 'G');
    }

    long long answer = 0;
    for (int row = 0; row < n; ++row) {
        // 先把上一行的 dp 做成前缀和，之后每种转移都能 O(1) 取矩形和
        for (int a = 0; a < 2; ++a) {
            for (int b = 0; b < 2; ++b) {
                for (int l = 0; l < n; ++l) {
                    for (int r = 0; r < n; ++r) {
                        long long s = dp[a][b][l][r];
                        if (l > 0) s += pre[a][b][l - 1][r];
                        if (r > 0) s += pre[a][b][l][r - 1];
                        if (l > 0 && r > 0) s -= pre[a][b][l - 1][r - 1];
                        pre[a][b][l][r] = (int)(s % MOD);
                    }
                }
            }
        }
        for (int a = 0; a < 2; ++a)
            for (int b = 0; b < 2; ++b)
                for (int l = 0; l < n; ++l)
                    for (int r = 0; r < n; ++r) nxt[a][b][l][r] = 0;

        for (int L = 0; L < n; ++L) {
            if (!grass[row][L]) continue;
            for (int R = L; R < n && grass[row][R]; ++R) {
                // 上一行区间 [l, r]。相邻两行必须有交集：[L, R] 与 [l, r] 相交，
                // 即 l <= R 且 r >= L；下面的取值范围都已把这个条件并进去。
                //
                // a' = 0（L 仍非递增）：上一行 a = 0 且 l >= L
                // a' = 1：上一行 a = 0 且 l < L（严格上升，第一次翻越谷底），
                //         或 a = 1 且 l <= L（继续非递减）
                // b' = 0（R 仍非递减）：上一行 b = 0 且 r <= R
                // b' = 1：上一行 b = 0 且 r > R（严格下降，第一次越过峰顶），
                //         或 b = 1 且 r >= R（继续非递增）
                int v00 = rectSum(0, 0, L, R, L, R);
                int v01 = (rectSum(0, 0, L, R, R + 1, n - 1) + rectSum(0, 1, L, R, R, n - 1)) % MOD;
                int v10 = (rectSum(0, 0, 0, L - 1, L, R) + rectSum(1, 0, 0, L, L, R)) % MOD;
                int v11 = ((rectSum(0, 0, 0, L - 1, R + 1, n - 1) + rectSum(1, 0, 0, L, R + 1, n - 1)) % MOD
                         + (rectSum(0, 1, 0, L - 1, R, n - 1) + rectSum(1, 1, 0, L, R, n - 1)) % MOD) % MOD;

                nxt[0][0][L][R] = v00 + 1;  // +1：这个子集只由第 row 行这一段构成
                nxt[0][1][L][R] = v01;
                nxt[1][0][L][R] = v10;
                nxt[1][1][L][R] = v11;

                // 每个合法子集在它的最后一行被统计一次
                long long add = ((long long)v00 + 1 + v01 + v10 + v11) % MOD;
                answer = (answer + add) % MOD;
            }
        }

        for (int a = 0; a < 2; ++a)
            for (int b = 0; b < 2; ++b)
                for (int l = 0; l < n; ++l)
                    for (int r = 0; r < n; ++r) dp[a][b][l][r] = nxt[a][b][l][r];
    }

    printf("%lld\n", answer);
    return 0;
}
