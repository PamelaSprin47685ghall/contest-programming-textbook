#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 5005;
const int MAXM = 100005;
const int INF = 1000000000;

int n, m;
int x[MAXN];     // 排序后的奶牛坐标
int cost[MAXM];  // cost[w]: 买一把宽度至少为 w 的伞的最低价
int dp[MAXN];    // dp[i]: 盖住排序后前 i 头牛的最小花费

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; i++) scanf("%d", &x[i]);
    sort(x, x + n);
    for (int w = 1; w <= m; w++) scanf("%d", &cost[w]);

    // 伞允许重叠, 所以多出的宽度可以盖在空牛栏上:
    // 从后往前取后缀最小值, 把 "大伞反而便宜" 的情况折价进来.
    for (int w = m - 1; w >= 1; w--) cost[w] = min(cost[w], cost[w + 1]);

    dp[0] = 0;
    for (int i = 1; i <= n; i++) {
        dp[i] = INF;
        for (int j = 0; j < i; j++) {
            // 最后一头伞盖住排序后第 j 到第 i-1 头牛, 需要宽度 x[i-1] - x[j] + 1
            int width = x[i - 1] - x[j] + 1;
            dp[i] = min(dp[i], dp[j] + cost[width]);
        }
    }

    printf("%d\n", dp[n]);
    return 0;
}
