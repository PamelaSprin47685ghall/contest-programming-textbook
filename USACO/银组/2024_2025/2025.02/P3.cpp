#include <cstdio>

using namespace std;

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        long long a, b, c, d;
        scanf("%lld %lld %lld %lld", &a, &b, &c, &d);

        if (a == c && b == d) {
            printf("0\n");
            continue;
        }

        // 从终点倒着推：每步只能减掉较小的那一堆，这一步是没有选择的，
        // 所以倒推路径唯一，能走到 (a, b) 就有解，答案就是步数。
        long long ans = 0;
        bool ok = true;
        while (a != c || b != d) {
            if (c < a || d < b) { // 正向只能变大，倒退过程中出现比起点还小即无解
                ok = false;
                break;
            }
            if (c > d) {
                // 可以连着减很多次 d，一直到 c 逼近 a 为止，一次批量算完
                long long k = (c - a) / d;
                if (k < 1) k = 1; // 至少减一次，否则原地打转
                ans += k;
                c -= k * d;
            } else if (d > c) {
                long long k = (d - b) / c;
                if (k < 1) k = 1;
                ans += k;
                d -= k * c;
            } else {
                ok = false; // 两堆相等又不是终点：正向操作永远造不出相等的两堆
                break;
            }
        }

        if (ok) {
            printf("%lld\n", ans);
        } else {
            printf("-1\n");
        }
    }
    return 0;
}
