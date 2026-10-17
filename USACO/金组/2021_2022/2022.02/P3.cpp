#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 100000;
const int MAXY = 10;
// 每个点对每行最多留 1 条边（11 条），同一 x 上的点还要两两配边（每个点摊到至多 5.5 条）
const int MAXE = 11 * MAXN + 6 * MAXN;

struct Point {
    int x, y;
};

struct Edge {
    int a, b;
    long long w;
};

Point pts[MAXN];
Edge edges[MAXE];
int dsu[MAXN];

bool pointLess(const Point &p, const Point &q) {
    if (p.x != q.x) return p.x < q.x;
    return p.y < q.y;
}

bool edgeLess(const Edge &p, const Edge &q) {
    return p.w < q.w;
}

int findRoot(int u) {
    while (dsu[u] != u) {
        dsu[u] = dsu[dsu[u]];  // 路径压缩：顺手把祖父提上来
        u = dsu[u];
    }
    return u;
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) scanf("%d%d", &pts[i].x, &pts[i].y);
    sort(pts, pts + n, pointLess);

    // 候选边：对每个点 p 和每一行 y，只留"y 行中 x <= p.x 的最近点"。
    // 若三点 a,c,b 满足 x_a <= x_c < x_b 且 y_c = y_b，则边 a-b 一定不会被 Kruskal 选上，
    // 所以每个点对每行只需要保留最近的前驱，边数降到 O(N)。
    int cnt = 0;
    int lastRow[MAXY + 1];  // lastRow[y]：y 行中被扫描过的、x 最大的点
    for (int y = 0; y <= MAXY; ++y) lastRow[y] = -1;
    for (int i = 0; i < n;) {
        int j = i;
        while (j < n && pts[j].x == pts[i].x) ++j;  // 同一个 x 上至多 11 个点
        for (int p = i; p < j; ++p) {
            for (int y = 0; y <= MAXY; ++y) {
                int q = lastRow[y];
                if (q < 0) continue;
                long long dx = pts[p].x - pts[q].x;
                long long dy = (long long)pts[p].y - y;
                edges[cnt].a = p;
                edges[cnt].b = q;
                edges[cnt].w = dx * dx + dy * dy;
                ++cnt;
            }
        }
        // x 相同的点（必在不同行）无法被上面的三点论证排除，两两相连
        for (int p = i; p < j; ++p)
            for (int q = p + 1; q < j; ++q) {
                long long dy = (long long)pts[p].y - pts[q].y;
                edges[cnt].a = p;
                edges[cnt].b = q;
                edges[cnt].w = dy * dy;
                ++cnt;
            }
        for (int p = i; p < j; ++p) lastRow[pts[p].y] = p;
        i = j;
    }

    // Kruskal 求最小生成树
    sort(edges, edges + cnt, edgeLess);
    for (int i = 0; i < n; ++i) dsu[i] = i;
    long long cost = 0;
    int comps = n;
    for (int e = 0; e < cnt && comps > 1; ++e) {
        int ra = findRoot(edges[e].a), rb = findRoot(edges[e].b);
        if (ra == rb) continue;
        dsu[ra] = rb;
        cost += edges[e].w;
        --comps;
    }
    printf("%lld\n", cost);
    return 0;
}
