#include <cstdio>

using namespace std;

const int MAXN = 200000;

int n;
int lastOcc[MAXN + 1];  // lastOcc[b]：品种 b 上一次出现的下标（0 表示还没出现过）
int bit[MAXN + 1];      // 树状数组：位置 l 上为 1 表示品种 b_l 在区间 [l, 当前 r] 内只出现一次

void add(int pos, int delta) {
    for (; pos <= n; pos += pos & -pos) bit[pos] += delta;
}

int sum(int pos) {
    int res = 0;
    for (; pos > 0; pos -= pos & -pos) res += bit[pos];
    return res;
}

int main() {
    scanf("%d", &n);
    long long ans = 0;
    for (int r = 1; r <= n; ++r) {
        int b;
        scanf("%d", &b);
        int prev = lastOcc[b];
        // 品种 b 新出现一次后，上一次出现的位置 l=prev 的品种在 [prev, r] 内不再唯一，失去当队长的资格
        if (prev > 0) add(prev, -1);
        // 枚举左队长 l：只有 l > prev 才能保证 b_l != b_r；再用树状数组数出其中品种唯一的位置个数。
        // 右队长 r 的品种在 [l, r] 内唯一，恰好等价于 l > prev 时 b_r 只出现一次（区间内 b_r 的另一次出现就是 prev）。
        ans += sum(r) - sum(prev);
        lastOcc[b] = r;
        // r 自身成为其品种当前唯一一次出现，标记进树状数组，供之后的右端点使用
        add(r, 1);
    }
    printf("%lld\n", ans);
    return 0;
}
