#include <algorithm>
#include <cstdio>
#include <vector>

struct Delivery {
    long long day;
    long long count;
};

int main() {
    int n;
    long long t;
    scanf("%d%lld", &n, &t);

    std::vector<Delivery> deliveries(n);
    for (int i = 0; i < n; ++i) {
        scanf("%lld%lld", &deliveries[i].day, &deliveries[i].count);
    }

    long long currentHay = 0;
    long long lastDay = 1;
    long long totalEaten = 0;

    for (int i = 0; i < n; ++i) {
        long long d = deliveries[i].day;
        long long b = deliveries[i].count;

        // 从 lastDay 到 d - 1 之间吃干草
        long long daysPassed = d - lastDay;
        long long eaten = std::min(currentHay, daysPassed);
        totalEaten += eaten;
        currentHay -= eaten;

        currentHay += b;
        lastDay = d;
    }

    // 处理从最后一次送草日 deliveries[n-1].day 到截止日 T 的吃草情况
    long long daysRemaining = t - lastDay + 1;
    long long eaten = std::min(currentHay, daysRemaining);
    totalEaten += eaten;

    printf("%lld\n", totalEaten);
    return 0;
}
