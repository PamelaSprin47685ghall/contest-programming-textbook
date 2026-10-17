#include <cstdio>

using namespace std;

const int MAXN = 1005;
const int MAXM = 2005;
const int MAXT = 1000;      // 走超过 1000 天一定亏本，理由见题解

int N, M;
long long C;
int gain[MAXN];             // 访问城市 i 能赚到的 moonies
int edgeFrom[MAXM], edgeTo[MAXM];
long long dp[2][MAXN];      // 滚动数组：dp[t&1][x] = 走了 t 天、停在 x 城时的最大收益

int main() {
    scanf("%d %d %lld", &N, &M, &C);
    for (int i = 1; i <= N; i++) scanf("%d", &gain[i]);
    for (int i = 0; i < M; i++) scanf("%d %d", &edgeFrom[i], &edgeTo[i]);

    for (int v = 1; v <= N; v++) dp[0][v] = -1;   // -1 表示该状态不可达
    dp[0][1] = 0;

    long long answer = 0;       // 一天都不走，净收益为 0
    for (int t = 1; t <= MAXT; t++) {
        int cur = t & 1, pre = cur ^ 1;
        for (int v = 1; v <= N; v++) dp[cur][v] = -1;
        for (int e = 0; e < M; e++) {
            int a = edgeFrom[e], b = edgeTo[e];
            if (dp[pre][a] < 0) continue;         // 前一天不在 a 城，这条边用不上
            long long cand = dp[pre][a] + gain[b];
            if (cand > dp[cur][b]) dp[cur][b] = cand;
        }
        if (dp[cur][1] >= 0) {                    // 只有回到 1 号城才算完成一次旅行
            long long profit = dp[cur][1] - C * (long long)t * t;
            if (profit > answer) answer = profit;
        }
    }
    printf("%lld\n", answer);
    return 0;
}
