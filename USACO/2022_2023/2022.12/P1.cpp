#include <algorithm>
#include <cstdio>
#include <vector>

int main() {
    int n;
    scanf("%d", &n);
    std::vector<long long> c(n);
    for (int i = 0; i < n; ++i) {
        scanf("%lld", &c[i]);
    }

    std::sort(c.begin(), c.end());

    long long maxRevenue = 0;
    long long bestTuition = 0;

    for (int i = 0; i < n; ++i) {
        long long tuition = c[i];
        long long count = n - i;  // 大于等于该学费的奶牛数量
        long long revenue = tuition * count;
        if (revenue > maxRevenue) {
            maxRevenue = revenue;
            bestTuition = tuition;
        }
    }

    printf("%lld %lld\n", maxRevenue, bestTuition);
    return 0;
}
