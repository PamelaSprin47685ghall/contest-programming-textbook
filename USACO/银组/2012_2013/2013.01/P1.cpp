#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 100000;

// 每次移动的起点和终点都是关键坐标: 两个相邻关键坐标之间的整段油漆层数处处相同,
// 所以只记录关键坐标处的层数变化就够了 (坐标可达 1e9, 无法逐点统计)
struct Event {
    int x;      // 关键坐标
    int delta;  // 从这个坐标起, 油漆层数的变化量
    bool operator<(const Event& other) const {
        return x < other.x;
    }
};

Event events[2 * MAXN];

int main() {
    int n, k;
    scanf("%d%d", &n, &k);

    int pos = 0;  // 奶牛当前所在的位置
    for (int i = 0; i < n; ++i) {
        int dist;
        char dir;
        scanf("%d %c", &dist, &dir);
        int nxt = (dir == 'L') ? pos - dist : pos + dist;
        // 走过的这一段在起点处多刷一层, 在终点处少刷一层
        int lo = min(pos, nxt);
        int hi = max(pos, nxt);
        events[2 * i].x = lo;
        events[2 * i].delta = 1;
        events[2 * i + 1].x = hi;
        events[2 * i + 1].delta = -1;
        pos = nxt;
    }

    sort(events, events + 2 * n);

    // 从左往右累加变化量: 处理到第 i 个事件前, coats 就是前两个事件之间整段的层数
    long long answer = 0;  // 总长度可能接近 2e9, 用 long long 累计更稳妥
    int coats = 0;
    for (int i = 0; i < 2 * n; ++i) {
        if (i > 0 && coats >= k) {
            answer += events[i].x - events[i - 1].x;
        }
        coats += events[i].delta;
    }

    printf("%lld\n", answer);
    return 0;
}
