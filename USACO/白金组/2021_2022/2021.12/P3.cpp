#include <cstdio>

using namespace std;

const int MAXN = 5000;
const long long MOD = 1000000007;

int n, x;
int y;                          // 大于 Bessie 心里的数的取值个数
long long inverse[MAXN + 1];    // 1..n 在模 MOD 下的逆元

// 期望值 dp[0][j][k] / dp[1][j][k]：还剩 j 个更小的"有用"取值、k 个更大的，
// 且 Bessie 最近一次说 LO（0）或 HI（1）时，之后还会说出的 HILO 的期望次数。
// 递推里只用到两个前缀和，所以不存整张表：
// sumHiInRow[j] = 固定 j 时，k 更小的 dp[1][j][k] 之和
// sumLoInColumn[k] = 固定 k 时，j 更小的 dp[0][j][k] 之和
long long sumHiInRow[MAXN + 1];
long long sumLoInColumn[MAXN + 1];

long long powerMod(long long base, long long exponent) {
    long long result = 1;
    base %= MOD;
    while (exponent > 0) {
        if (exponent & 1) result = result * base % MOD;
        base = base * base % MOD;
        exponent >>= 1;
    }
    return result;
}

int main() {
    scanf("%d%d", &n, &x);
    y = n - x;
    for (int i = 1; i <= n; ++i) inverse[i] = powerMod(i, MOD - 2);

    long long expectLastLo = 0; // dp[0][j][k]
    long long expectLastHi = 0; // dp[1][j][k]
    for (int j = 0; j <= x; ++j) {
        for (int k = 0; k <= y; ++k) {
            long long sumSmaller = (sumHiInRow[j] + sumLoInColumn[k]) % MOD;
            // j + k > 0 时，下一个有用取值在两类里等可能，各占 1/(j+k)；
            // j + k = 0 时已经没有取值可选，期望值为 0。
            long long share = (j + k == 0) ? 0 : inverse[j + k];
            expectLastLo = sumSmaller * share % MOD;
            // 刚说过 HI，若下一个取值落在更小的一侧（概率 j/(j+k)），就多出一个 HILO
            expectLastHi = (sumSmaller + j) % MOD * share % MOD;
            sumHiInRow[j] = (sumHiInRow[j] + expectLastHi) % MOD;
            sumLoInColumn[k] = (sumLoInColumn[k] + expectLastLo) % MOD;
        }
    }

    // 所有排列等可能，共 n! 个，初始状态是 j = x、k = y、最近一次说 LO
    long long answer = expectLastLo;
    for (int i = 1; i <= n; ++i) answer = answer * i % MOD;
    printf("%lld\n", answer);
    return 0;
}
