#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

int main() {
    int n;
    long long m;
    scanf("%d%lld", &n, &m);

    char buf[200005];
    scanf("%s", buf);
    std::string s = buf;

    std::vector<long long> a(n);
    long long totalMilk = 0;
    for (int i = 0; i < n; ++i) {
        scanf("%lld", &a[i]);
        totalMilk += a[i];
    }

    std::vector<bool> badL(n, false), badR(n, false);
    for (int i = 0; i < n; ++i) {
        if (s[i] == 'R' && s[(i + 1) % n] == 'L') {
            badL[i] = true;
            badR[(i + 1) % n] = true;
        }
    }

    long long loss = 0;
    for (int i = 0; i < n; ++i) {
        long long sum = 0;
        if (badL[i]) {
            int j = (i - 1 + n) % n;
            while (s[j] == 'R') {
                sum += a[j];
                j = (j - 1 + n) % n;
            }
            loss += std::min(sum, m);
        }
        if (badR[i]) {
            int j = (i + 1) % n;
            while (s[j] == 'L') {
                sum += a[j];
                j = (j + 1) % n;
            }
            loss += std::min(sum, m);
        }
    }

    printf("%lld\n", totalMilk - loss);
    return 0;
}
