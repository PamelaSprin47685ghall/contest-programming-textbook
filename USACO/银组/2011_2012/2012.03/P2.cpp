#include <cstdio>
#include <algorithm>
#include <set>

using namespace std;

const int MAXN = 100000;
const int INF = 2000000000;

struct Drop {
    int x, y;
};

// 只按水平位置排序: 花盆是一段连续的 x 区间, 排序后才能用两条竖直边界扫过去.
bool ByX(const Drop& a, const Drop& b) {
    return a.x < b.x;
}

Drop drop[MAXN];
multiset<int> heightInPot;   // 当前花盆(两条扫描线之间)接到的所有雨滴高度

int main() {
    int n = 0, d = 0;
    scanf("%d %d", &n, &d);
    for (int i = 0; i < n; ++i)
        scanf("%d %d", &drop[i].x, &drop[i].y);
    sort(drop, drop + n, ByX);

    // 两条竖直扫描线夹出一段 x 区间, 区间内的雨滴高度放进 heightInPot.
    // 雨滴下落速度都是 1, 所以接到雨的时间差 = 区间内最高雨滴与最低雨滴的高度差.
    heightInPot.insert(drop[0].y);
    int left = 0, right = 0;
    int best = INF;
    while (true) {
        int fallenSpan = *heightInPot.rbegin() - *heightInPot.begin();
        if (fallenSpan >= d) {
            // 区间已满足要求, 记下宽度, 再把左边界右移: 对同一个 right, 满足条件的
            // 最窄区间就是左边界尽量靠右的那个, 所以不用再试更小的 right.
            int width = drop[right].x - drop[left].x;
            if (width < best) best = width;
            heightInPot.erase(heightInPot.find(drop[left].y));
            ++left;
        } else {
            // 区间还不满足要求, 只能让右边界右移; 右边界到头就说明无解.
            if (right == n - 1) break;
            ++right;
            heightInPot.insert(drop[right].y);
        }
    }

    printf("%d\n", best == INF ? -1 : best);
    return 0;
}
