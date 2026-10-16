#include <algorithm>
#include <cstdio>
#include <vector>

int main() {
    int n, k;
    scanf("%d%d", &n, &k);

    // 差分数组: 区间 [a, b] 加 1 记作 diff[a]++, diff[b+1]--.
    std::vector<int> diff(n + 2, 0);
    for (int i = 0; i < k; ++i) {
        int a, b;
        scanf("%d%d", &a, &b);
        diff[a] += 1;
        diff[b + 1] -= 1;
    }
    for (int i = 1; i <= n; ++i) diff[i] += diff[i - 1];

    // 前缀和之后 diff[1..n] 就是每个位置的草垛数, 排序取中位数.
    std::sort(diff.begin() + 1, diff.begin() + n + 1);
    printf("%d\n", diff[1 + n / 2]);
    return 0;
}