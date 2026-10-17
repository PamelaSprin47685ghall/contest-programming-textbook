#include <cstdio>
#include <algorithm>
using namespace std;

// t 最大 1e18，而零位置序列每步至少乘 1.5，(3/2)^110 > 1e18，故 110 步足够覆盖
const int MAXJ = 115;
// 哨兵：比任何可能用到的 x 都大，使 j=1 时的下界约束自动成立
const long long BIG = 4000000000000000000LL;

long long inv1[MAXJ];  // inv1[j] = g^(j-1)(t-1)
long long inv2[MAXJ];  // inv2[j] = g^(j-1)(l2+t-1)
long long inv3[MAXJ];  // inv3[j] = g^(j-1)(r2+t)

// g(n) = floor((2n-2)/3)：若 t_{i,j} <= x 当且仅当 t_{i,j-1} <= g(x)，故 g 是序列的逆映射
// 写成 (2n+4)/3-2 与 floor((2n-2)/3) 在 n >= -2 时完全相同，且不会出现负数取整分歧
long long shrink(long long n) {
    return (2 * n + 4) / 3 - 2;
}

// 生成 G[j] = g^(j-1)(n)（j >= 1），G[0] 置哨兵
void build(long long n, long long *G) {
    G[0] = BIG;
    G[1] = n;
    for (int j = 2; j < MAXJ; j++) G[j] = shrink(G[j - 1]);
}

int main() {
    int Q;
    scanf("%d", &Q);
    while (Q--) {
        long long l1, r1, l2, r2, t;
        scanf("%lld %lld %lld %lld %lld", &l1, &r1, &l2, &r2, &t);

        long long ans = 0;

        // 情况一：i >= t/2+1 等价于 t <= 2i-1。此时奶牛 i 一直停在原位，f(i,t) = i
        long long low1 = max(max(l1, l2), t / 2 + 1);
        long long high1 = min(r1, r2);
        if (high1 >= low1) ans += high1 - low1 + 1;

        // 情况二：i <= t/2（即 t >= 2i）。此时 f(i,t) = t_{i,j} - t，其中 t_{i,j-1} < t <= t_{i,j}
        // t_{i,0} = 2i-1 是第一次不再原位的时间，t_{i,1} = 3i-1 是第一次回到位置 0 的时间
        build(t - 1, inv1);
        build(l2 + t - 1, inv2);
        build(r2 + t, inv3);
        for (int j = 1; j <= 110; j++) {
            // 区间条件 t_{i,j-1} < t <= t_{i,j} 与 l2+t <= t_{i,j} <= r2+t
            // 由 t_{i,j} <= x 当且仅当 3i-1 <= g^(j-1)(x)，三者都化为对 3i-1 的界：
            //   l2+t <= t_{i,j}    <==>  3i-1 >  inv2[j]
            //   t_{i,j} <= r2+t    <==>  3i-1 <= inv3[j]
            //   t_{i,j-1} <= t-1   <==>  3i-1 <= inv1[j-1]
            long long low = max(l1, (inv2[j] + 4) / 3);
            long long cap = min(inv1[j - 1], inv3[j]);
            long long high = min(min(r1, t / 2), (cap + 4) / 3 - 1);
            if (high >= low) ans += high - low + 1;
        }

        printf("%lld\n", ans);
    }
    return 0;
}
