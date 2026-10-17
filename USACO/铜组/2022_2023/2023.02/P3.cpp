#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

int main() {
    int n;
    long long k;
    scanf("%d%lld", &n, &k);
    vector<long long> d(n);
    for (int i = 0; i < n; ++i) {
        scanf("%lld", &d[i]);
    }

    // 第一天必须开启一段订阅: 开销为 (1 + K)
    long long totalCost = 1 + k;

    // 贪心考虑从第 i-1 天到第 i 天:
    // 方案 A: 延续原有订阅, 额外支付两期间天数差 (d[i] - d[i-1]) 个 moonies
    // 方案 B: 让原有订阅结束, 在第 i 天重新开启一次新订阅, 额外支付 (1 + K) 个 moonies
    for (int i = 1; i < n; ++i) {
        long long extendCost = d[i] - d[i - 1];
        long long newSubCost = 1 + k;
        totalCost += min(extendCost, newSubCost);
    }

    printf("%lld\n", totalCost);
    return 0;
}
