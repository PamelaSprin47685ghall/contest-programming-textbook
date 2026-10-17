#include <cstdio>

using namespace std;

const long long MOD = 1000000007LL;
const int MAXN = 5000010;

// 逆元表：组合数递推时每次除的数不超过 N+1，故只求到 N+1
static int invTab[MAXN];
static int cpos[105], cval[105];   // 记住的位置与数值
// dp[i][0]：前 c_i 头牛的最大值恰为 v_i；dp[i][1]：恰为 v_i+1
static long long dp[105][2];

int main() {
    int n, q;
    if (scanf("%d %d", &n, &q) != 2) return 0;
    for (int i = 0; i < q; i++) scanf("%d %d", &cpos[i], &cval[i]);

    // O(N) 线性递推求逆元：inv[i] = -(MOD/i) * inv[MOD%i] (mod MOD)。
    // 若对每个数用费马小定理，N = 5e6 时 O(N log MOD) 太慢。
    int lim = n + 1;
    invTab[1] = 1;
    for (int i = 2; i <= lim; i++)
        invTab[i] = (int)(MOD - MOD / i * invTab[MOD % i] % MOD);

    // 合法数组的充要条件：对任意 i < j 有 a_j >= a_i - 1，
    // 即每个位置要么放进一个 >= 当前最大值的数（它成为新的最大值），
    // 要么被强制取 当前最大值 - 1。于是只需数那些"前缀最大值"构成的非降子序列。
    dp[0][0] = 1;
    for (int i = 0; i + 1 < q; i++) {
        int k = cpos[i + 1] - cpos[i] - 1;                  // 两段已知值之间的自由位置数
        long long v = (long long)cval[i + 1] - cval[i] + 1; // 可用取值区间长度（平移后）
        if (v < 0) continue;                                // 固定值差超过 1，这段怎么填都不合法

        long long s00 = 0, s01 = 0, s10 = 0, s11 = 0;
        // 用 C(n,k) = C(n-1,k-1) * n / k 从上一轮 j-1 的值推出 j 时的值，
        // 从而避免求大数阶乘（值域 1e9 无法打表）。
        long long eqv = 1, eqvp = 1, ltv = 1, ltvp = 1;     // 上一轮（j-1）的组合数值
        long long kc = 1;                                   // C(k, j)
        for (int j = 0; j <= k; j++) {
            // arr_eq(x, j) = C(x+j-2, j-1)：长度 j、取值 <= x 且末项恰为 x 的非降序列数
            // arr_lt(x, j) = C(x+j-2, j)  ：长度 j、取值 <= x-1 的非降序列数
            long long t1 = v + j - 2;      // v=0 且 j=1 时会取到 -1
            long long t2 = t1 + 1;
            if (t1 < 0) t1 += MOD;
            if (t2 < 0) t2 += MOD;
            if (j == 0) {
                eqv = 0; eqvp = 0; ltv = 1; ltvp = 1;
            } else if (j == 1) {
                eqv = 1; eqvp = 1; ltv = t1; ltvp = t2;
            } else {
                eqv = eqv * t1 % MOD * invTab[j - 1] % MOD;
                eqvp = eqvp * t2 % MOD * invTab[j - 1] % MOD;
                ltv = ltv * t1 % MOD * invTab[j] % MOD;
                ltvp = ltvp * t2 % MOD * invTab[j] % MOD;
            }
            // 选出哪 j 个自由位置作为"抬升最大值"的位置：C(k, j) 种
            s11 = (s11 + eqv * kc) % MOD;
            s10 = (s10 + ltv * kc) % MOD;
            s01 = (s01 + eqvp * kc) % MOD;
            s00 = (s00 + ltvp * kc) % MOD;
            kc = kc * (k - j) % MOD * invTab[j + 1] % MOD;  // C(k, j+1) = C(k,j)*(k-j)/(j+1)
        }
        if (v == 0) {
            // v_{i+1} = v_i - 1：前后状态 0 都不可能，只能落到状态 1（最大值停在 v_i）
            s01 = (s01 + s11 + s10) % MOD;
            s11 = 0; s10 = 0; s00 = 0;
        } else if (v == 1) {
            // v_{i+1} = v_i：从状态 1 出发最大值不会再掉到 v_{i+1}，1 -> 0 并入 1 -> 1
            s11 = (s11 + s10) % MOD;
            s10 = 0;
        }
        dp[i + 1][0] = (dp[i][0] * s00 + dp[i][1] * s10) % MOD;
        dp[i + 1][1] = (dp[i][0] * s01 + dp[i][1] * s11) % MOD;
    }
    printf("%lld\n", (dp[q - 1][0] + dp[q - 1][1]) % MOD);
    return 0;
}
