#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100000;

int n;
long long neededFlavor;      // 一顿饭至少要有的总风味
long long flavor[MAXN];      // 每捆干草的 flavor
long long spiciness[MAXN];   // 每捆干草的 spiciness
long long sortedSpice[MAXN]; // 把 spiciness 去重后从小到大排好
int distinctCount;

// 只允许 spiciness <= limit 的干草捆时, 能否凑出总风味 >= neededFlavor 的一段?
// 一顿饭的辣度不超过 limit, 等价于它整段都落在"可用"干草捆组成的连续段里;
// 风味都是正数, 所以一段连续段里最优的取法就是全取, 只需看每段的总风味.
bool CanMakeMeal(long long limit) {
    long long sum = 0;
    for (int i = 0; i < n; i++) {
        if (spiciness[i] <= limit) {
            sum += flavor[i];
            if (sum >= neededFlavor) return true;
        } else {
            sum = 0;
        }
    }
    return false;
}

int main() {
    scanf("%d%lld", &n, &neededFlavor);
    for (int i = 0; i < n; i++) {
        scanf("%lld%lld", &flavor[i], &spiciness[i]);
        sortedSpice[i] = spiciness[i];
    }

    // 一顿饭的辣度一定是某捆干草的 spiciness, 所以答案只需在这些值里找
    sort(sortedSpice, sortedSpice + n);
    distinctCount = 0;
    for (int i = 0; i < n; i++) {
        if (i == 0 || sortedSpice[i] != sortedSpice[i - 1]) {
            sortedSpice[distinctCount++] = sortedSpice[i];
        }
    }

    // 上限放宽只会让可选的干草捆变多, 所以"可行"随 limit 单调, 可以二分最小可行辣度
    int low = 0, high = distinctCount - 1;
    while (low < high) {
        int mid = (low + high) / 2;
        if (CanMakeMeal(sortedSpice[mid])) high = mid;
        else low = mid + 1;
    }
    printf("%lld\n", sortedSpice[low]);
    return 0;
}
