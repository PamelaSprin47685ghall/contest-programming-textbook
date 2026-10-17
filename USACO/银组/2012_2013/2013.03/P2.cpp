#include <algorithm>
#include <cstdio>
#include <set>

using namespace std;

const int MAXN = 50000;
const int INF = 1000000000;  // 大于所有坐标的哨兵, 保证射线一定碰到一条边

int x1[MAXN], y1[MAXN];  // 左下角
int x2[MAXN], y2[MAXN];  // 右上角
bool visible[MAXN];      // 该围栏是否确认不被任何围栏包含, 决定离开扫描线时要不要删掉它的边

// 扫描线事件: x 是事件发生的横坐标, id 是围栏编号, type 为 1 表示左边界(进入), 0 表示右边界(离开)
// 围栏的边互不相交, 所以同一 x 上先处理离开再处理进入, 不会误判包含关系
struct Event {
    int x;
    int id;
    int type;
    bool operator<(const Event& other) const {
        if (x != other.x) {
            return x < other.x;
        }
        return type < other.type;
    }
};

Event events[2 * MAXN];

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d%d%d%d", &x1[i], &y1[i], &x2[i], &y2[i]);
        events[2 * i].x = x1[i];
        events[2 * i].id = i;
        events[2 * i].type = 1;
        events[2 * i + 1].x = x2[i];
        events[2 * i + 1].id = i;
        events[2 * i + 1].type = 0;
    }
    sort(events, events + 2 * n);

    // 集合里存当前仍然"可见"的围栏的水平边, 按 y 排序: (y, 1) 是下边界, (y, 0) 是上边界.
    // 围栏边互不相交 => 这些围栏在 y 方向的区间也互不相交, 所以从一点向上作射线,
    // 遇到的第一条边是下边界 <=> 该点不在任何围栏内部; 是上边界 <=> 该点被某个围栏包含.
    // 又因为围栏边互不相交, "围栏被包含"等价于"它的左下角被包含", 所以只在左边界处检查一次.
    multiset<pair<int, int> > edges;
    edges.insert(make_pair(INF, 1));  // 哨兵: 上方没有边时视为碰到的是一条下边界

    int answer = 0;
    for (int i = 0; i < 2 * n; ++i) {
        int id = events[i].id;
        if (events[i].type == 1) {
            // -1 比同 y 的任何真实边都小, 这样 lower_bound 会取到 y 不小于 y1[id] 的第一条边
            pair<int, int> first = *edges.lower_bound(make_pair(y1[id], -1));
            if (first.second == 1) {
                ++answer;
                visible[id] = true;
                edges.insert(make_pair(y1[id], 1));
                edges.insert(make_pair(y2[id], 0));
            }
        } else if (visible[id]) {
            edges.erase(edges.find(make_pair(y1[id], 1)));
            edges.erase(edges.find(make_pair(y2[id], 0)));
        }
    }

    printf("%d\n", answer);
    return 0;
}
