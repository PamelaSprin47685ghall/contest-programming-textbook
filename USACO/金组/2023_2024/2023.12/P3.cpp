#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 200005;

int barn[MAXN];
// 把所有货批送到 barn[i] 时，左侧（含重复位置）谷仓产生的浪费距离之和
long long leftCost[MAXN];
// 把所有货批送到 barn[i] 时，右侧谷仓产生的浪费距离之和
long long rightCost[MAXN];

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &barn[i]);
    }
    sort(barn, barn + n);

    // 从 barn[i-1] 挪到 barn[i] 时，左侧 i 个谷仓每批都多走 barn[i]-barn[i-1]
    for (int i = 1; i < n; ++i) {
        leftCost[i] = leftCost[i - 1] + (long long)(barn[i] - barn[i - 1]) * i;
    }
    for (int i = n - 2; i >= 0; --i) {
        rightCost[i] = rightCost[i + 1] + (long long)(barn[i + 1] - barn[i]) * (n - 1 - i);
    }

    int q;
    scanf("%d", &q);
    while (q--) {
        long long a, b;
        scanf("%lld%lld", &a, &b);
        // 在 [barn[k-1], barn[k]] 上总浪费关于 y 是线性的，斜率由
        // a*k - b*(n-k) 决定；斜率变号的位置就是最优点：
        // k = floor(n*b/(a+b))。
        int best = (int)((long long)n * b / (a + b));
        printf("%lld\n", a * leftCost[best] + b * rightCost[best]);
    }
    return 0;
}
