#include <cstdio>
#include <set>
#include <algorithm>

using namespace std;

const int MAXN = 100000;
const int PROBE = -1;   // 在比较函数里代表"按高度定位用的探针", 不会真的进集合

struct Point {
    long long x, y;
};

struct Segment {
    Point a, b;   // 统一存成 a.x < b.x; 竖直段则是 a.x == b.x 且 a.y < b.y
    int id;
};

struct Event {
    long long x;
    int kind;   // 0: 竖直段(删除之前探针) 1: 右端点 2: 左端点 3: 竖直段(插入之后探针)
    int id;
};

// 同一列上的事件必须按 探针 -> 删除 -> 插入 -> 探针 的顺序处理:
// 这样两次探针就能把"右端点落在这一列"和"左端点落在这一列"的线段都覆盖到.
bool operator<(const Event &e1, const Event &e2) {
    if (e1.x != e2.x) return e1.x < e2.x;
    if (e1.kind != e2.kind) return e1.kind < e2.kind;
    return e1.id < e2.id;
}

int n;
Segment seg[MAXN];
long long curX;         // 扫描线当前的 x 坐标
long double probeY;     // 探针高度

// 线段在横坐标 curX 处的纵坐标. 集合里的线段都跨过 curX, 所以结果落在两个端点的
// y 之间 (不超过 10^9), long double 的 64 位尾数足以分清两个不同的高度.
long double heightAt(int i) {
    const Segment &s = seg[i];
    if (s.a.x == s.b.x) {
        return (long double)s.a.y;   // 竖直段不进集合, 这里只是兜底
    }
    long double dx = (long double)(s.b.x - s.a.x);
    return (long double)s.a.y + (long double)(s.b.y - s.a.y) * (long double)(curX - s.a.x) / dx;
}

long double keyOf(int i) {
    if (i == PROBE) return probeY;
    return heightAt(i);
}

struct HeightCmp {
    bool operator()(int i, int j) const {
        long double ki = keyOf(i), kj = keyOf(j);
        if (ki != kj) return ki < kj;
        return i < j;   // 高度恰好相同(说明两者在该处相交)时用编号决出先后, 保证是严格弱序
    }
};

long long cross(const Point &o, const Point &u, const Point &v) {
    return (u.x - o.x) * (v.y - o.y) - (u.y - o.y) * (v.x - o.x);
}

// 已知 p, q, r 共线, 判断 q 是否落在线段 pr 上
bool onSegment(const Point &p, const Point &q, const Point &r) {
    long long lo = p.x < r.x ? p.x : r.x, hi = p.x < r.x ? r.x : p.x;
    if (q.x < lo || q.x > hi) return false;
    lo = p.y < r.y ? p.y : r.y;
    hi = p.y < r.y ? r.y : p.y;
    return lo <= q.y && q.y <= hi;
}

// 叉积的绝对值最多约 2*10^18, long long 放得下; 题目要求端点相碰也算相交, 所以要判共线情形
bool isect(const Segment &s1, const Segment &s2) {
    long long d1 = cross(s1.a, s1.b, s2.a);
    long long d2 = cross(s1.a, s1.b, s2.b);
    long long d3 = cross(s2.a, s2.b, s1.a);
    long long d4 = cross(s2.a, s2.b, s1.b);
    if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
        ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
        return true;
    }
    if (d1 == 0 && onSegment(s1.a, s2.a, s1.b)) return true;
    if (d2 == 0 && onSegment(s1.a, s2.b, s1.b)) return true;
    if (d3 == 0 && onSegment(s2.a, s1.a, s2.b)) return true;
    if (d4 == 0 && onSegment(s2.a, s1.b, s2.b)) return true;
    return false;
}

Event events[2 * MAXN];
int verticalIdx[MAXN];

