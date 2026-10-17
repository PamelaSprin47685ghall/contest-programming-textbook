#include <cstdio>

using namespace std;

const int MAXN = 1005;   // N, M 的上限
const long long INF = 4000000000000000000LL;

int n, m;
int fx, fy, bx, by;
char sf[MAXN];           // FJ 的路径字符串
char sb[MAXN];           // Bessie 的路径字符串
int px[MAXN], py[MAXN];  // FJ 走完前 i 步后的坐标
int qx[MAXN], qy[MAXN];  // Bessie 走完前 j 步后的坐标
// dp[i][j]: FJ 走到第 i 个位置、Bessie 走到第 j 个位置时的最小能耗
long long dp[MAXN][MAXN];

int main() {
    scanf("%d %d", &n, &m);
    scanf("%d %d", &fx, &fy);
    scanf("%d %d", &bx, &by);
    scanf("%s", sf);
    scanf("%s", sb);

    px[0] = fx;
    py[0] = fy;
    for (int i = 0; i < n; i++) {
        int dx = 0, dy = 0;
        if (sf[i] == 'N') dy = 1;
        else if (sf[i] == 'S') dy = -1;
        else if (sf[i] == 'E') dx = 1;
        else dx = -1;
        px[i + 1] = px[i] + dx;
        py[i + 1] = py[i] + dy;
    }
    qx[0] = bx;
    qy[0] = by;
    for (int j = 0; j < m; j++) {
        int dx = 0, dy = 0;
        if (sb[j] == 'N') dy = 1;
        else if (sb[j] == 'S') dy = -1;
        else if (sb[j] == 'E') dx = 1;
        else dx = -1;
        qx[j + 1] = qx[j] + dx;
        qy[j + 1] = qy[j] + dy;
    }

    // 每个时刻两人各自前进或原地不动, 所以 (i,j) 只能由 (i-1,j)、(i,j-1)、(i-1,j-1) 转移来;
    // 时间步的能量按"两人到达该状态后的距离"计入, 起点那一步不计。
    dp[0][0] = 0;
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            if (i == 0 && j == 0) continue;
            long long dx = px[i] - qx[j];
            long long dy = py[i] - qy[j];
            long long cost = dx * dx + dy * dy;
            long long best = INF;
            if (i > 0 && dp[i - 1][j] < best) best = dp[i - 1][j];
            if (j > 0 && dp[i][j - 1] < best) best = dp[i][j - 1];
            if (i > 0 && j > 0 && dp[i - 1][j - 1] < best) best = dp[i - 1][j - 1];
            dp[i][j] = best + cost;
        }
    }

    printf("%lld\n", dp[n][m]);
    return 0;
}
