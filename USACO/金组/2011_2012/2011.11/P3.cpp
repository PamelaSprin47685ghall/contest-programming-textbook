#include <cstdio>

using namespace std;

const int MAXN = 255;

struct HSeg {
    int x1, x2, y;
};

struct VSeg {
    int x, y1, y2;
};

int N, H, V;
HSeg horiz[MAXN];
VSeg vert[MAXN];

// 横竖线段相交当且仅当横线的 y 落在竖线区间内, 且竖线的 x 落在横线区间内
bool cross[MAXN][MAXN];

int matchH[MAXN];  // matchH[h]: 与第 h 条横线匹配的竖线编号, -1 表示未匹配
bool vis[MAXN];

// 匈牙利算法: 为竖线 v 找增广路
bool tryKuhn(int v) {
    for (int h = 0; h < H; h++) {
        if (!cross[h][v] || vis[h]) {
            continue;
        }
        vis[h] = true;
        // 横线 h 还空着, 或者能从它原来的搭档那里腾出位置
        if (matchH[h] == -1 || tryKuhn(matchH[h])) {
            matchH[h] = v;
            return true;
        }
    }
    return false;
}

int main() {
    scanf("%d", &N);

    H = V = 0;
    for (int i = 0; i < N; i++) {
        int x1, y1, x2, y2;
        scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
        if (y1 == y2) {
            // 水平线段: 端点顺序可能相反, 统一成 x1 <= x2
            if (x1 > x2) {
                int t = x1;
                x1 = x2;
                x2 = t;
            }
            horiz[H].x1 = x1;
            horiz[H].x2 = x2;
            horiz[H].y = y1;
            H++;
        } else {
            // 垂直线段: 统一成 y1 <= y2
            if (y1 > y2) {
                int t = y1;
                y1 = y2;
                y2 = t;
            }
            vert[V].x = x1;
            vert[V].y1 = y1;
            vert[V].y2 = y2;
            V++;
        }
    }

    for (int h = 0; h < H; h++) {
        for (int v = 0; v < V; v++) {
            cross[h][v] = (vert[v].y1 <= horiz[h].y && horiz[h].y <= vert[v].y2)
                          && (horiz[h].x1 <= vert[v].x && vert[v].x <= horiz[h].x2);
        }
    }

    // 把"保留尽量多不相交线段"看成二分图最大独立集:
    // 左侧是横线, 右侧是竖线, 相交即连边. 由 König 定理,
    // 最大独立集 = 顶点总数 - 最小点覆盖 = 顶点总数 - 最大匹配.
    for (int h = 0; h < H; h++) {
        matchH[h] = -1;
    }
    int matching = 0;
    for (int v = 0; v < V; v++) {
        for (int h = 0; h < H; h++) {
            vis[h] = false;
        }
        if (tryKuhn(v)) {
            matching++;
        }
    }

    printf("%d\n", N - matching);
    return 0;
}
