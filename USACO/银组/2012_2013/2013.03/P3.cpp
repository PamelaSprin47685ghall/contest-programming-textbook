#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 1005;
const long long INF = 1LL << 60;

int pos[MAXN];  // 所有奶牛的位置, 再加上起点 0

// dp[l][r][0]: 已经给排序后第 l..r 头"牛"套上绳套, 人站在左端 pos[l] 时的最小累计损失;
// dp[l][r][1] 表示站在右端 pos[r]. 因为路是连续走的, 套好的牛在排序后一定是连续一段.
long long dp[MAXN][MAXN][2];

int main() {
    int n;
    scanf("%d", &n);
    pos[0] = 0;  // 起点 0 看成一头一开始就套好的虚拟牛, 这样区间一定包含下标 0 那头
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &pos[i]);
    }
    sort(pos, pos + n + 1);

    int total = n + 1;
    int start = 0;  // 虚拟牛排序后的下标
    for (int i = 0; i < total; ++i) {
        if (pos[i] == 0) {
            start = i;
        }
    }

    for (int l = 0; l < total; ++l) {
        for (int r = 0; r < total; ++r) {
            dp[l][r][0] = dp[l][r][1] = INF;
        }
    }
    dp[start][start][0] = dp[start][start][1] = 0;

    // 一头牛没套上绳套时每分钟损失 1, 所以走这段路的代价 = 距离 * 还没套上的牛数.
    // 区间长度从小到大递推, 每次只把区间向左或向右扩一头.
    for (int len = 1; len < total; ++len) {
        for (int l = 0; l + len - 1 < total; ++l) {
            int r = l + len - 1;
            long long remaining = n - (len - 1);  // 区间里除了虚拟牛还有 len-1 头真牛已处理
            for (int side = 0; side < 2; ++side) {
                long long cur = dp[l][r][side];
                if (cur >= INF) {
                    continue;
                }
                int at = (side == 0) ? pos[l] : pos[r];  // 人现在的位置
                if (l > 0) {
                    long long cost = cur + (long long)(at - pos[l - 1]) * remaining;
                    if (cost < dp[l - 1][r][0]) {
                        dp[l - 1][r][0] = cost;
                    }
                }
                if (r + 1 < total) {
                    long long cost = cur + (long long)(pos[r + 1] - at) * remaining;
                    if (cost < dp[l][r + 1][1]) {
                        dp[l][r + 1][1] = cost;
                    }
                }
            }
        }
    }

    long long answer = min(dp[0][total - 1][0], dp[0][total - 1][1]);
    printf("%lld\n", answer);
    return 0;
}
