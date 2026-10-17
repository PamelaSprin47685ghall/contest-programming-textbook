#include <algorithm>
#include <cstdio>

int main() {
    int n;
    scanf("%d", &n);

    // timeNeed[t]: 时刻 t 正在挤奶所需的桶数总和.
    int timeNeed[1001] = {};
    for (int i = 0; i < n; ++i) {
        int start, end, buckets;
        scanf("%d%d%d", &start, &end, &buckets);
        for (int t = start; t <= end; ++t) {
            timeNeed[t] += buckets;
        }
    }

    // 在所有时刻中, 找出瞬时所需的最大桶数.
    int maxBuckets = 0;
    for (int t = 1; t <= 1000; ++t) {
        if (timeNeed[t] > maxBuckets) {
            maxBuckets = timeNeed[t];
        }
    }

    printf("%d\n", maxBuckets);
    return 0;
}
