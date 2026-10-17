#include <algorithm>
#include <cstdio>
#include <vector>

int main() {
    int n, q;
    scanf("%d%d", &n, &q);
    std::vector<int> c(n);
    std::vector<int> t(n);
    for (int i = 0; i < n; ++i) scanf("%d", &c[i]);
    for (int i = 0; i < n; ++i) scanf("%d", &t[i]);

    // 第 i 农场关门时间为 c[i], 走过去需要 t[i]
    // 假设在时间 S 出发, 到达时刻为 S + t[i]
    // 能在关门前到达要求: S + t[i] < c[i] => S < c[i] - t[i]
    // 设 diff[i] = c[i] - t[i], 则条件等价于 diff[i] > S
    std::vector<int> diff(n);
    for (int i = 0; i < n; ++i) {
        diff[i] = c[i] - t[i];
    }

    std::sort(diff.begin(), diff.end());

    for (int query = 0; query < q; ++query) {
        int v, s;
        scanf("%d%d", &v, &s);

        // 统计 diff 中严格大于 s 的元素数量
        // 使用 upper_bound 查找第一个 > s 的位置
        auto it = std::upper_bound(diff.begin(), diff.end(), s);
        int count = diff.end() - it;

        if (count >= v) {
            printf("YES\n");
        } else {
            printf("NO\n");
        }
    }

    return 0;
}
