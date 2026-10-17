#include <cstdio>
#include <algorithm>

using namespace std;

const long long MOD = 1000000007LL;
const int MAXQ = 105;       // Q <= 100
const int MAXC = 10005;     // C <= 10^4

long long N;
int Q, C;
// 原始约束，以及「按 h 去重、再按 a 排序」后的约束（下标从 1 开始用）
long long rawA[MAXQ], rawH[MAXQ];
long long A[MAXQ], H[MAXQ];

long long dpPrev[MAXC], dpCur[MAXC], pref[MAXC], powU[MAXC], powW[MAXC];

long long power(long long base, long long exp) {
    long long res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp & 1) res = res * base % MOD;
        base = base * base % MOD;
        exp >>= 1;
    }
    return res;
}

int main() {
    scanf("%lld %d %d", &N, &Q, &C);
    for (int i = 0; i < Q; ++i) scanf("%lld %lld", &rawA[i], &rawH[i]);

    // 同一个 h 可以对应多个 a；因为题面保证存在合法序列，
    // 较小的 a 那条约束（限制区间更长、上界更松）蕴含其余同 h 的约束，所以只留最小的 a。
    int order[MAXQ];
    for (int i = 0; i < Q; ++i) order[i] = i;
    sort(order, order + Q, [&](int x, int y) {
        if (rawH[x] != rawH[y]) return rawH[x] < rawH[y];
        return rawA[x] < rawA[y];
    });
    int m = 0;
    for (int t = 0; t < Q; ++t) {
        int idx = order[t];
        if (t > 0 && rawH[order[t - 1]] == rawH[idx]) continue;  // 同 h 只留最小的 a
        ++m;
        A[m] = rawA[idx];
        H[m] = rawH[idx];
    }
    // 按 a 升序重排；由于存在合法序列，此时各约束的限制区间互不相交
    for (int i = 1; i <= m; ++i) order[i - 1] = i;
    sort(order, order + m, [&](int x, int y) { return A[x] < A[y]; });
    long long sa[MAXQ], sh[MAXQ];
    for (int i = 1; i <= m; ++i) {
        sa[i] = A[order[i - 1]];
        sh[i] = H[order[i - 1]];
    }
    for (int i = 1; i <= m; ++i) {
        A[i] = sa[i];
        H[i] = sh[i];
    }

    // dp[j]：处理到当前约束为止，满足前面所有约束、且 c_{h_cur} = j（也就是前缀最大值 = j）的方案数。
    // 记 S = max_{k <= a_cur} c_k：约束要求 (a_cur, h_cur) 之间的位置都 <= S，且 c_{h_cur} > S。
    // 从上一个约束的 c_{h_prev} = k 转移时：
    //   中间 u = a_cur - h_prev 个位置随便填（1..C），h_cur 之前 w = h_cur - a_cur - 1 个位置要 <= S。
    //   若这 u 个位置都 <= k，则 S = k，贡献 k^{u+w}；
    //   若其中最大值为 t > k，则 S = t，选法数 (t^u - (t-1)^u)、贡献再乘 t^w。
    dpPrev[0] = 1;
    for (int j = 1; j <= C; ++j) dpPrev[j] = 0;

    for (int i = 1; i <= m; ++i) {
        long long u = A[i] - (i == 1 ? 0 : H[i - 1]);
        long long w = H[i] - A[i] - 1;
        for (int t = 0; t <= C; ++t) {
            powU[t] = power(t, u);
            powW[t] = power(t, w);
        }
        pref[0] = dpPrev[0];
        for (int j = 1; j <= C; ++j) {
            pref[j] = (pref[j - 1] + dpPrev[j]) % MOD;
        }
        long long sumB = 0;     // 累积 sum_{k <= j-1} dpPrev[k] * k^{u+w}
        long long sumA = 0;     // 累积 sum_{t <= j-1} (t^u - (t-1)^u) * t^w * sum_{k <= t-1} dpPrev[k]
        dpCur[0] = 0;
        for (int j = 1; j <= C; ++j) {
            dpCur[j] = (sumB + sumA) % MOD;
            long long termB = powU[j] * powW[j] % MOD;
            sumB = (sumB + dpPrev[j] * termB) % MOD;
            long long diff = (powU[j] - powU[j - 1] + MOD) % MOD;
            long long termA = diff * powW[j] % MOD;
            sumA = (sumA + termA * pref[j - 1]) % MOD;
        }
        for (int j = 0; j <= C; ++j) dpPrev[j] = dpCur[j];
    }

    long long total = 0;
    for (int j = 1; j <= C; ++j) total = (total + dpPrev[j]) % MOD;
    // 最后一个约束之后剩下的 N - H[m] 个位置随便填
    total = total * power(C, N - H[m]) % MOD;
    printf("%lld\n", total);
    return 0;
}
