#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>
#include <utility>
#include <functional>

using namespace std;

// 完整解法：把"树"和"限制"按坐标合并成一个扫描序列，
// 再用小根堆保存所有生效限制给出的"累计砍树上限"。
// 扫到一棵树时，只要最紧的那个上限还没被用满，就贪心砍掉它。
const int MAXN = 100005;

int treePosition[MAXN];

struct Event {
    int position; // 事件坐标：树的坐标，或限制区间的左端点
    int kind;     // 0 表示遇到一条限制，1 表示遇到一棵树
    int right;    // 限制区间的右端点（kind 为 0 时有效）
    int extra;    // 该限制内最多还能砍掉的棵数（kind 为 0 时有效）
};

Event events[2 * MAXN];

bool cmpEvent(const Event &a, const Event &b) {
    if (a.position != b.position) {
        return a.position < b.position;
    }
    return a.kind < b.kind; // 同一坐标先处理限制，因为区间包含左端点处的树
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        int N, K;
        scanf("%d %d", &N, &K);
        for (int i = 0; i < N; ++i) {
            scanf("%d", &treePosition[i]);
        }
        sort(treePosition, treePosition + N); // 有序才能二分统计区间内的树

        int eventCount = 0;
        for (int i = 0; i < N; ++i) {
            events[eventCount].position = treePosition[i];
            events[eventCount].kind = 1;
            events[eventCount].right = 0;
            events[eventCount].extra = 0;
            ++eventCount;
        }
        for (int i = 0; i < K; ++i) {
            int l, r, t;
            scanf("%d %d %d", &l, &r, &t);
            // 区间内原本的树数减去必须保留的 t，就是这段区间允许砍掉的最大棵数
            int existing = int(upper_bound(treePosition, treePosition + N, r)
                              - lower_bound(treePosition, treePosition + N, l));
            events[eventCount].position = l;
            events[eventCount].kind = 0;
            events[eventCount].right = r;
            events[eventCount].extra = existing - t;
            ++eventCount;
        }
        sort(events, events + eventCount, cmpEvent);

        int cut = 0; // 目前已经砍掉的树数
        // 堆中每项是 (累计砍树上限, 失效坐标 r)，含义是"砍树总数不得超过这个上限"。
        // 上限越小越紧，所以用小根堆，堆顶就是当前最紧的限制。
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> limits;
        for (int i = 0; i < eventCount; ++i) {
            if (events[i].kind == 0) {
                // 进入区间时就记下"从此刻起最多还能砍多少棵"
                limits.push(make_pair(cut + events[i].extra, events[i].right));
            } else {
                while (!limits.empty() && limits.top().second < events[i].position) {
                    limits.pop(); // 右端点已经过去，这条限制不再管后面的树
                }
                if (limits.empty() || limits.top().first > cut) {
                    ++cut; // 所有生效限制都还有余量，砍掉这棵树一定不亏
                }
            }
        }
        printf("%d\n", cut);
    }
    return 0;
}
