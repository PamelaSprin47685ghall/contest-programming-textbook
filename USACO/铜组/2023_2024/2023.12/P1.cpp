#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    vector<long long> h(n);
    for (int i = 0; i < n; ++i) {
        scanf("%lld", &h[i]);
    }

    for (int j = 0; j < m; ++j) {
        long long cane;
        scanf("%lld", &cane);

        long long eatenUpTo = 0;
        for (int i = 0; i < n && eatenUpTo < cane; ++i) {
            if (h[i] > eatenUpTo) {
                long long eatAmount = min(h[i], cane) - eatenUpTo;
                h[i] += eatAmount;
                eatenUpTo += eatAmount;
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        printf("%lld\n", h[i]);
    }
    return 0;
}
