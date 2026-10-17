#include <cstdio>

using namespace std;

const int MAXN = 100005;
const int MAXA = 26;
const int INF = 1000000000;

int N, M, K;
char S[MAXN];               // 原串
int d[MAXA][MAXA];          // 两个字母之间转换的最短代价（先跑一遍 Floyd）

// ps[i][j]：把前 i 个位置全部改成字母 j 的花费
int ps[MAXN][MAXA];
// dp[i][j]：前 i 位已经合法、且末尾连续 K 位都是字母 j 的最小花费
// mn[i]：min over j of dp[i][j]，另外规定 mn[0] = 0 表示空前缀
int dp[MAXN][MAXA];
int mn[MAXN];

int main() {
    scanf("%d %d %d", &N, &M, &K);
    scanf("%s", S);
    for (int i = 0; i < M; i++)
        for (int j = 0; j < M; j++) scanf("%d", &d[i][j]);

    // 转换可以借道中间字母，故先求任意两字母间的最短路
    for (int c = 0; c < M; c++)
        for (int a = 0; a < M; a++)
            for (int b = 0; b < M; b++)
                if (d[a][c] + d[c][b] < d[a][b]) d[a][b] = d[a][c] + d[c][b];

    for (int i = 1; i <= N; i++)
        for (int j = 0; j < M; j++)
            ps[i][j] = ps[i - 1][j] + d[S[i - 1] - 'a'][j];

    for (int j = 0; j < M; j++) dp[0][j] = INF;     // 长度 0 不可能以某个字母的“段”结尾
    mn[0] = 0;                                      // 但空前缀本身是合法的起点

    for (int i = 1; i <= N; i++) {
        mn[i] = INF;
        for (int j = 0; j < M; j++) {
            int best = INF;
            // 情况一：最后一段（全是 j）长度超过 K，可以由前 i-1 位延长而来
            if (dp[i - 1][j] < INF) best = dp[i - 1][j] + d[S[i - 1] - 'a'][j];
            // 情况二：最后一段长度正好是 K，前 i-K 位任意合法方案后面接上 K 个 j
            if (i >= K && mn[i - K] < INF) {
                int cand = ps[i][j] - ps[i - K][j] + mn[i - K];
                if (cand < best) best = cand;
            }
            dp[i][j] = best;
            if (best < mn[i]) mn[i] = best;
        }
    }

    printf("%d\n", mn[N]);
    return 0;
}
