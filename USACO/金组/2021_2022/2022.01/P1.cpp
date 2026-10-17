#include <cstdio>

using namespace std;

const int MAXN = 105;
const int MAXH = 1005;
const int MOD = 1000000007;

int N;
int H[MAXN];
long long dp[MAXH], ndp[MAXH], pre[MAXH];

// 设第 i 对相邻奶牛 (i, i+1) 被喂了 c_i 袋玉米(i = 1..N-1, c_0 = c_N = 0),
// 最终共同饥饿值为 v. 于是 h_i = c_{i-1} + c_i + v, 每个 c_i >= 0, v >= 0.
// 反过来, 给定 (v, c) 就能唯一确定 h, 所以只需数满足 h_i <= H_i 的 (v, c) 对数,
// 再按可行性把重复计数的部分去掉:
//   N 为偶数: (v, c) 可行则 (0, c') 也可行(c' 由 c 平移得到), 故只数 v = 0 的一遍;
//   N 为奇数: 对 h 取交替和可得 v 恰好等于它, 即 v 被唯一确定, 不会重复, 直接枚举 v 累加.
int main() {
    scanf("%d", &N);
    int minH = MAXH, maxH = 0;
    for (int i = 1; i <= N; ++i) {
        scanf("%d", &H[i]);
        if (H[i] < minH) minH = H[i];
        if (H[i] > maxH) maxH = H[i];
    }
    int vMax = (N % 2 == 0) ? 0 : minH;
    long long answer = 0;
    for (int v = 0; v <= vMax; ++v) {
        for (int c = 0; c <= maxH; ++c) dp[c] = 0;
        dp[0] = 1;  // 边界: c_0 = 0
        int lim = 0;  // 当前 c_{i-1} 的可能最大值, 超过它的状态恒为 0
        bool ok = true;
        for (int i = 1; i <= N; ++i) {
            // h_i = c_{i-1} + c_i + v <= H_i 意味着 c_i <= H_i - v 且 c_{i-1} <= H_i - v
            int top = H[i] - v;
            if (top < 0) {
                ok = false;
                break;
            }
            long long s = 0;
            for (int t = 0; t <= top; ++t) {
                if (t <= lim) {
                    s += dp[t];
                    if (s >= MOD) s -= MOD;
                }
                pre[t] = s;  // pre[t] = dp[0] + ... + dp[min(t, lim)]
            }
            for (int c = 0; c <= top; ++c) ndp[c] = pre[top - c];
            for (int c = 0; c <= top; ++c) dp[c] = ndp[c];
            lim = top;
        }
        if (ok) {
            // 收尾要求 c_N = 0
            answer += dp[0];
            if (answer >= MOD) answer -= MOD;
        }
    }
    printf("%lld\n", answer);
    return 0;
}
