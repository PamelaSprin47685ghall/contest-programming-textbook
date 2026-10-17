#include <cstdio>

using namespace std;

const int MOD = 1000000007;
const int MAXM = 20;
const int MAXS = 1 << MAXM; // 难度掩码总数上限 2^20

int N, M;
int cnt[MAXS];              // 每种"认为该题困难的测试员集合"对应的题目数量
int dp[MAXS];               // 以掩码 b 的题目作为最后一题的方案数
static int sdp[MAXS][MAXM]; // 子集和：sdp[b][j] = 只变动 b 的前 j 位时，所有子掩码的 dp 之和
int ord[100005];            // ord[x] = 从 x 道题中选出并排列任意子集（含空集）的方案数

int main() {
    scanf("%d %d", &N, &M);
    static char buf[100005];
    static int probMask[100005]; // 每道题的困难掩码
    for (int i = 0; i < N; i++) {
        probMask[i] = 0;
    }
    for (int i = 0; i < M; i++) {
        scanf("%s", buf);
        for (int j = 0; j < N; j++) {
            if (buf[j] == 'H') {
                probMask[j] |= 1 << i;
            }
        }
    }
    for (int j = 0; j < N; j++) {
        cnt[probMask[j]]++;
    }

    // ord[x] = 1 + x * ord[x-1]：要么不用这道题，要么把它放在末尾而前面的部分任选
    ord[0] = 1;
    for (int x = 1; x <= N; x++) {
        ord[x] = (int)((1LL + (long long)x * ord[x - 1]) % MOD);
    }

    int full = 1 << M;
    long long ans = 0;
    for (int b = 0; b < full; b++) {
        // sum = 1 + 所有真子掩码的 dp 之和
        // 枚举 b 中每一个二进制位 j：所有"仅在前 j 位与 b 不同"的子掩码之和已存于 sdp[b^(1<<j)][j]
        long long sum = 1;
        for (int j = M - 1; j >= 0; j--) {
            if (b & (1 << j)) {
                sum += sdp[b ^ (1 << j)][j];
                if (sum >= MOD) sum -= MOD;
            }
        }
        long long ways = (ord[cnt[b]] - 1 + MOD) % MOD; // 至少选一道该掩码的题并排列
        dp[b] = (int)(sum * ways % MOD);
        ans += dp[b];
        if (ans >= MOD) ans -= MOD;

        sdp[b][0] = dp[b];
        for (int j = 0; j < M - 1; j++) {
            sdp[b][j + 1] = sdp[b][j];
            if (b & (1 << j)) {
                sdp[b][j + 1] += sdp[b ^ (1 << j)][j];
                if (sdp[b][j + 1] >= MOD) sdp[b][j + 1] -= MOD;
            }
        }
    }
    printf("%lld\n", ans);
    return 0;
}
