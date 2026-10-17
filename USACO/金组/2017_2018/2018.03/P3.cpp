#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 255;
const int MAXW = 1005;
const long long NEG_INF = -1000000000000000000LL; // 表示该重量状态不可达

int n, w;
int cowWeight[MAXN];
int cowTalent[MAXN];
// dp[k]：总重恰好为 k 时能达到的最大"修正才艺"；k = w 表示总重至少为 w
long long dp[MAXW];

// 判断能否选出总重 >= w 的牛群，使 1000*总才艺 - y*总重 >= 0
bool Doable(int y) {
    for (int k = 0; k <= w; k++) dp[k] = NEG_INF;
    dp[0] = 0;
    for (int i = 1; i <= n; i++) {
        long long value = 1000LL * cowTalent[i] - (long long)y * cowWeight[i];
        // 0/1 背包，重量维从大到小枚举，保证每头牛只用一次
        for (int k = w; k >= 0; k--) {
            if (dp[k] == NEG_INF) continue;
            int nextWeight = k + cowWeight[i];
            if (nextWeight > w) nextWeight = w; // 超过 w 的部分不影响可行性，统一记在 w 上
            if (dp[k] + value > dp[nextWeight]) dp[nextWeight] = dp[k] + value;
        }
    }
    return dp[w] >= 0;
}

int main() {
    scanf("%d %d", &n, &w);
    for (int i = 1; i <= n; i++)
        scanf("%d %d", &cowWeight[i], &cowTalent[i]);

    // 二分答案 y = floor(1000 * 最优比率)：y 越大越难满足，可行性单调
    int lo = 0, hi = 1000 * 250 * 1000 + 1;
    while (hi > lo + 1) {
        int mid = (lo + hi) / 2;
        if (Doable(mid)) lo = mid;
        else hi = mid;
    }
    printf("%d\n", lo);
    return 0;
}
