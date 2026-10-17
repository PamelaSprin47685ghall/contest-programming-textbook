#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 50005;

struct Point {
    long long x;
    long long y;
};

int n;
Point teamA[MAXN];
Point teamB[MAXN];
Point hull[MAXN];  // 凸包顶点, 逆时针排列, hull[0] 是最下面的那个顶点
int hullCnt;

long long Cross(const Point &o, const Point &a, const Point &b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

bool CmpPoint(const Point &a, const Point &b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

// 单调链法求凸包. 判定用 <= 0 弹出, 于是边界上的共线中间点都被丢掉,
// 凸包严格凸, 这样从 hull[0] 看其他顶点时极角互不相同, 后面才能二分.
void BuildHull(Point *p, int m) {
    sort(p, p + m, CmpPoint);
    int k = 0;
    for (int i = 0; i < m; i++) {
        while (k >= 2 && Cross(hull[k - 2], hull[k - 1], p[i]) <= 0) k--;
        hull[k++] = p[i];
    }
    int lowerSize = k + 1;  // 上凸包能弹到的下界, 别把下凸包已经放好的点弹掉
    for (int i = m - 2; i >= 0; i--) {
        while (k >= lowerSize && Cross(hull[k - 2], hull[k - 1], p[i]) <= 0) k--;
        hull[k++] = p[i];
    }
    hullCnt = k - 1;  // 起点被写了两遍, 去掉最后一个

    // 把最下面的顶点换到 0 号位: 从它出发看别的顶点, 极角都在 [0, π) 内且递增.
    int lowest = 0;
    for (int i = 1; i < hullCnt; i++) {
        if (hull[i].y < hull[lowest].y ||
            (hull[i].y == hull[lowest].y && hull[i].x < hull[lowest].x)) {
            lowest = i;
        }
    }
    rotate(hull, hull + lowest, hull + hullCnt);
}

// 判断 p 是否落在凸包内部或边界上.
bool Inside(const Point &p) {
    if (hullCnt == 1) return p.x == hull[0].x && p.y == hull[0].y;
    if (hullCnt == 2) {
        // 所有点共线时凸包退化成一条线段, 用点积判断 p 是否夹在两个端点之间.
        if (Cross(hull[0], hull[1], p) != 0) return false;
        long long back = (p.x - hull[0].x) * (hull[1].x - hull[0].x) +
                         (p.y - hull[0].y) * (hull[1].y - hull[0].y);
        long long front = (p.x - hull[1].x) * (hull[0].x - hull[1].x) +
                          (p.y - hull[1].y) * (hull[0].y - hull[1].y);
        return back >= 0 && front >= 0;
    }
    // 先排除落在 hull[0] 张开的两条边界射线之外的情况.
    if (Cross(hull[0], hull[1], p) < 0) return false;
    if (Cross(hull[0], hull[hullCnt - 1], p) > 0) return false;
    // 二分找出 p 的极角夹在哪两个相邻顶点之间.
    int lo = 1;
    int hi = hullCnt - 1;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (Cross(hull[0], hull[mid], p) <= 0) hi = mid;
        else lo = mid + 1;
    }
    if (Cross(hull[0], hull[lo], p) == 0) {
        // p 与 hull[0], hull[lo] 共线: 只有落在 hull[0] 到 hull[lo] 这一段上才算数.
        long long dx = p.x - hull[0].x;
        long long dy = p.y - hull[0].y;
        long long hx = hull[lo].x - hull[0].x;
        long long hy = hull[lo].y - hull[0].y;
        return dx * dx + dy * dy <= hx * hx + hy * hy;
    }
    // p 在 hull[lo-1] 与 hull[lo] 之间: 只要不在这条边的外侧就还在凸包里.
    return Cross(hull[lo - 1], hull[lo], p) >= 0;
}

int CountCaptured(const Point *team, int m) {
    int cnt = 0;
    for (int i = 0; i < m; i++) {
        if (Inside(team[i])) cnt++;
    }
    return cnt;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%lld %lld", &teamA[i].x, &teamA[i].y);
    for (int i = 0; i < n; i++) scanf("%lld %lld", &teamB[i].x, &teamB[i].y);

    // 一颗石子能被对方三颗石子围出的三角形盖住, 等价于它落在对方的凸包内:
    // 所有三角形恰好拼满凸包(平面上凸包内任一点都在某个三角形里), 反过来
    // 三角形都在凸包里, 所以两者是同一件事. 于是先求凸包, 再数点.
    BuildHull(teamA, n);
    int scoreA = CountCaptured(teamB, n);
    BuildHull(teamB, n);
    int scoreB = CountCaptured(teamA, n);

    printf("%d %d\n", scoreA, scoreB);
    return 0;
}
