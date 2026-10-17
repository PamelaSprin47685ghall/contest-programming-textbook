#include <cstdio>
#include <algorithm>
#include <set>

using namespace std;

const int MAXN = 100005;

struct Hill {
    int x1, y1, x2, y2;   // 山丘的两端, 保证 x1 < x2, y1 < y2
    int id;               // 输入顺序, 用来在集合里认出同一条山丘
};

int n;
Hill hills[MAXN];

// 因为任意两条山丘既不相交也不相碰, 它们的上下关系与 x 无关, 所以比较器只要固定在一个
// 公共位置即可: 取两者 x 区间的公共部分右端 min(x2), 在那里比高度。
// 两条山丘在该处的高度一定不等 (否则就相碰了), 因此比较器构成严格弱序。
struct HillLow {
    bool operator()(const Hill &a, const Hill &b) const {
        if (a.x2 < b.x2) {
            // 在 x = a.x2 处, a 的高度就是 a.y2, b 的高度用比例式交叉相乘表示
            return (long long)(a.y2 - b.y1) * (b.x2 - b.x1) <
                   (long long)(b.y2 - b.y1) * (a.x2 - b.x1);
        }
        return (long long)(a.y2 - a.y1) * (b.x2 - a.x1) <
               (long long)(b.y2 - a.y1) * (a.x2 - a.x1);
    }
};

struct Event {
    int x, y;       // 山丘端点的坐标
    int id;         // 对应的山丘
    bool isStart;   // true 表示左端点 (x1,y1), false 表示右端点 (x2,y2)
};

Event events[2 * MAXN];

bool earlier(const Event &a, const Event &b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d %d", &hills[i].x1, &hills[i].y1, &hills[i].x2, &hills[i].y2);
        hills[i].id = i;
        events[2 * i].x = hills[i].x1;
        events[2 * i].y = hills[i].y1;
        events[2 * i].id = i;
        events[2 * i].isStart = true;
        events[2 * i + 1].x = hills[i].x2;
        events[2 * i + 1].y = hills[i].y2;
        events[2 * i + 1].id = i;
        events[2 * i + 1].isStart = false;
    }
    // 同一 x 上按高度排序, 于是"离开"的山丘一定先于"下方更高"的山丘被处理:
    // 高度更低的旧山丘已经删掉, 高度更高的新山丘还不影响她往下看的结果
    sort(events, events + 2 * n, earlier);

    // active 里始终放着所有"跨过当前 x"的山丘, 并按高度排好序
    set<Hill, HillLow> active;
    active.insert(hills[0]);       // 第一条山丘的左端点就是 (0,0), Bessie 从这里出发
    int current = 0;               // Bessie 正在爬的山丘
    int touched = 1;

    for (int e = 1; e < 2 * n; e++) {
        const Event &ev = events[e];
        if (ev.isStart) {
            active.insert(hills[ev.id]);
        } else if (ev.id == current) {
            // 当前山丘走到头了, 直接往下掉: 落在紧挨着她下方的山丘上
            set<Hill, HillLow>::iterator it = active.find(hills[current]);
            if (it == active.begin()) break;   // 下方已无山丘, 落到 y = -infinity 的枕头堆上
            it--;
            current = it->id;
            active.erase(hills[ev.id]);
            touched++;
        } else {
            active.erase(hills[ev.id]);
        }
    }

    printf("%d\n", touched);
    return 0;
}
