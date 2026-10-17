#include <cstdio>

using namespace std;

const int MAXN = 1000000;
const long long MOD = 1000000007;

// prefix[n] = dp(1) + dp(2) + ... + dp(n), dp 是不存在连续 K 个同色的方案数
int prefix[MAXN + 1];

int main() {
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);

    // n < K 时没有长度 K 的区间, 随便涂, dp(n) = M^n, 所以前缀和递推是 prefix(n) = M*prefix(n-1) + M
    prefix[0] = 0;
    for (int i = 1; i < k && i <= n; i++) {
        prefix[i] = (int)(((long long)m * prefix[i - 1] + m) % MOD);
    }
    // 由 dp(n) = (M-1) * (prefix(n-1) - prefix(n-K)) 推出 prefix(n) = M*prefix(n-1) - (M-1)*prefix(n-K)
    for (int i = k; i <= n; i++) {
        long long value = (long long)m * prefix[i - 1] % MOD
                          - (long long)(m - 1) * prefix[i - k] % MOD;
        value %= MOD;
        if (value < 0) {
            value += MOD;
        }
        prefix[i] = (int)value;
    }

    long long allPaintings = 1;
    for (int i = 1; i <= n; i++) {
        allPaintings = allPaintings * m % MOD;
    }

    // 合法画作 = 全部画作 - 不存在连续 K 个同色的画作 = M^N - dp(N), 而 dp(N) = prefix(N) - prefix(N-1)
    long long answer = (allPaintings - prefix[n] + prefix[n - 1]) % MOD;
    if (answer < 0) {
        answer += MOD;
    }
    printf("%lld\n", answer);
    return 0;
}
