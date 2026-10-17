#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 100005;

int surplus[MAXN];  // 前 i 堆的累计净余量 A - B

int main() {
    int n;
    scanf("%d", &n);

    int cur = 0;
    for (int i = 0; i < n; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        cur += a - b;
        surplus[i] = cur;
    }

    // 设第 n 堆与第 1 堆之间移动 t 个干草, 那么第 i 条相邻边上的流量恰好是 surplus[i] + t;
    // 总工作量就是 sum|surplus[i] + t|, 它关于 t 是凸函数, 取中位数时最小.
    sort(surplus, surplus + n);
    int median = surplus[n / 2];

    long long ans = 0;
    for (int i = 0; i < n; i++) {
        long long diff = (long long)surplus[i] - median;
        if (diff < 0) {
            diff = -diff;
        }
        ans += diff;
    }
    printf("%lld\n", ans);
    return 0;
}
