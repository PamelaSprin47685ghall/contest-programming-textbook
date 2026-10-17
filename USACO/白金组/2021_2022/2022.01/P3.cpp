#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXV = 200000; // 所有组的向量总数上限

struct Vec {
    long long x, y;
};

Vec group[MAXV];   // 当前组的向量；求凸包时先平移成相对坐标
Vec offsets[MAXV]; // 所有凸包的边向量（偏移序列），总长度不超过向量总数
int offsetCount;

Vec startPoint; // 各组"字典序最小顶点"之和，作为巡回的起点

long long cross(Vec a, Vec b) { return a.x * b.y - a.y * b.x; }
long long norm2(Vec a) { return a.x * a.x + a.y * a.y; }

bool compareByArgument(Vec a, Vec b) {
    long long c = cross(a, b);
    if (c == 0) return norm2(a) < norm2(b); // 同向时靠得近的在前
    return c > 0;                           // 叉积为正说明 a 的极角更小
}

// 求当前组的凸包：平移成相对坐标后按极角排序做 Graham 扫描。
// 顶点就地存在 group[0..cnt-1]，逆时针顺序，从字典序最小的顶点开始；
// originOut 返回这个起点的绝对坐标（平移量）。
int buildHull(int g, Vec &originOut) {
    int best = 0;
    for (int i = 1; i < g; ++i)
        if (group[i].x < group[best].x || (group[i].x == group[best].x && group[i].y < group[best].y))
            best = i;
    originOut = group[best];
    Vec tmp = group[0];
    group[0] = group[best];
    group[best] = tmp;
    for (int i = 0; i < g; ++i) {
        group[i].x -= originOut.x;
        group[i].y -= originOut.y;
    }
    // 平移后所有点的方向都落在 (-90°, 90°] 内，按极角升序排列
    sort(group, group + g, compareByArgument);

    int cnt = 0;
    for (int i = 0; i < g; ++i) {
        Vec p = group[i]; // 会被覆盖，先存下来
        while (cnt >= 2) {
            Vec a = group[cnt - 1];
            Vec b = group[cnt - 2];
            // 叉积 <= 0 表示右转或共线，中间那个点不可能出现在凸包上
            if (cross(Vec{a.x - b.x, a.y - b.y}, Vec{p.x - b.x, p.y - b.y}) > 0) break;
            --cnt;
        }
        group[cnt++] = p;
    }
    return cnt;
}

// 按极角把边向量分成两半，使 (0,-1) 排在最后：
// 半区 0 是 (-90°, 90°]，半区 1 是 (90°, 270°]
int offsetHalf(Vec p) {
    if (p.x > 0 || (p.x == 0 && p.y > 0)) return 0;
    return 1;
}

// 同一个半区里按极角升序（跨度小于 180°，用叉积判断即可）
bool compareOffsets(Vec a, Vec b) {
    int halfA = offsetHalf(a), halfB = offsetHalf(b);
    if (halfA != halfB) return halfA < halfB;
    return cross(a, b) > 0;
}

int main() {
    int n;
    scanf("%d", &n);
    startPoint.x = 0;
    startPoint.y = 0;
    offsetCount = 0;

    for (int t = 0; t < n; ++t) {
        int g;
        scanf("%d", &g);
        for (int i = 0; i < g; ++i) scanf("%lld%lld", &group[i].x, &group[i].y);

        Vec origin;
        int cnt = buildHull(g, origin);
        // 每个凸包的顶点和都是可达的选择，把它们加起来作为起点
        startPoint.x += origin.x;
        startPoint.y += origin.y;
        // 凸包逆时针走一圈的边向量就是它的偏移序列（最后一个再补上回到起点的边）
        for (int i = 1; i <= cnt; ++i) {
            offsets[offsetCount].x = group[i % cnt].x - group[i - 1].x;
            offsets[offsetCount].y = group[i % cnt].y - group[i - 1].y;
            ++offsetCount;
        }
    }

    // 由交换论证，多个凸包的 Minkowski 和，其偏移序列就是把所有偏移按极角排序
    sort(offsets, offsets + offsetCount, compareOffsets);

    // 从起点出发依次累加偏移，途经的点恰好是 Minkowski 和的全部顶点
    // （共线偏移之间的中间点只会落在边上，不会漏掉更优的顶点）
    long long answer = norm2(startPoint);
    for (int i = 0; i < offsetCount; ++i) {
        startPoint.x += offsets[i].x;
        startPoint.y += offsets[i].y;
        long long d = norm2(startPoint);
        if (d > answer) answer = d;
    }
    printf("%lld\n", answer);
    return 0;
}
