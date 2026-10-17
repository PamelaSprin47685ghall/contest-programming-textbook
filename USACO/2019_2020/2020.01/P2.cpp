#include <cstdio>
#include <vector>

int main() {
    int n;
    scanf("%d", &n);
    std::vector<int> b(n - 1);
    for (int i = 0; i < n - 1; ++i) {
        scanf("%d", &b[i]);
    }

    // 枚举 a[0] 的取值 1..n
    for (int first = 1; first <= n; ++first) {
        std::vector<int> a(n);
        std::vector<bool> used(n + 1, false);

        a[0] = first;
        used[first] = true;
        bool valid = true;

        for (int i = 0; i < n - 1; ++i) {
            a[i + 1] = b[i] - a[i];
            if (a[i + 1] < 1 || a[i + 1] > n || used[a[i + 1]]) {
                valid = false;
                break;
            }
            used[a[i + 1]] = true;
        }

        if (valid) {
            for (int i = 0; i < n; ++i) {
                printf("%d%c", a[i], i == n - 1 ? '\n' : ' ');
            }
            return 0;
        }
    }

    return 0;
}
