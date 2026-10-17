#include <cmath>
#include <cstdio>

using namespace std;

const int MAXN = 100000;

int n;
long long extraTarget; // 在每层楼 1 头基础奶牛之外，还需要额外安排的奶牛数 K - N
long long need[MAXN];  // 每层楼需要的总工作量

// 阈值 x 下，工作量 work 的楼层能额外安排多少头奶牛：
// 第 c 头额外奶牛把时间从 work/c 降到 work/(c+1)，边际收益是 work/(c(c+1))，
// 统计边际收益不低于 x 的奶牛数，即满足 c(c+1) <= work/x 的最大 c（二次方程求根）。
long long cowsAbove(long double x, long long work) {
    long double c = (sqrtl(1.0L + 4.0L * work / x) - 1.0L) / 2.0L;
    if (c < 0.0L) c = 0.0L;
    if (c > 4000000000000.0L) c = 4000000000000.0L; // 安全上限，防止极端输入下强转溢出
    return (long long)c;
}

// 统计所有楼层在阈值 x 下的额外奶牛总数；总数超过 K 的上限即可提前结束
long long totalCows(long double x) {
    long long total = 0;
    for (int i = 0; i < n; ++i) {
        total += cowsAbove(x, need[i]);
        if (total > 1000000000000LL) return total;
    }
    return total;
}

int main() {
    long long k;
    scanf("%d %lld", &n, &k);
    extraTarget = k - n;
    for (int i = 0; i < n; ++i) scanf("%lld", &need[i]);

    // 边际收益是递减的，贪心取收益最大的奶牛即可得最优。
    // 二分"最后一头被采用的奶牛"的边际收益 x：x 越大被取用的奶牛越少，单调。
    long double lo = 0.0L, hi = 1e18L;
    for (int iter = 0; iter < 100; ++iter) {
        long double mid = (lo + hi) / 2.0L;
        if (totalCows(mid) >= extraTarget) lo = mid;
        else hi = mid;
    }

    long double answer = 0.0L;
    long long total = 0;
    for (int i = 0; i < n; ++i) {
        long long extra = cowsAbove(lo, need[i]);
        answer += (long double)need[i] / (long double)(extra + 1); // 每层至少 1 头奶牛
        total += extra;
    }
    // 这样共排出 n + total 头奶牛，比允许的 K 头多出 total - extraTarget 头。
    // 奶牛越多用时越短，把多出的奶牛各按边际收益 lo 折算后加回去。
    answer += (long double)(total - extraTarget) * lo;

    printf("%lld\n", (long long)(answer + 0.5L));
    return 0;
}
