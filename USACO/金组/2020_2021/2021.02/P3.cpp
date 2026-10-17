#include <cstdio>

using namespace std;

const int DIGITS = 39;   // 3^39 > 2*10^18，足以容纳 x+k 的取值范围

long long pow3[DIGITS + 1];

int main() {
    pow3[0] = 1;
    for (int i = 1; i <= DIGITS; ++i) pow3[i] = pow3[i - 1] * 3;

    int q;
    scanf("%d", &q);
    while (q--) {
        long long d, x, y;
        scanf("%lld%lld%lld", &d, &x, &y);
        // 统计满足 0 <= k <= d 且 (x+k, y+k) 有奶牛（即两条对角线上对应格）的 k 的个数。
        // 逐位确定 k 的三进制数字，状态为：
        //   cmp：低若干位与 d 比较的关系（0 表示已确定小于，1 表示仍相等，2 表示已确定大于），
        //        由于是从低位到高位处理，越靠后的差异位越重要，会被直接覆盖；
        //   xc, yc：x+k、y+k 在当前位产生的进位（0 或 1）。
        // 每一位都要求 x+k 与 y+k 在该位的三进制数字奇偶性相同
        long long dp[3][2][2] = {};   // dp[cmp][xc][yc]
        dp[1][0][0] = 1;
        for (int i = 0; i < DIGITS; ++i) {
            long long ndp[3][2][2] = {};
            int dd = (int)(d / pow3[i] % 3);
            int xd = (int)(x / pow3[i] % 3);
            int yd = (int)(y / pow3[i] % 3);
            for (int cmp = 0; cmp < 3; ++cmp) {
                for (int xc = 0; xc < 2; ++xc) {
                    for (int yc = 0; yc < 2; ++yc) {
                        long long ways = dp[cmp][xc][yc];
                        if (ways == 0) continue;
                        for (int j = 0; j < 3; ++j) {   // 第 i 位的取值
                            int sx = xd + xc + j;       // 相加后不超过 5，进位最多为 1
                            int sy = yd + yc + j;
                            // 奶牛条件：两个数字在该位的奇偶性必须一致
                            if ((sx % 3) % 2 != (sy % 3) % 2) continue;
                            int ncmp = cmp;
                            if (j > dd) ncmp = 2;
                            else if (j < dd) ncmp = 0;
                            ndp[ncmp][sx / 3][sy / 3] += ways;
                        }
                    }
                }
            }
            for (int cmp = 0; cmp < 3; ++cmp)
                for (int xc = 0; xc < 2; ++xc)
                    for (int yc = 0; yc < 2; ++yc)
                        dp[cmp][xc][yc] = ndp[cmp][xc][yc];
        }
        // cmp 为 0/1 表示 k < d 或 k == d，即 k <= d；进位必须为 0，即高位没有溢出
        printf("%lld\n", dp[0][0][0] + dp[1][0][0]);
    }
    return 0;
}
