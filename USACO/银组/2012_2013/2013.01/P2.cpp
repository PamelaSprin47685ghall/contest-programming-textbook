#include <algorithm>
#include <cstdio>
#include <set>

using namespace std;

const int MAXN = 50000;

struct Square {
    int x, y;
    // 集合里按 y 排序: 扫描线只会在集合里找 y 方向上的邻居
    bool operator<(const Square& other) const {
        if (y != other.y) return y < other.y;
        return x < other.x;
    }
};

bool ByX(const Square& a, const Square& b) {
    return a.x < b.x;
}

Square squares[MAXN];
// 与当前正方形 x 距离小于 K 的正方形, 按 y 排好序
set<Square> active;

int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    for (int i = 0; i < n; ++i) {
        scanf("%d%d", &squares[i].x, &squares[i].y);
    }
    sort(squares, squares + n, ByX);

    long long area = 0;  // 唯一一对重叠正方形的公共面积
    int pairs = 0;       // 已经找到的重叠对数
    int left = 0;        // active 中最靠左的正方形在数组中的下标
    for (int i = 0; i < n; ++i) {
        // x 距离达到 K 的两块正方形最多只碰到边, 公共面积为 0; 后面的正方形只会更靠右,
        // 所以这些正方形可以永久移出集合
        while (left < i && squares[i].x - squares[left].x >= k) {
            active.erase(squares[left]);
            ++left;
        }
        // 集合里剩下的正方形与当前正方形的 x 距离都小于 K, 于是只要 y 距离也小于 K 就必重叠
        Square probe;
        probe.x = -2000000;  // 比题目允许的横坐标都小, 让 lower_bound 停在第一个 y 不小于下界的正方形上
        probe.y = squares[i].y - k + 1;
        set<Square>::iterator it = active.lower_bound(probe);
        while (it != active.end() && it->y - squares[i].y < k) {
            if (++pairs > 1) {
                printf("-1\n");
                return 0;
            }
            int dx = squares[i].x - it->x;
            int dy = squares[i].y - it->y;
            if (dy < 0) dy = -dy;
            area = (long long)(k - dx) * (k - dy);
            ++it;
        }
        active.insert(squares[i]);
    }

    printf("%lld\n", area);
    return 0;
}
