// P3 Supervision
// 只关心"每个被选中的露营者左边最近的被选教练"：教练位置随下标严格递增，
// 所以能管到这头露营者的教练恰好是在窗口 [p-D, p] 里的那一批（已处理过的教练的一段后缀）。
// 用滑动窗口 + 惰性乘 2 的标记维护 f_j = 最靠右的被选教练恰为 j 的方案数。
#include <cstdio>

using namespace std;

const int MAXN = 1000005;
const long long MOD = 1000000007LL;

int quePos[MAXN];      // 窗口内教练的位置
long long queG[MAXN];  // 窗口内教练"归一化"后的 f 值（真实的 f 值 = 它 × mul）
long long fsum;        // 已经滑出窗口（再也不会被乘 2）的教练的 f 值之和
long long gsum;        // 仍在窗口内的教练的归一化值之和
long long mul, invMul; // mul：窗口内共享的乘 2 惰性标记；invMul 是它在模意义下的逆

int main() {
    int n;
    long long D;
    scanf("%d %lld", &n, &D);
    const long long inv2 = (MOD + 1) / 2;
    mul = 1;
    invMul = 1;
    fsum = 0;
    gsum = 0;
    int head = 0, tail = 0;
    for (int i = 1; i <= n; i++) {
        long long p;
        int coach;
        scanf("%lld %d", &p, &coach);
        // 位置 < p - D 的教练，从这头奶牛开始（以及之后的每一头奶牛）都不可能再成为
        // "最近的被选教练"，可以彻底移出窗口：它的方案数从此不会再被乘 2
        while (head < tail && quePos[head] < p - D) {
            fsum = (fsum + queG[head] % MOD * mul) % MOD;
            gsum = (gsum - queG[head] + MOD) % MOD;
            head++;
        }
        if (coach == 0) {
            // 露营者可以选也可以不选：窗口内所有状态的方案数都翻倍，用惰性标记统一处理
            mul = mul * 2 % MOD;
            invMul = invMul * inv2 % MOD;
        } else {
            // 这头教练可以选（最靠右的被选教练变成它，前缀方案数随意 = total）
            // 也可以不选（状态不变）。1 是"此前一个教练都没选"的空集。
            long long total = (1 + fsum + gsum * mul) % MOD;
            // 存归一化值，插入时刻的 mul 要在稍后取出时被抵消掉
            long long g = total * invMul % MOD;
            quePos[tail] = p;
            queG[tail] = g;
            tail++;
            gsum = (gsum + g) % MOD;
        }
    }
    // 统计时不要忘记"没有选任何教练"的那 1 种（空集），但它不是非空子集，最后要减掉
    long long ans = (fsum + gsum * mul) % MOD;
    printf("%lld\n", ans);
    return 0;
}
