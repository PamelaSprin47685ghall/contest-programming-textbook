#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 2005;

long long pos[MAXN];  // 排序后的奶牛坐标
// dp[i]: 覆盖前 i 头牛的最小花费. 费用可能是 x.5, 所以统一存两倍费用, 全程用整数计算.
long long dp[MAXN];

int main() {
    int n;
    long long a, b;
    scanf("%d%lld%lld", &n, &a, &b);
    for (int i = 0; i < n; ++i) {
        scanf("%lld", &pos[i]);
    }
    sort(pos, pos + n);

    dp[0] = 0;
    for (int i = 1; i <= n; ++i) {
        dp[i] = -1;  // 还没算过
        for (int j = 0; j < i; ++j) {
            // 用一座基站覆盖第 j..i-1 头牛: 基站放区间中点, 半径 = 区间长度的一半,
            // 两倍花费 = 2A + B * (区间长度).
            long long cost2 = dp[j] + 2 * a + b * (pos[i - 1] - pos[j]);
            if (dp[i] < 0 || cost2 < dp[i]) dp[i] = cost2;
        }
    }

    // 两倍花费为偶数时答案是整数, 否则答案是 x.5
    if (dp[n] % 2 == 0) {
        printf("%lld\n", dp[n] / 2);
    } else {
        printf("%lld.5\n", dp[n] / 2);
    }
    return 0;
}
