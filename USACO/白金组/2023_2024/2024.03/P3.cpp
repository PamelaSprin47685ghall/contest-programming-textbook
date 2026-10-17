#include <cstdio>
#include <algorithm>
using namespace std;

// R <= 20，要放置的机器人最多 19 个，状态数为 2^19 * 19 左右
const int MAXM = 1 << 19;
const long long INF = (long long)4e18;

int L, R, N, K;
int a[100005];          // 激活点（读入后排序）

// dp[mask][i]：已经放置了 mask 中的机器人、且刚刚走到第 i 个待放置机器人的位置上
// （还没有等待激活点）所花的最少时间，未到达的状态为 INF。
long long dp[MAXM][19];
// hop[d]：在旋转参考系里沿环走过 d 段弧（弧长 d*L/R）的最短时间；
// 后面要把下标差 j-i 展开成 j-i+R，故表格开到 2R 大小，省去每次转移取模。
long long hop[40];

long long ceilDiv(long long x, long long y) {
    return (x + y - 1) / y;
}

// 关键转化：把所有机器人一起"扣掉"它们的逆时针位移，即让参考系随机器人逆时针自转，
// 于是机器人在参考系里静止，激活点每 K 秒顺时针移动 1 个单位。
// 在这个参考系里我们逆时针速度是 1 - 1/K = (K-1)/K，顺时针速度是 1 + 1/K = (K+1)/K。
// 两点之间的最短行走时间就是两个方向取小者（两点之间总能选到较快的那个方向）。
long long travel(int x, int y) {
    int cw = (x - y + L) % L;                        // 从 y 顺时针走到 x 的弧长
    long long best = ceilDiv((long long)K * cw, K + 1);
    if (K > 1) {                                     // K = 1 时逆时针速度为 0，不能走
        long long other = ceilDiv((long long)K * (L - cw), K - 1);
        if (other < best) best = other;
    }
    return best;
}

// 站在第 i 个待放置机器人的位置（参考系里固定在 (L/R)*(i+1)）上、当前时刻为 t 时，
// 求最早的不早于 t 的时刻，使得某个激活点恰好经过该位置。
// 激活点 a 在时刻 t' 位于 a - t'/K，要求 a - t'/K ≡ pos (mod L)，
// 也就是 t' 必须是 K 的整数倍 t' = K*m，且 m ≡ a - pos (mod L)。
// 于是取 m 为 ceil(t/K) 加上"环上从 pos+m 走到下一个激活点"的顺时针距离。
long long alignTime(int i, long long t) {
    long long m = ceilDiv(t, K);                     // 到目前为止已经过去了多少整段 K 秒
    int residue = (int)(((long long)(L / R) * (i + 1) + m) % L);  // 激活点的下标必须恰为这个值
    int lo = 0, hi = N;
    while (lo < hi) {                                // 找第一个 >= residue 的激活点
        int mid = (lo + hi) >> 1;
        if (a[mid] < residue) lo = mid + 1;
        else hi = mid;
    }
    int d = (lo == N ? L + a[0] : a[lo]) - residue;  // 环上顺时针还需走过的距离
    return (long long)K * (m + d);
}

int main() {
    scanf("%d%d%d%d", &L, &R, &N, &K);
    for (int i = 0; i < N; i++) scanf("%d", &a[i]);
    sort(a, a + N);

    for (int d = 0; d < R; d++) hop[d] = travel(0, (L / R) * d);
    // 位置下标差 j-i 可能为负，展开成 2R 长度即可省掉取模
    for (int d = 0; d < R; d++) hop[d + R] = hop[d];

    int cnt = R - 1;                 // 待放置的机器人数（原机器人已经在 0 号位置了）
    int full = (1 << cnt) - 1;
    for (int mask = 0; mask <= full; mask++)
        for (int i = 0; i < cnt; i++) dp[mask][i] = INF;
    // 起点就是原机器人的位置 0，所以初始只需花"走到第 i 个机器人位置"的时间，
    // 等待对齐统一交给主循环处理。
    for (int i = 0; i < cnt; i++) dp[1 << i][i] = hop[i + 1];

    long long ans = INF;
    for (int mask = 1; mask <= full; mask++) {
        for (int i = 0; i < cnt; i++) {
            if (!(mask >> i & 1)) continue;
            // 走到第 i 个机器人的位置后必须等到有激活点经过才能真的放下它。
            // 官方做法要在每条转移边里都做一次二分；把"等待"推迟到取出状态时统一做，
            // 因为等待时间关于到达时间单调不减，先取最小再对齐与先对齐再取最小等价。
            long long t = alignTime(i, dp[mask][i]);
            if (mask == full) {                  // 全部放好，最后落点的对齐时间就是答案
                if (t < ans) ans = t;
                continue;
            }
            for (int j = 0; j < cnt; j++) {
                if (mask >> j & 1) continue;
                long long cand = t + hop[j - i + R];     // 第 i 与第 j 个机器人位置间的绕环距离
                long long &slot = dp[mask | (1 << j)][j];
                if (cand < slot) slot = cand;
            }
        }
    }
    printf("%lld\n", ans);
    return 0;
}
