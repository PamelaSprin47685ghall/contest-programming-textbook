#include <cstdio>

using namespace std;

// 官方做法：把答案拆成 a、b 两段拼起来。
// 段 a 负责让 popcount 的异或值等于 K：K 的每个二进制位 i（0..4）贡献一个元素
// 2^(2^i) - 1，它的 popcount 恰好是 2^i，且是所有 popcount 为 2^i 的数中最小的。
// 因此 a 是所有"异或值为 K"的数组中元素和最小的一个：若 M < sum(a) 则无解。
// 段 b 用元素和 d = M - sum(a) 补上差值，并要求它自己的 popcount 异或为 0：
//   d 为偶数：两个 d/2 —— 相同 popcount 异或自己得 0；
//   d 为奇数且 >= 3：1, 2, (d-3)/2, (d-3)/2 —— 两个 1 抵消；
//   d = 0：两个 0；
//   d = 1：只能把 a 里的 1 换成 2（popcount 不变、总和加一），a 中没有 1 就无解，
//          因为 a 已是最小和，没有别的数组能把和从最小和提到最小和 + 1。
const int MAXK_BIT = 5; // K <= 31，用到的二进制位最多到 2^4

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        long long M;
        int K;
        scanf("%lld %d", &M, &K);

        long long a[MAXK_BIT];
        int aCnt = 0;
        long long sumA = 0;
        for (int i = 0; i < MAXK_BIT; ++i) {
            if (K & (1 << i)) {
                a[aCnt] = (1LL << (1 << i)) - 1; // popcount 恰为 2^i 的最小数
                sumA += a[aCnt];
                ++aCnt;
            }
        }

        long long d = M - sumA;
        long long b[4];
        int bCnt = 0;
        bool ok = true;
        if (d < 0) {
            ok = false;
        } else if (d == 0) {
            b[bCnt++] = 0;
            b[bCnt++] = 0;
        } else if (d == 1) {
            if (a[0] == 1) {
                a[0] = 2; // 1 与 2 的 popcount 都是 1，这样总和正好加一
                b[bCnt++] = 0;
                b[bCnt++] = 0;
            } else {
                ok = false;
            }
        } else if (d % 2 == 0) {
            b[bCnt++] = d / 2;
            b[bCnt++] = d / 2;
        } else {
            b[bCnt++] = 1;
            b[bCnt++] = 2;
            b[bCnt++] = (d - 3) / 2;
            b[bCnt++] = (d - 3) / 2;
        }

        if (!ok) {
            printf("-1\n");
            continue;
        }
        printf("%d\n", aCnt + bCnt);
        for (int i = 0; i < aCnt; ++i) {
            printf("%lld ", a[i]);
        }
        for (int i = 0; i < bCnt; ++i) {
            printf("%lld ", b[i]);
        }
        printf("\n");
    }
    return 0;
}
