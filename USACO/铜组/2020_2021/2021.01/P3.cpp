#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

int main() {
    int n;
    scanf("%d", &n);
    vector<long long> a(n);
    vector<long long> b(n);
    for (int i = 0; i < n; ++i) scanf("%lld", &a[i]);
    for (int i = 0; i < n; ++i) scanf("%lld", &b[i]);

    // 从大到小为每头牛匹配可以容纳它的牛棚
    sort(a.rbegin(), a.rend());
    sort(b.rbegin(), b.rend());

    long long totalWays = 1;

    for (int i = 0; i < n; ++i) {
        // 统计有多少个牛棚的高度上限 >= a[i]
        int count = 0;
        for (int j = 0; j < n; ++j) {
            if (b[j] >= a[i]) {
                ++count;
            }
        }
        // 之前比当前牛更高的 i 头牛已经各占用了 1 个牛棚, 且那些牛棚的高度必然也都 >= a[i]
        int availableStalls = count - i;
        if (availableStalls <= 0) {
            totalWays = 0;
            break;
        }
        totalWays *= availableStalls;
    }

    printf("%lld\n", totalWays);
    return 0;
}
