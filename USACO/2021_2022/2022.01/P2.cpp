#include <algorithm>
#include <cstdio>

bool Beats(const int d1[4], const int d2[4]) {
    int wins = 0;
    int losses = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (d1[i] > d2[j]) ++wins;
            else if (d1[i] < d2[j]) ++losses;
        }
    }
    return wins > losses;
}

void Solve() {
    int a[4], b[4];
    for (int i = 0; i < 4; ++i) scanf("%d", &a[i]);
    for (int i = 0; i < 4; ++i) scanf("%d", &b[i]);

    for (int c1 = 1; c1 <= 10; ++c1) {
        for (int c2 = c1; c2 <= 10; ++c2) {
            for (int c3 = c2; c3 <= 10; ++c3) {
                for (int c4 = c3; c4 <= 10; ++c4) {
                    int c[4] = {c1, c2, c3, c4};
                    if (Beats(a, b) && Beats(b, c) && Beats(c, a)) {
                        printf("yes\n");
                        return;
                    }
                    if (Beats(b, a) && Beats(a, c) && Beats(c, b)) {
                        printf("yes\n");
                        return;
                    }
                }
            }
        }
    }
    printf("no\n");
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
