#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXG = 100005;

struct Grazing {
    long long x, y, t;
};

int g, n;
Grazing grazing[MAXG];    // 所有啃食，按时间升序

// 按时间排序用的比较函数；同一时刻内部的先后顺序无关紧要
bool byTime(const Grazing &a, const Grazing &b) {
    return a.t < b.t;
}

// 从 (x1, y1) 时刻 t1 出发，能否在时刻 t2 赶到 (x2, y2)：
// 直线距离不超过时间差即可，两边平方避免开根号（数值最大约 8 * 10^18，long long 够用）
bool reachable(const Grazing &u, const Grazing &v) {
    long long dx = u.x - v.x;
    long long dy = u.y - v.y;
    long long dt = u.t - v.t;
    return dx * dx + dy * dy <= dt * dt;
}

// 第一个时间严格大于 t 的啃食下标（都不满足时返回 g）
int firstAfter(long long t) {
    int lo = 0, hi = g;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (grazing[mid].t > t) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

int main() {
    scanf("%d %d", &g, &n);
    for (int i = 0; i < g; ++i) {
        scanf("%lld %lld %lld", &grazing[i].x, &grazing[i].y, &grazing[i].t);
    }
    sort(grazing, grazing + g, byTime);

    int innocent = 0;
    for (int i = 0; i < n; ++i) {
        Grazing alibi;
        scanf("%lld %lld %lld", &alibi.x, &alibi.y, &alibi.t);

        // firstAfter 之前的最后一次啃食如果也发生在时刻 t，就是"同一时刻"的那次
        int after = firstAfter(alibi.t);
        int same = (after > 0 && grazing[after - 1].t == alibi.t) ? after - 1 : -1;

        bool proves = false;    // 这份不在场证明能否证明提交者无罪
        if (same >= 0) {
            // 同一时刻只能在一个地点，而数据保证一头牛能走遍所有啃食，
            // 所以同时刻的啃食必然同地点；地点对不上就直接无罪
            if (grazing[same].x != alibi.x || grazing[same].y != alibi.y) proves = true;
        }
        // 时间不晚于 t 的最近一次啃食能赶到，则更早的啃食也都能赶到（三角形不等式 + 数据保证）
        int earlier = (same >= 0) ? same - 1 : after - 1;
        if (!proves && earlier >= 0 && !reachable(alibi, grazing[earlier])) proves = true;
        // 时间晚于 t 的最近一次啃食同理：它能赶到，更晚的也都能赶到
        if (!proves && after < g && !reachable(alibi, grazing[after])) proves = true;

        if (proves) ++innocent;
    }
    printf("%d\n", innocent);
    return 0;
}
