#include <algorithm>
#include <cstdio>
#include <vector>

void Solve() {
    int n;
    scanf("%d", &n);
    std::vector<long long> h(n);
    for (int i = 0; i < n; ++i) {
        scanf("%lld", &h[i]);
    }

    if (n == 1) {
        printf("0\n");
        return;
    }

    long long totalCorn = 0;

    // 第一遍: 从左向右贪心削平
    // 如果 h[i+1] > h[i], 我们必须对 (i+1, i+2) 喂食使其高度降低到与 h[i] 相等
    for (int i = 0; i < n - 2; ++i) {
        if (h[i + 1] > h[i]) {
            long long d = h[i + 1] - h[i];
            h[i + 1] -= d;
            h[i + 2] -= d;
            totalCorn += 2 * d;
        }
    }

    // 第二遍: 从右向左对称贪心削平
    for (int i = n - 1; i >= 2; --i) {
        if (h[i - 1] > h[i]) {
            long long d = h[i - 1] - h[i];
            h[i - 1] -= d;
            h[i - 2] -= d;
            totalCorn += 2 * d;
        }
    }

    // 检查是否所有数非负且全部相等
    bool allEqual = true;
    for (int i = 0; i < n; ++i) {
        if (h[i] < 0 || h[i] != h[0]) {
            allEqual = false;
            break;
        }
    }

    if (allEqual) {
        printf("%lld\n", totalCorn);
    } else {
        printf("-1\n");
    }
}

int main() {
    int t;
    if (scanf("%d", &t) == 1) {
        while (t--) {
            Solve();
        }
    }
    return 0;
}
