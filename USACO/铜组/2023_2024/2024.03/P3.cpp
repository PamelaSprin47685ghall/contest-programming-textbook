#include <algorithm>
#include <cstdio>
#include <vector>

void Solve() {
    int n;
    scanf("%d", &n);
    std::vector<int> h(n - 1);
    for (int i = 0; i < n - 1; ++i) {
        scanf("%d", &h[i]);
    }

    std::vector<bool> has(n + 1, false);
    for (int v : h) has[v] = true;

    std::vector<int> notHas;
    for (int i = 1; i <= n; ++i) {
        if (!has[i]) notHas.push_back(i);
    }

    int countOne = 0;
    for (int v : h) if (v == 1) ++countOne;

    if (notHas.size() > 2 || h.back() != 1 ||
        (notHas.size() == 2 && countOne != 2)) {
        printf("-1\n");
        return;
    }

    std::vector<int> p(n);
    if (notHas.size() == 1) {
        p[0] = 1;
        p[n - 1] = notHas[0];
    } else {
        p[0] = notHas[0];
        p[n - 1] = notHas[1];
    }

    int l = 0, r = n - 1;
    for (int i = 0; i < n - 1; ++i) {
        if (p[l] > p[r]) {
            p[++l] = h[i];
        } else {
            p[--r] = h[i];
        }
    }

    for (int i = 0; i < n; ++i) {
        printf("%d%c", p[i], i + 1 == n ? '\n' : ' ');
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
