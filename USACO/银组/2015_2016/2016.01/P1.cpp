#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 50005;

int N, K;
int x[MAXN]; // 所有干草堆的位置，升序

// 判断：每头奶牛的威力都是 r 时，能否用不超过 K 头奶牛炸掉全部干草堆。
// 贪心策略：当前最靠左、还没被炸掉的干草堆位于 x[i]，它必须被某头奶牛覆盖。
// 奶牛落点最靠右可以放在 x[i]+r，此时它覆盖 [x[i], x[i]+2r]；因为 x[i] 左边
// 已经没有干草堆，落点越靠右只会多炸掉右边的干草堆，不会漏掉任何东西。
// 于是每头奶牛都取这个最靠右的位置即可。
bool feasible(int r) {
    int used = 0; // 已发射的奶牛数
    int i = 0;    // 第一个尚未被炸掉的干草堆下标
    while (i < N) {
        ++used;
        if (used > K) return false; // 奶牛不够，r 不可行
        long long reach = (long long)x[i] + 2LL * r; // 落点在 x[i]+r 处能覆盖到的右端
        while (i < N && x[i] <= reach) ++i;
    }
    return true;
}

int main() {
    scanf("%d%d", &N, &K);
    for (int i = 0; i < N; ++i) {
        scanf("%d", &x[i]);
    }
    sort(x, x + N);

    // 可行性关于 r 单调（r 越大越好炸），二分最小的可行 r
    int lo = 0, hi = 1000000000;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (feasible(mid)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    printf("%d\n", lo);
    return 0;
}
