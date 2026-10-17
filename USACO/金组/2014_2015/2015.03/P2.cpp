#include <cstdio>

using namespace std;

const int MAXN = 505;
const int MOD = 1000000007;

int N;
char grid[MAXN][MAXN];

// dp[r1][r2] 表示已经从路径中心向两端对称展开 a 层时的方案数: 左侧端点落在第 r1 行,
// 右侧端点落在第 r2 行。端点所在的反对角线由 a 决定, 所以列号可以由行号唯一推出,
// 状态里不必再存列。
int dp[MAXN][MAXN];
int nxt[MAXN][MAXN];

int main() {
    scanf("%d", &N);
    for (int i = 0; i < N; i++) scanf("%s", grid[i]);

    // a = 0 时左端点和右端点重合在中心格 (r, N-1-r) 上, 长度为 1 的串必是回文
    for (int r = 0; r < N; r++) dp[r][r] = 1;

    for (int a = 0; a < N - 1; a++) {
        for (int r1 = 0; r1 < N; r1++)
            for (int r2 = 0; r2 < N; r2++) nxt[r1][r2] = 0;
        for (int r1 = 0; r1 <= N - 1 - a; r1++) {
            int c1 = N - 1 - a - r1; // 左端点在第 a 层反对角线上
            for (int r2 = a; r2 < N; r2++) {
                int cur = dp[r1][r2];
                if (cur == 0) continue;
                int c2 = N - 1 + a - r2; // 右端点在同一条反对角线的镜像位置上
                // 左端点向外走一步: 向左 (r1, c1-1) 或向上 (r1-1, c1)
                for (int d1 = 0; d1 < 2; d1++) {
                    int nr1 = r1 - d1;
                    int nc1 = c1 - 1 + d1;
                    if (nr1 < 0 || nc1 < 0) continue;
                    // 右端点向外走一步: 向右 (r2, c2+1) 或向下 (r2+1, c2)
                    for (int d2 = 0; d2 < 2; d2++) {
                        int nr2 = r2 + d2;
                        int nc2 = c2 + 1 - d2;
                        if (nr2 >= N || nc2 >= N) continue;
                        // 回文要求新加入的两端字符相同
                        if (grid[nr1][nc1] != grid[nr2][nc2]) continue;
                        int &t = nxt[nr1][nr2];
                        t += cur;
                        if (t >= MOD) t -= MOD;
                    }
                }
            }
        }
        for (int r1 = 0; r1 < N; r1++)
            for (int r2 = 0; r2 < N; r2++) dp[r1][r2] = nxt[r1][r2];
    }

    // 展开到最外层后左端点只能是 (0,0), 右端点只能是 (N-1,N-1)
    printf("%d\n", dp[0][N - 1]);
    return 0;
}
