#include <cstdio>

using namespace std;

const int MAXW = 100005;  // 竖直位移 w 的上界 (w <= N <= 100000)

long long m, n, l, h, b;      // 题目给定的 M, N, L, H 与模数 B
int primeFactor[MAXW][6];     // 每个数的全体不同质因子 (w <= 1e5 时最多 6 个)
int primeCnt[MAXW];

// 统计横向位移 x 在 [lo, hi] 中、且 d | x 的那些 x 的 (m + 1 - x) 之和, 并对 b 取模.
// 横向位移 x 的横幅在 M x N 的格子里有 (m + 1 - x) 种左右摆法.
long long multiplesSum(long long lo, long long hi, long long d) {
    long long kHi = hi / d;              // 把 x 写成 k * d, k 的上界
    long long kLo = (lo + d - 1) / d;    // k 的下界 (向上取整)
    if (kLo > kHi) {
        return 0;
    }
    long long cnt = kHi - kLo + 1;
    long long kSum = (kHi * (kHi + 1) - (kLo - 1) * kLo) / 2;  // kLo + ... + kHi
    long long res = (cnt * (m + 1) - d * kSum) % b;
    if (res < 0) {
        res += b;
    }
    return res;
}

int main() {
    scanf("%lld %lld %lld %lld %lld", &m, &n, &l, &h, &b);

    // 竖直位移 w 只可能取 1..min(N, H); 只需要这些数的质因子, 所以筛到这个上界就够
    int maxW = (int)(n < h ? n : h);
    for (int i = 2; i <= maxW; i++) {
        if (primeCnt[i] == 0) {  // 没有更小的数标记过它, 说明 i 是质数
            for (int j = i; j <= maxW; j += i) {
                primeFactor[j][primeCnt[j]] = i;
                primeCnt[j]++;
            }
        }
    }

    // 一条横幅的本原位移 (dx, dy) 满足 gcd(dx, dy) = 1 且 L^2 <= dx^2 + dy^2 <= H^2.
    // 统计时按 dy = w > 0, dx > 0 算一遍, 再乘 2 得到 dy < 0 的那一半 (上下对称).
    long long ans = 0;
    long long lo = l;  // 满足 lo^2 + w^2 >= L^2 的最小 dx
    long long hi = h;  // 满足 hi^2 + w^2 <= H^2 的最大 dx
    for (long long w = 1; w <= maxW; w++) {
        // w 增大时 dx 的上下界只会变小, 所以用两个单调递减的指针慢慢挪, 均摊 O(H)
        while (lo > 1 && (lo - 1) * (lo - 1) >= l * l - w * w) {
            lo--;
        }
        while (hi >= 1 && hi * hi > h * h - w * w) {
            hi--;
        }
        long long cap = (hi < m ? hi : m);  // 横向位移还要受 M 限制
        if (lo > cap) {
            continue;
        }
        // 容斥: 用 w 的质因子去掉与 w 不互素的 dx, 保证 gcd(dx, w) = 1
        long long coprime = 0;
        for (int mask = 0; mask < (1 << primeCnt[w]); mask++) {
            long long d = 1;
            int bits = 0;
            for (int j = 0; j < primeCnt[w]; j++) {
                if ((mask >> j) & 1) {
                    d *= primeFactor[w][j];
                    bits++;
                }
            }
            if (bits % 2 == 0) {
                coprime += multiplesSum(lo, cap, d);
            } else {
                coprime -= multiplesSum(lo, cap, d);
            }
        }
        coprime %= b;
        if (coprime < 0) {
            coprime += b;
        }
        // 竖直位移为 w 时, 竖直方向有 (n - w + 1) 个起点
        ans = (ans + coprime * ((n - w + 1) % b)) % b;
    }
    ans = 2 * ans % b;

    // 水平、垂直的横幅: dx 与 dy 有一个为 0, 本原就要求另一个恰好是 1, 即长度恰为 1 的相邻柱子
    if (l <= 1 && 1 <= h) {
        ans = (ans + (n % b) * ((m + 1) % b) + (m % b) * ((n + 1) % b)) % b;
    }
    printf("%lld\n", ans);
    return 0;
}
