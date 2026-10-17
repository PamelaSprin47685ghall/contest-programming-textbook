#include <cstdio>

using namespace std;

// 思路概述：
//   1) 把矩形边界看成平面图：V = 4N + I, E = 4N + 2I（I 是交点个数），
//      由欧拉公式得区域总数 = I + C + 1，C 是边界的连通分量数。
//   2) 每个区域的"深度"（包含它的矩形个数）的奇偶性决定黑白，深度为偶数即白色。
//   3) 用扫描线 + 树状数组求 I；用树状数组求出 4N 个"采样点"的颜色
//      （每个矩形在两条竖边的外侧、靠近两端各取一点），由此得到黑白区
//      域数的带符号公式；用并查集求 C，并判断每个连通分量浸没在什么颜色里。

const int MAXC = 200005;   // 2N 个坐标

int N, T, M;               // M = 2N，所有坐标都在 1..M 内
int ft[MAXC];              // 树状数组：当前扫描线上激活的水平边（按 y 计数）

struct Event {
    int l, r, id;          // 矩形的下边、上边 y 坐标与该矩形编号
    bool start;            // 左边界为真（开始事件），右边界为假（结束事件）
} events[MAXC];

int who[MAXC];             // who[y] = 在坐标 y 处有角点的矩形编号

// 线段树：维护"当前扫描线上激活的角点"。
// st_cnt：区间内角点个数；st_lazy：区间内所有角点要合并到的代表元。
// 只需存这两个量，就能把"把 v 与 [l, r] 内所有角点合并"摊还到 O(log N)。
int st_cnt[4 * MAXC];
int st_lazy[4 * MAXC];

int uf[MAXC];

int find(int a) { return a == uf[a] ? a : (uf[a] = find(uf[a])); }

void merge(int a, int b) { uf[find(a)] = find(b); }

int exterior[MAXC];        // 每个连通分量最左竖直边外侧区域的深度奇偶性
bool vis[MAXC];

void update(int i, int v) {
    for (; i <= M; i += i & -i) ft[i] += v;
}

int query(int i) {
    int s = 0;
    for (; i > 0; i -= i & -i) s += ft[i];
    return s;
}

// 把代表元 v 记到结点 w 的区间上：区间为空则什么也不做（否则两个都合并到
// 同一个空区间的矩形会被错误地连到一起）；区间已有代表元就直接合并。
void apply(int w, int v) {
    if (!st_cnt[w]) return;
    if (st_lazy[w]) {
        merge(v, st_lazy[w]);
    } else {
        st_lazy[w] = v;
    }
}

void push(int w, int L, int R) {
    if (!st_lazy[w]) return;
    if (R - L > 1) {
        apply(w * 2, st_lazy[w]);
        apply(w * 2 + 1, st_lazy[w]);
    } else {
        // 叶子：区间里唯一坐标 R 上的角点确实属于 who[R]，可以真正合并了
        merge(st_lazy[w], who[R]);
    }
    st_lazy[w] = 0;
}

void pull(int w) { st_cnt[w] = st_cnt[w * 2] + st_cnt[w * 2 + 1]; }

// 把矩形 v 与坐标落在 (a, b) 内的所有激活角点合并（结点 [L, R) 覆盖坐标 L+1..R）
void update_range_merge(int w, int L, int R, int a, int b, int v) {
    push(w, L, R);
    if (a >= R || b <= L) return;
    if (a <= L && b >= R) {
        apply(w, v);
        push(w, L, R);
    } else {
        int mid = (L + R) / 2;
        update_range_merge(w * 2, L, mid, a, b, v);
        update_range_merge(w * 2 + 1, mid, R, a, b, v);
        pull(w);
    }
}

// 坐标 i 处的角点数增减 v
void update_inc(int w, int L, int R, int i, int v) {
    push(w, L, R);
    if (i <= L || i > R) return;
    if (R - L == 1) {
        st_cnt[w] += v;
    } else {
        int mid = (L + R) / 2;
        update_inc(w * 2, L, mid, i, v);
        update_inc(w * 2 + 1, mid, R, i, v);
        pull(w);
    }
}

// 扫描结束后把还挂在内部结点上的延迟合并推到叶子，完成所有合并
void force_lazy(int w, int L, int R) {
    push(w, L, R);
    if (R - L > 1) {
        int mid = (L + R) / 2;
        force_lazy(w * 2, L, mid);
        force_lazy(w * 2 + 1, mid, R);
    }
}

int main() {
    scanf("%d %d", &N, &T);
    M = 2 * N;
    for (int i = 1; i <= N; i++) {
        int x1, y1, x2, y2;
        scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
        events[x1].l = y1;
        events[x1].r = y2;
        events[x1].id = i;
        events[x1].start = true;
        events[x2] = events[x1];
        events[x2].start = false;
        who[y1] = i;
        who[y2] = i;
        uf[i] = i;
    }

    int corners[2] = {0, 0};   // 采样点中深度为偶 / 奇（即白 / 黑）的个数
    long long intersections = 0;

    for (int x = 1; x <= M; x++) {
        int l = events[x].l, r = events[x].r, id = events[x].id;
        if (events[x].start) {
            // 左竖边：插入之前，query(y) 的奇偶性就是点 (x-eps, y+eps) 所在
            // 区域的深度奇偶，也就是该采样点的颜色
            exterior[id] = query(l) & 1;
            corners[query(l) & 1]++;
            corners[query(r) & 1]++;
            // 区间 (l, r) 内的水平边都会与这条竖边相交
            intersections += query(r) - query(l);
            // 竖边与这些水平边相交 => 与它们的矩形属于同一连通分量
            update_range_merge(1, 0, M, l, r, id);
            update(l, 1);
            update(r, 1);
            update_inc(1, 0, M, l, 1);
            update_inc(1, 0, M, r, 1);
        } else {
            update(l, -1);
            update(r, -1);
            update_inc(1, 0, M, l, -1);
            update_inc(1, 0, M, r, -1);
            intersections += query(r) - query(l);
            update_range_merge(1, 0, M, l, r, id);
            corners[query(l) & 1]++;
            corners[query(r) & 1]++;
        }
    }

    force_lazy(1, 0, M);

    // 每个连通分量按"最左竖边"分类：它左侧紧邻的区域就是该分量的外部区域
    int black_immersed = 0, white_immersed = 0;
    for (int x = 1; x <= M; x++) {
        if (!events[x].start) continue;
        int root = find(events[x].id);
        if (vis[root]) continue;
        vis[root] = true;
        if (exterior[events[x].id]) {
            black_immersed++;
        } else {
            white_immersed++;
        }
    }

    long long black_corners = (long long)corners[0] - corners[1] + 2 * intersections;
    long long white_corners = (long long)corners[1] - corners[0] + 2 * intersections;
    long long black_regions = black_corners / 4 + black_immersed;
    long long white_regions = white_corners / 4 + white_immersed + 1;

    if (T == 1) {
        printf("%lld\n", white_regions + black_regions);
    } else {
        printf("%lld %lld\n", white_regions, black_regions);
    }
    return 0;
}
