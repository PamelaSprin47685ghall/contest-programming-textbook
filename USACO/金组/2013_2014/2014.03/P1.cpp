#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100005;

// 旋转坐标后, 一块草地 i 能被吃到的初始位置构成矩形 [u_i-k, u_i+k] x [v_i-k, v_i+k].
// 事件表示: 在横扫坐标 pos 处, 对 v 方向区间 [bot, top] 整体加上 delta.
struct Event {
    int pos;
    int bot;
    int top;
    int delta;
};

int n, k;
Event events[2 * MAXN];
int xs[2 * MAXN];             // v 方向所有区间端点, 排序去重后用于压缩
int ysCnt;                    // 压缩后坐标个数
int leafCnt;                  // 基本小区间个数 = ysCnt - 1
int treeMax[8 * MAXN];        // 每个结点的区间最大值
int treeLazy[8 * MAXN];       // 整个结点被整体加上的值

bool CmpEvent(const Event &a, const Event &b) {
    return a.pos < b.pos;
}

// 二分找到 val 在压缩坐标里的下标 (val 必定存在)
int IdxOf(int val) {
    int lo = 0, hi = ysCnt - 1;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (xs[mid] < val) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

// 区间加, 结点最大值 = 自身懒标记 + 两个孩子最大值的较大者
void Update(int node, int l, int r, int ql, int qr, int delta) {
    if (qr < l || r < ql) return;
    if (ql <= l && r <= qr) {
        treeMax[node] += delta;
        treeLazy[node] += delta;
        return;
    }
    int mid = (l + r) / 2;
    Update(node * 2, l, mid, ql, qr, delta);
    Update(node * 2 + 1, mid + 1, r, ql, qr, delta);
    treeMax[node] = treeLazy[node] + max(treeMax[node * 2], treeMax[node * 2 + 1]);
}

int main() {
    scanf("%d %d", &n, &k);
    for (int i = 0; i < n; i++) {
        int g, x, y;
        scanf("%d %d %d", &g, &x, &y);
        // 曼哈顿距离 <= k 等价于 |(x+y)-(x_i+y_i)| <= k 且 |(x-y)-(x_i-y_i)| <= k
        int u = x + y;
        int v = x - y;
        events[2 * i].pos = u - k;
        events[2 * i].bot = v - k;
        events[2 * i].top = v + k;
        events[2 * i].delta = g;
        // 离开区间时取 +k+1, 保证 u = u_i+k 处这块草地仍然算数
        events[2 * i + 1].pos = u + k + 1;
        events[2 * i + 1].bot = v - k;
        events[2 * i + 1].top = v + k;
        events[2 * i + 1].delta = -g;
        xs[2 * i] = v - k;
        xs[2 * i + 1] = v + k + 1;
    }

    sort(xs, xs + 2 * n);
    ysCnt = unique(xs, xs + 2 * n) - xs;
    leafCnt = ysCnt - 1;
    sort(events, events + 2 * n, CmpEvent);

    // 所有坐标都是整数, 而实数位置的最优解总能在某个整数位置取到, 所以按事件坐标横扫即可.
    // 同一坐标上的事件全部生效后再查询, 对应 Bessie 的初始位置就落在该坐标上.
    int ans = 0;
    int i = 0;
    while (i < 2 * n) {
        int cur = events[i].pos;
        while (i < 2 * n && events[i].pos == cur) {
            Update(1, 0, leafCnt - 1, IdxOf(events[i].bot), IdxOf(events[i].top + 1) - 1,
                   events[i].delta);
            i++;
        }
        if (treeMax[1] > ans) ans = treeMax[1];
    }
    printf("%d\n", ans);
    return 0;
}
