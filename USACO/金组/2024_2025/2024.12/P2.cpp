#include <cstdio>

using namespace std;

const int MAXN = 500005;
const int MOD = 1000000007;

int N;
char s[MAXN]; // 1 号下标开始存放偏好字符串

int lastR[MAXN];  // lastR[i]：<= i 的最后一个 'R' 的位置（0 表示没有）
int lastB[MAXN];  // lastB[i]：<= i 的最后一个 'B' 的位置（0 表示没有）
int firstB[MAXN]; // firstB[i]：>= i 的第一个 'B' 的位置（N+1 表示没有）

long long dp[MAXN];           // dp[i]：前 i 个点的合法染色方案数
long long paritySum[2][MAXN]; // paritySum[p][i]：下标与 p 同奇偶的所有 t <= i 的 dp[t] 之和

// 取 dp[t] 之和，t 落在 [lo, hi] 内且与 i 同奇偶（同奇偶才能由一个区间直接跨过来）
long long rangeSum(int lo, int hi, int i) {
    if (lo < 0) {
        lo = 0;
    }
    if (lo % 2 != i % 2) {
        ++lo;
    }
    if (hi % 2 != i % 2) {
        --hi;
    }
    if (lo > hi) {
        return 0;
    }
    long long res = paritySum[i % 2][hi] - (lo > 0 ? paritySum[i % 2][lo - 1] : 0);
    return (res + MOD) % MOD;
}

int main() {
    scanf("%d", &N);
    scanf("%s", s + 1);

    for (int i = 1; i <= N; ++i) {
        lastR[i] = (s[i] == 'R') ? i : lastR[i - 1];
        lastB[i] = (s[i] == 'B') ? i : lastB[i - 1];
    }
    firstB[N + 1] = N + 1; // 哨兵：表示没有
    for (int i = N; i >= 1; --i) {
        firstB[i] = (s[i] == 'B') ? i : firstB[i + 1];
    }

    dp[0] = 1;
    paritySum[0][0] = 1;

    for (int i = 1; i <= N; ++i) {
        // 第 i 个点留白：只有 s[i] = 'X' 时才允许
        if (s[i] == 'X') {
            dp[i] = dp[i - 1];
        }
        // 蓝半边 [i-x+1, i] 里不能有 R，这给出 x 的上界
        int cutoffX = i - lastR[i];
        int x = 1;
        while (2 * x <= i && x <= cutoffX) {
            int lRed = i - 2 * x + 1; // 红半边左端点
            int j = firstB[lRed];     // 红半边里最靠左的 B
            if (j <= i - x) {
                // 这个 B 落在红半边里，x 不能用；把 x 抬到让 j 落进蓝半边，
                // 中间这一段 x 全是坏的（同一个 B 一直卡在红半边内）
                x = i - j + 1;
            } else {
                // x 现在可用。红半边继续往左扩时会碰到 lRed 左边的最后一个 B，
                // 记它的位置为 j2：只要 x' <= (i - j2) / 2，这个 B 就还在红半边之外。
                int j2 = lastB[lRed - 1];
                int tLow = i - 2 * ((i - j2) / 2);
                int limitLow = i - 2 * cutoffX; // 蓝半边不能含 R 等价于 t >= i - 2*cutoffX
                if (tLow < limitLow) {
                    tLow = limitLow;
                }
                dp[i] = (dp[i] + rangeSum(tLow, i - 2 * x, i)) % MOD;
                x = (i - j2) / 2 + 1;
            }
        }
        paritySum[0][i] = paritySum[0][i - 1];
        paritySum[1][i] = paritySum[1][i - 1];
        paritySum[i % 2][i] = (paritySum[i % 2][i] + dp[i]) % MOD;
    }

    printf("%lld\n", dp[N]);
    return 0;
}
