#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>

const int MAXL = 1001;
int posOnFence[MAXL][MAXL];

struct Point {
    int x, y;
};

int main() {
    int n, p;
    scanf("%d%d", &n, &p);

    std::vector<Point> posts(p);
    for (int i = 0; i < p; ++i) {
        scanf("%d%d", &posts[i].x, &posts[i].y);
    }

    for (int x = 0; x < MAXL; ++x) {
        for (int y = 0; y < MAXL; ++y) {
            posOnFence[x][y] = -1;
        }
    }

    int perimeter = 0;

    // 沿栅栏边缘遍历一圈, 记录每个整数坐标点沿栅栏周长的距离编号
    for (int i = 0; i < p; ++i) {
        Point start = posts[i];
        Point end = posts[(i + 1) % p];

        int dx = (end.x > start.x) - (end.x < start.x);
        int dy = (end.y > start.y) - (end.y < start.y);
        int dist = std::abs(end.x - start.x) + std::abs(end.y - start.y);

        for (int step = 0; step < dist; ++step) {
            posOnFence[start.x][start.y] = perimeter++;
            start.x += dx;
            start.y += dy;
        }
    }

    // 处理 N 头牛的询问
    for (int i = 0; i < n; ++i) {
        int x1, y1, x2, y2;
        scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
        int d1 = posOnFence[x1][y1];
        int d2 = posOnFence[x2][y2];

        int straightDist = std::abs(d1 - d2);
        int circularDist = std::min(straightDist, perimeter - straightDist);
        printf("%d\n", circularDist);
    }

    return 0;
}
