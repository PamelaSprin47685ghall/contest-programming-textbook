#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

int main() {
    int n, l;
    scanf("%d%d", &n, &l);
    vector<int> c(n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &c[i]);
    }

    sort(c.rbegin(), c.rend());

    // 原始状态下的 h 指数
    int h = 0;
    while (h < n && c[h] >= h + 1) {
        ++h;
    }

    // 尝试使 h 指数提高到 h + 1:
    // 考察前 h + 1 篇论文 (下标 0 到 h):
    // 它们的目标引用数都需要至少达到 h + 1.
    // 因为每篇论文至多只能引用 1 次, 所以原本引用数就小于 h 的论文不可能达到 h + 1.
    // 只有原本引用数为 h 的论文可以通过消耗 1 次引用机会达到 h + 1.
    // 只要下标为 h 的这篇论文的引用数 c[h] >= h, 且需要补充的引用总量 <= L, 就可以将 h 提高到 h + 1!
    if (h < n) {
        int needed = 0;
        for (int i = 0; i <= h; ++i) {
            if (c[i] < h + 1) {
                needed += (h + 1 - c[i]);
            }
        }
        if (needed <= l && c[h] >= h) {
            ++h;
        }
    }

    printf("%d\n", h);
    return 0;
}
