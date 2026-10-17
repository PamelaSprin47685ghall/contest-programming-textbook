#include <cstdio>

using namespace std;

const int MAXN = 100005;
const int OFF = MAXN;      // 前缀和的偏移量: 前缀和范围是 [-n, n], 加偏移后落进 [0, 2n]
const int SZ = 2 * MAXN;   // 树状数组实际下标范围 [1, SZ]

int bit[SZ + 5];

// 求已插入元素中值 <= p 的个数 (p 已经是加过偏移的下标)
int query(int p) {
    int sum = 0;
    for (int i = p + 1; i > 0; i -= (i & -i)) {
        sum += bit[i];
    }
    return sum;
}

// 把值 p 插入树状数组 (p 已经是加过偏移的下标)
void update(int p) {
    for (int i = p + 1; i <= SZ; i += (i & -i)) {
        bit[i]++;
    }
}

int main() {
    int n, x;
    scanf("%d %d", &n, &x);

    // 把 >= X 的数看成 +1, < X 的数看成 -1, 这样区间中位数 >= X
    // 就等价于该区间和 >= 0 (区间内大数不少于小数).
    // 记前缀和 P[0] = 0, P[j] = A[1] + ... + A[j], 则区间 (i, j] 的和等于 P[j] - P[i],
    // 于是要统计的就是满足 0 <= i < j <= n 且 P[i] <= P[j] 的二元组数.
    long long ans = 0;
    int prefix = 0;
    update(OFF);   // 先插入 P[0] = 0, 之后每个 j 只会被更小的 i 统计到

    for (int j = 0; j < n; j++) {
        int h;
        scanf("%d", &h);
        prefix += (h >= x ? 1 : -1);
        // 之前插入的都是 i < j 的前缀和, 统计其中 <= prefix 的个数
        ans += query(prefix + OFF);
        update(prefix + OFF);
    }

    printf("%lld\n", ans);
    return 0;
}
