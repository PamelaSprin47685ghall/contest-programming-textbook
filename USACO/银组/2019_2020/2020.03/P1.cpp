// USACO 2020 US Open Contest, Silver P1: Social Distancing
#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXM = 100000;
const long long MAXX = 1000000000000000000LL;   // 坐标上界 10^18

struct Interval {
    long long left, right;
};

int n, m;
Interval intervals[MAXM];

bool cmpInterval(const Interval &a, const Interval &b) {
    return a.left < b.left;
}

// 判断距离下限 d 时能否放下至少 n 头牛。
// 贪心：每头牛都放在"不违反 d 的前提下最靠左"的位置，
// 这样给后面的牛留下的空间最大，所以放得下当且仅当这个贪心能放满 n 头。
bool canPlace(long long d) {
    int count = 0;
    bool hasLast = false;   // 是否已经放过牛
    long long last = 0;     // 上一头牛所在的位置

    for (int i = 0; i < m && count < n; ++i) {
        // 第一头牛直接放在区间最左端；之后每头牛放在 max(上一头 + d, 区间左端)
        long long pos = hasLast ? max(last + d, intervals[i].left) : intervals[i].left;
        while (pos <= intervals[i].right) {
            ++count;
            if (count >= n) break;
            last = pos;
            hasLast = true;
            pos = max(last + d, intervals[i].left);
        }
    }
    return count >= n;
}

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        scanf("%lld %lld", &intervals[i].left, &intervals[i].right);
    }
    // 区间互不相交，按左端点排序后贪心才能保证"从左到右依次放牛"的正确顺序
    sort(intervals, intervals + m, cmpInterval);

    // 可行性关于 d 单调：d 越小越容易放下 n 头牛，于是可以二分最大的可行 d
    long long lo = 1, hi = MAXX;
    while (lo < hi) {
        long long mid = lo + (hi - lo + 1) / 2;
        if (canPlace(mid)) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }

    printf("%lld\n", lo);
    return 0;
}
