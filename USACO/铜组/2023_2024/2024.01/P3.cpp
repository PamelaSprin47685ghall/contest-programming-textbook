#include <cstdio>
#include <cstdlib>
#include <vector>

int main() {
    int n;
    scanf("%d", &n);
    std::vector<long long> a(n);
    for (int i = 0; i < n; ++i) {
        scanf("%lld", &a[i]);
    }

    // 从左到右依次消除每个位置 a[i]:
    // 如果在位置 i 施加一次操作 (增加或减少):
    // 之后每个位置 j (j >= i) 的增加量是 (j - i + 1) * delta
    // 维护当前累积对后续位置产生的一次项变化:
    // val(j) = cur_val + slope * (j - prev_j)
    long long totalOps = 0;
    long long currentSlope = 0;
    long long currentValue = 0;

    for (int i = 0; i < n; ++i) {
        currentValue += currentSlope;
        long long actualVal = a[i] + currentValue;
        // 需要抵消 actualVal, 施加 delta = -actualVal 次操作
        long long delta = -actualVal;
        totalOps += std::abs(delta);

        currentSlope += delta;
        currentValue += delta;
    }

    printf("%lld\n", totalOps);
    return 0;
}