bool verticalLess(int i, int j) {
    if (seg[i].a.x != seg[j].a.x) return seg[i].a.x < seg[j].a.x;
    return seg[i].a.y < seg[j].a.y;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        long long x1, y1, x2, y2;
        scanf("%lld%lld%lld%lld", &x1, &y1, &x2, &y2);
        if (x1 > x2 || (x1 == x2 && y1 > y2)) {
            long long t = x1; x1 = x2; x2 = t;
            t = y1; y1 = y2; y2 = t;
        }
        seg[i].a.x = x1; seg[i].a.y = y1;
        seg[i].b.x = x2; seg[i].b.y = y2;
        seg[i].id = i;
    }

    int ans1 = -1, ans2 = -1;

    // 竖直段不放进活动集合 (它们在集合里没有"高度"可言), 改为用一次探针查询找出
    // 与它同列相交的线段; 两条竖直段共线重叠这种退化情形则在这里单独检查.
    int verticalCnt = 0;
    for (int i = 0; i < n; ++i) {
        if (seg[i].a.x == seg[i].b.x) verticalIdx[verticalCnt++] = i;
    }
    sort(verticalIdx, verticalIdx + verticalCnt, verticalLess);
    for (int k = 0; k + 1 < verticalCnt; ++k) {
        int p = verticalIdx[k], q = verticalIdx[k + 1];
        if (seg[p].a.x == seg[q].a.x && seg[q].a.y <= seg[p].b.y) {
            ans1 = p; ans2 = q;
        }
    }

    int eventCnt = 0;
    for (int i = 0; i < n; ++i) {
        if (seg[i].a.x == seg[i].b.x) {
            events[eventCnt].x = seg[i].a.x; events[eventCnt].kind = 0; events[eventCnt].id = i; ++eventCnt;
            events[eventCnt].x = seg[i].a.x; events[eventCnt].kind = 3; events[eventCnt].id = i; ++eventCnt;
        } else {
            events[eventCnt].x = seg[i].a.x; events[eventCnt].kind = 2; events[eventCnt].id = i; ++eventCnt;
            events[eventCnt].x = seg[i].b.x; events[eventCnt].kind = 1; events[eventCnt].id = i; ++eventCnt;
        }
    }
    sort(events, events + eventCnt);

    set<int, HeightCmp> active;
    curX = events[0].x;
    for (int e = 0; e < eventCnt && ans1 < 0; ++e) {
        curX = events[e].x;
        int id = events[e].id;
        if (events[e].kind == 0 || events[e].kind == 3) {
            // 此刻集合里的每条线段都跨过或触及这一列, 高度落在竖直段 y 区间内的那条
            // 必然与竖直段相交, 而集合又按高度有序, 二分一次就能找到.
            probeY = (long double)seg[id].a.y;
            set<int, HeightCmp>::iterator it = active.lower_bound(PROBE);
            if (it != active.end() && heightAt(*it) <= (long double)seg[id].b.y) {
                ans1 = id;
                ans2 = *it;
            }
        } else if (events[e].kind == 2) {
            active.insert(id);
            set<int, HeightCmp>::iterator it = active.find(id);
            set<int, HeightCmp>::iterator jt = it;
            for (int k = 0; k < 2 && jt != active.begin(); ++k) {
                --jt;
                if (isect(seg[id], seg[*jt])) { ans1 = id; ans2 = *jt; break; }
            }
            if (ans1 < 0) {
                jt = it;
                for (int k = 0; k < 2; ++k) {
                    ++jt;
                    if (jt == active.end()) break;
                    if (isect(seg[id], seg[*jt])) { ans1 = id; ans2 = *jt; break; }
                }
            }
        } else {
            // 删掉这一条之后, 原本隔着它的上下两条变成邻居, 相交的一对正是在这种
            // 相邻关系发生变化时才会进入视野的, 所以这一对必须补测.
            set<int, HeightCmp>::iterator it = active.find(id);
            set<int, HeightCmp>::iterator after = it;
            ++after;
            if (it != active.begin() && after != active.end()) {
                set<int, HeightCmp>::iterator before = it;
                --before;
                if (isect(seg[*before], seg[*after])) { ans1 = *before; ans2 = *after; }
            }
            active.erase(it);
        }
    }

    // 题目保证删掉某一条之后就没有交点了, 于是每一对相交的线段都含那条"坏线段".
    // 找到的一对里若较大编号者还与别的线段相交, 那条坏线段只能是它 (否则删另一条
    // 会留下它和第三者的交点); 否则这一对就是仅有的交点, 两者都能删, 取编号小的.
    if (ans1 > ans2) {
        int t = ans1; ans1 = ans2; ans2 = t;
    }
    int cntAns2 = 0;
    for (int i = 0; i < n; ++i) {
        if (i != ans2 && isect(seg[i], seg[ans2])) ++cntAns2;
    }
    printf("%d\n", cntAns2 > 1 ? ans2 + 1 : ans1 + 1);
    return 0;
}
