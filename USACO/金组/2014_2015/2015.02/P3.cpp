#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

struct Point {
    long long x, y;
};

// 叉积 (o->a) x (o->b): > 0 表示从 o 到 a 再到 b 是左转
long long cross(Point o, Point a, Point b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

// 相对原点的点积
long long dotOrigin(Point a, Point b) {
    return a.x * b.x + a.y * b.y;
}

bool byXY(Point a, Point b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

// 求凸包顶点, 逆时针排列, 共线的中间点都丢掉
vector<Point> convexHull(vector<Point> p) {
    int n = p.size();
    if (n <= 2) return p;
    sort(p.begin(), p.end(), byXY);
    vector<Point> h(2 * n);
    int k = 0;
    for (int i = 0; i < n; i++) { // 下凸壳: 从左扫到右
        while (k >= 2 && cross(h[k - 2], h[k - 1], p[i]) <= 0) k--;
        h[k++] = p[i];
    }
    for (int i = n - 2, t = k + 1; i >= 0; i--) { // 上凸壳: 从右扫回左
        while (k >= t && cross(h[k - 2], h[k - 1], p[i]) <= 0) k--;
        h[k++] = p[i];
    }
    h.resize(k - 1); // 末尾重复了起点
    return h;
}

// 判断向量 x 是否比 y 更早出现在以 base 为起点的极角扫描里。
// 用整数实现, 避免浮点误差
bool radialLess(Point x, Point y, Point base) {
    long long cx = base.x * x.y - base.y * x.x;
    long long cy = base.x * y.y - base.y * y.x;
    if (cx == 0) cx = base.x * x.x + base.y * x.y; // 与 base 反向时算作"更晚"
    if (cy == 0) cy = base.x * y.x + base.y * y.y;
    if ((cx < 0) == (cy < 0)) return x.x * y.y - x.y * y.x > 0;
    return cy < 0;
}

// 逆时针凸包 P 上 dot(P[i], pt) 的最大值。
// 把 P[0]-P[last] 当作起扫方向, 对"边向量与 pt 逆时针转 90 度后的超前关系"
// 做二分, 找到最优顶点
long long maxDotOnHull(const vector<Point> &P, Point pt) {
    int lo = 0, hi = (int)P.size() - 1;
    Point base = {P[0].x - P[P.size() - 1].x, P[0].y - P[P.size() - 1].y};
    Point target = {-pt.y, pt.x};
    while (lo < hi) {
        int mid = (lo + hi + 1) >> 1;
        int prev = mid ? mid - 1 : (int)P.size() - 1;
        Point edge = {P[mid].x - P[prev].x, P[mid].y - P[prev].y};
        if (radialLess(edge, target, base)) lo = mid;
        else hi = mid - 1;
    }
    return dotOrigin(P[lo], pt);
}

// 倍增法动态凸包: 第 i 层放一个大小恰为 2^i 的凸包, 满了就并到上一层
struct DynHull {
    vector<vector<Point> > buckets;

    bool empty() const { return buckets.empty(); }

    void add(Point p) {
        vector<Point> merged;
        merged.push_back(p);
        for (size_t i = 0; i < buckets.size(); i++) {
            if (buckets[i].empty()) {
                buckets[i] = convexHull(merged);
                return;
            }
            for (size_t j = 0; j < buckets[i].size(); j++)
                merged.push_back(buckets[i][j]);
            buckets[i].clear();
        }
        buckets.push_back(convexHull(merged));
    }

    long long maxDot(Point pt) {
        long long res = 0;
        bool has = false;
        for (size_t i = 0; i < buckets.size(); i++) {
            if (buckets[i].empty()) continue;
            long long cur = maxDotOnHull(buckets[i], pt);
            if (!has || cur > res) {
                has = true;
                res = cur;
            }
        }
        return res;
    }
};

int main() {
    int n, q;
    scanf("%d %d", &n, &q);

    // 最初的牛群不会再变, 给它们建一个静态凸包就够了
    vector<Point> initial;
    for (int i = 0; i < n; i++) {
        long long x, y;
        scanf("%lld %lld", &x, &y);
        Point p = {x, y};
        initial.push_back(p);
    }
    vector<Point> base = convexHull(initial);

    DynHull added;
    for (int i = 0; i < q; i++) {
        int cmd;
        scanf("%d", &cmd);
        if (cmd == 1) {
            long long x, y;
            scanf("%lld %lld", &x, &y);
            Point p = {x, y};
            added.add(p);
        } else {
            long long a, b, c;
            scanf("%lld %lld %lld", &a, &b, &c);
            Point pt = {a, b};
            Point neg = {-a, -b};
            // 有牛在线下方 <=> min(Ax+By) <= c; 有牛在线同侧或上方 <=> max >= c。
            // 两者同时成立当且仅当直线穿过牛群 (或压到某头牛), 此时不可用
            bool hasBelow = dotOrigin(base[0], pt) <= c || -maxDotOnHull(base, neg) <= c;
            bool hasAbove = dotOrigin(base[0], pt) >= c || maxDotOnHull(base, pt) >= c;
            if (!added.empty()) {
                if (!hasBelow) hasBelow = -added.maxDot(neg) <= c;
                if (!hasAbove) hasAbove = added.maxDot(pt) >= c;
            }
            printf("%s\n", (hasBelow && hasAbove) ? "NO" : "YES");
        }
    }
    return 0;
}
