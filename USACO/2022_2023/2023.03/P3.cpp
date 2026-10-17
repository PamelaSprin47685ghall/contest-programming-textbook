#include <cstdio>
#include <vector>

int main() {
    int n, k, t;
    if (scanf("%d%d%d", &n, &k, &t) != 3) return 0;

    std::vector<int> a(k + 2);
    for (int i = 1; i <= k; ++i) {
        scanf("%d", &a[i]);
    }
    a[k + 1] = n;

    std::vector<int> ans(n);

    // 对于每个区间 [A[i], A[i+1]) 中的每个初始位置 j
    for (int i = 1; i <= k; ++i) {
        int diff = a[i + 1] - a[i];
        for (int j = a[i]; j < a[i + 1]; ++j) {
            // j 相对于 A[i] 的初始相对偏移
            int currentShift = j - a[i];
            // 在参考系中 j 逆向平移了 T 步
            int newShift = currentShift - t;
            newShift = (newShift % diff + diff) % diff;

            // A[i] 随着全局参考系整体顺时针偏移 T 步, 故最终位置为:
            int finalPos = (a[i] + newShift + t) % n;
            ans[finalPos] = j;
        }
    }

    for (int i = 0; i < n; ++i) {
        printf("%d%c", ans[i], i == n - 1 ? '\n' : ' ');
    }
    return 0;
}
