#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 105;      // n 的上限
const int MAXK = 8;        // k 的上限 (k <= 7)
const long long INF = 4000000000000000000LL;

int n, k;
long long r[MAXN];         // 每个房间最终要容纳的牛数
long long a[MAXN];         // 旋转到切点之后的 r
long long dp[MAXK][MAXN];  // dp[t][i]: 从第 i 个房间起还可用 t 道门(且第 i 个房间用门)的最小总距离

int main() {
    scanf("%d %d", &n, &k);
    for (int i = 0; i < n; i++) {
        scanf("%lld", &r[i]);
    }

    long long ans = INF;
    // 枚举"切点": 假想第 s 个房间左边那段栅栏外没有牛经过, 于是可以把它摊成一条链,
    // 并且第 s 个房间必须自己开门(否则房间 s 的牛无处可来)。这样每种最优方案都会被枚举到。
    for (int s = 0; s < n; s++) {
        for (int i = 0; i < n; i++) {
            a[i] = r[(s + i) % n];
        }

        // dp[t][i] = min over e in [i+1, n] of (a[i..e-1] 都从门口 i 走过来的距离 + dp[t-1][e])
        // 其中 dp[0][n] = 0 表示"已经覆盖到末尾、门还没用完"这件事只能由最后一道门负责,
        // 因此 t >= 1 时 dp[t][n] = INF, 保证恰好用 k 道门。
        for (int t = 0; t <= k; t++) {
            for (int i = 0; i <= n; i++) {
                dp[t][i] = INF;
            }
        }
        dp[0][n] = 0;
        for (int t = 1; t <= k; t++) {
            for (int i = 0; i < n; i++) {
                long long val = 0;   // 房间 i..e-1 从门口 i 走到各自房间的总距离
                for (int e = i + 1; e <= n; e++) {
                    // 先把房间 e-1 加进来: 它离门口 i 有 e-1-i 步
                    val += a[e - 1] * (e - 1 - i);
                    if (dp[t - 1][e] < INF && val + dp[t - 1][e] < dp[t][i]) {
                        dp[t][i] = val + dp[t - 1][e];
                    }
                }
            }
        }
        if (dp[k][0] < ans) ans = dp[k][0];
    }
    printf("%lld\n", ans);
    return 0;
}
