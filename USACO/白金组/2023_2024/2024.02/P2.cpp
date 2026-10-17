#include <cstdio>
#include <algorithm>
using namespace std;

// 《Minimum Sum of Maximums》(feb24 platinum P2)  做法说明见 P2.md
//
// 两端各补一个固定哨兵 1000000，把序列切成 K+1 个"段"。
// 段内非固定值升序摆放最优，于是答案 = 所有非固定值之和 + 各段大端点之和
//        + 两个哨兵之和 + 若干"端点修正量" - 2*1000000，
// 其中修正量只与每段被分到的值和该段两个固定端点有关，用区间 DP 求最小值。

const int MAXN = 305;      // 位置数 / 非固定值个数上限 (N <= 300)
const int MAXM = 128;      // 段数 <= K+1 <= 7，掩码规模 2^7
const int INF = 1000000000;

int a[MAXN];               // 每个位置的 ugliness
int stuck[MAXN];           // 该位置是否被固定
int fixedPos[8];           // 固定位置，输入保证升序
int vals[MAXN];            // 所有非固定值，升序
int M;                     // 非固定值个数 = N - K

int rmn[8], rmx[8], rlen[8];   // 每段的两个端点值(小/大)与段长
int rcnt;                      // 段长 > 0 的段数
int maskLen[MAXM];             // 每个掩码所含段长之和

// dp[L][R+1][mask]：只用 vals[L..R] 里的值填满 mask 中的各段所需的最小修正量
// (R 用 R+1 存储，便于表示空区间)
int dp[MAXN][MAXN][MAXM];

int main() {
    int N, K;
    scanf("%d %d", &N, &K);
    for (int i = 1; i <= N; i++) scanf("%d", &a[i]);
    for (int i = 0; i < K; i++) scanf("%d", &fixedPos[i]);
    for (int i = 1; i <= N; i++) stuck[i] = 0;
    for (int i = 0; i < K; i++) stuck[fixedPos[i]] = 1;

    M = 0;
    for (int i = 1; i <= N; i++)
        if (!stuck[i]) vals[M++] = a[i];
    sort(vals, vals + M);

    // 哨兵值取 1000000，不小于任何 a_i，故哨兵与相邻固定值的最大端点就是哨兵
    const int SENT = 1000000;
    int pos[9], val[9], tot = 0;
    pos[tot] = 0; val[tot] = SENT; tot++;
    for (int i = 1; i <= N; i++)
        if (stuck[i]) { pos[tot] = i; val[tot] = a[i]; tot++; }
    pos[tot] = N + 1; val[tot] = SENT; tot++;

    long long addon = 0;    // 段长为 0(两个固定值相邻)的段的直接贡献
    long long sumMax = 0;   // 所有段长为正的大端点之和
    rcnt = 0;
    for (int i = 0; i + 1 < tot; i++) {
        int sz = pos[i + 1] - pos[i] - 1;
        int lo = min(val[i], val[i + 1]);
        int hi = max(val[i], val[i + 1]);
        if (sz == 0) {
            addon += hi;
        } else {
            rmn[rcnt] = lo; rmx[rcnt] = hi; rlen[rcnt] = sz; rcnt++;
            sumMax += hi;
        }
    }

    long long sumVals = 0;
    for (int i = 0; i < M; i++) sumVals += vals[i];

    int full = (1 << rcnt) - 1;
    for (int mask = 0; mask <= full; mask++) {
        int s = 0;
        for (int k = 0; k < rcnt; k++)
            if ((mask >> k) & 1) s += rlen[k];
        maskLen[mask] = s;
    }
    for (int L = 0; L <= M; L++)
        for (int R = 0; R <= M; R++)
            for (int mask = 0; mask <= full; mask++)
                dp[L][R][mask] = INF;

    for (int L = M - 1; L >= 0; L--) {
        for (int R = L; R <= M - 1; R++) {
            int rs = R - L + 1;                 // 区间里可用值的个数
            for (int mask = 0; mask <= full; mask++) {
                if (maskLen[mask] > rs) continue;   // 值不够，保持 INF
                int best = INF;
                if (mask == 0) {
                    best = 0;                   // 没有段要填
                } else {
                    // 两端值都空着不用
                    if (maskLen[mask] <= rs - 1) {
                        if (L + 1 <= R && dp[L + 1][R + 1][mask] < best)
                            best = dp[L + 1][R + 1][mask];
                        if (R - 1 >= L && dp[L][R][mask] < best)
                            best = dp[L][R][mask];
                    }
                    // 拆成左右两块互不相交的段集：最优时左边恰好用掉前缀
                    for (int sub = (mask - 1) & mask; sub > 0; sub = (sub - 1) & mask) {
                        int s1 = maskLen[sub];
                        int c1 = dp[L][L + s1][sub];
                        if (c1 >= INF) continue;
                        int c2 = dp[L + s1][R + 1][mask ^ sub];
                        if (c2 >= INF) continue;
                        if (c1 + c2 < best) best = c1 + c2;
                    }
                    // 段 k 的两端正好取 vals[L] 与 vals[R]
                    for (int k = 0; k < rcnt; k++) {
                        if (!((mask >> k) & 1)) continue;
                        if (rlen[k] == 1 && L < R) continue;  // 只有一个值时两端必须重合
                        // 最小值端点是 vals[L]，最大值端点是 vals[R]
                        int c = max(vals[L], rmn[k]) - vals[L]
                              + max(vals[R], rmx[k]) - rmx[k];
                        if ((1 << k) == mask) {
                            if (c < best) best = c;           // 只填这一段，中间值留给外面
                        } else {
                            int inner = dp[L + 1][R][mask ^ (1 << k)];
                            if (inner < INF && c + inner < best) best = c + inner;
                        }
                    }
                }
                dp[L][R + 1][mask] = best;
            }
        }
    }

    long long ans = (M == 0 ? 0 : (long long)dp[0][M][full]);
    ans += sumVals + sumMax + addon - 2LL * SENT;
    printf("%lld\n", ans);
    return 0;
}
