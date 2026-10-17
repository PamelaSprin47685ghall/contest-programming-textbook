#include <cstdio>

using namespace std;

const int MAXN = 105;

int n;
long long tCookie, tMuffin;
long long a[MAXN], b[MAXN], c[MAXN];

// 检查花 w 个 mooney 能否满足所有朋友。
// 此时烤饼干耗时 x、烤松饼耗时 y，且 x + y = tCookie + tMuffin - w；
// 把 y = (tCookie + tMuffin - w) - x 代进约束 a_i * x + b_i * y <= c_i，
// 每条约束就变成 x 的一个上下界，只要这些界留出的区间非空即可行。
bool feasible(long long w) {
    long long sum = tCookie + tMuffin - w;         // 即 x + y
    long long lo = 1, hi = tCookie;                // 1 <= x <= tCookie
    if (sum - tMuffin > lo) lo = sum - tMuffin;    // 由 1 <= y <= tMuffin 推出
    if (sum - 1 < hi) hi = sum - 1;
    for (int i = 0; i < n; ++i) {
        if (a[i] > b[i]) {
            // (a_i - b_i) * x <= c_i - b_i * sum
            long long limit = c[i] - b[i] * sum;
            if (limit < 0) return false;           // 下取整后连 x >= 1 都取不到
            limit /= a[i] - b[i];                  // 非负分子，除法就是下取整
            if (limit < hi) hi = limit;
        } else if (b[i] > a[i]) {
            // (b_i - a_i) * x >= b_i * sum - c_i，即 x >= ceil(...)
            long long need = b[i] * sum - c[i];
            if (need > 0) {
                long long diff = b[i] - a[i];
                long long limit = (need + diff - 1) / diff;
                if (limit > lo) lo = limit;
            }
        } else {
            // a_i == b_i 时约束与 x 无关，直接判断
            if (a[i] * sum > c[i]) return false;
        }
        if (lo > hi) return false;
    }
    return true;
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        scanf("%d %lld %lld", &n, &tCookie, &tMuffin);
        for (int i = 0; i < n; ++i) {
            scanf("%lld %lld %lld", &a[i], &b[i], &c[i]);
        }
        // w 越大越容易满足（见题解），所以在 [0, tCookie + tMuffin - 2] 上二分
        long long lo = 0, hi = tCookie + tMuffin - 2;
        while (lo < hi) {
            long long mid = (lo + hi) / 2;
            if (feasible(mid)) hi = mid;
            else lo = mid + 1;
        }
        printf("%lld\n", lo);
    }
    return 0;
}
