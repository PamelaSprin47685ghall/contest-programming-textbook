#include <cstdio>

using namespace std;

// 记起始数为 start, 目标数为 goal. 三个关键结论(见题解):
//   1) 最优解里不会出现"先乘 2 再除 2": 若出现 *,+^k,/ (此时 k 必为偶数),
//      它的效果是 x -> x + k/2, 用 k/2 次加法就能替代, 步数更少.
//      于是操作序列必然分成三段: S1 只有除法(以及为凑偶数加的一个 1),
//      S2 只有加法, S3 只有乘法与加法.
//   2) 紧贴一次除法之前的加法至多一个((x+2)/2 = x/2+1), 乘法之后同理,
//      所以 S1 的除法次数一旦确定, S1 的走法就唯一确定;
//   3) S3 的乘法次数一旦确定, 进入 S3 前必须达到的数也唯一确定, 即 goal >> 乘的次数.

int main() {
    int N;
    scanf("%d", &N);
    for (int t = 0; t < N; ++t) {
        long long start, goal;
        scanf("%lld%lld", &start, &goal);
        long long best = -1;  // -1 表示还没有找到任何可行方案
        // mult 是结尾乘法的次数, 即 S3 中乘法的次数.
        for (int mult = 0; (goal >> mult) > 0; ++mult) {
            long long target = goal >> mult;  // 进入 S3 之前需要达到的数
            long long cur = start;
            long long cost = 0;
            // S1: 只要 cur 超过 target, 就先把它变成偶数再除以 2.
            // 加 1 是唯一能改变奇偶性的手段, 而由结论 2, 加更多没有意义.
            while (cur > target) {
                if (cur % 2 == 1) {
                    ++cur;
                    ++cost;
                }
                cur /= 2;
                ++cost;
            }
            cost += target - cur;  // S2: 剩下的差距全部用加法补上
            cost += mult;          // S3 中的 mult 次乘法
            // S3 中的加法次数: 从 target 出发乘 mult 次刚好凑出 goal 的高位,
            // 低 mult 位里的每个 1 都要用一次加法补出来.
            long long low = goal & ((1LL << mult) - 1);
            while (low > 0) {
                cost += low & 1;
                low >>= 1;
            }
            if (best == -1 || cost < best) best = cost;
        }
        printf("%lld\n", best);
    }
    return 0;
}
