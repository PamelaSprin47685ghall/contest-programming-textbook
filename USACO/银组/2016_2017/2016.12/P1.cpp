#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 100005;

int haybale[MAXN];  // 所有干草堆的位置, 排好序后使用

int main() {
    int n, q;
    scanf("%d%d", &n, &q);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &haybale[i]);
    }

    // 排序之后, 落在区间 [A, B] 里的位置在数组中恰好是一段连续下标,
    // 于是一条询问只要两次二分就能数出个数, 不必逐个干草堆检查.
    sort(haybale, haybale + n);

    for (int i = 0; i < q; ++i) {
        int a, b;
        scanf("%d%d", &a, &b);
        // 从第一个 >= A 的下标开始, 到第一个 > B 的下标之前, 都是区间内的干草堆.
        int firstNotLess = lower_bound(haybale, haybale + n, a) - haybale;
        int firstGreater = upper_bound(haybale, haybale + n, b) - haybale;
        printf("%d\n", firstGreater - firstNotLess);
    }
    return 0;
}
