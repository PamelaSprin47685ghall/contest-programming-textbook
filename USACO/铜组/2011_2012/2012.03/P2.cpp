#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

struct Points {
    int n;
    vector<int> x, y;

    void Read() {
        scanf("%d", &n);
        x.resize(n + 1);  // 下标 0 是起点 (0, 0)
        y.resize(n + 1);
        for (int i = 1; i <= n; ++i) scanf("%d%d", &x[i], &y[i]);
    }
};

// 从点 p0 走到点 p1 的方向编码: dx, dy 各取符号, 组合成 0..8 的一个值.
int Direction(const Points& points, int p0, int p1) {
    auto sign = [](int v) { return (0 < v) - (v < 0); };
    return sign(points.x[p1] - points.x[p0]) * 3 +
           sign(points.y[p1] - points.y[p0]);
}

// 枚举所有点的排列, 要求相邻两步方向必须不同 (不能走直线).
struct Search {
    const Points* points;
    vector<int> order;   // 当前排列, 下标 0 固定为起点
    int answer = 0;

    void Compute(int index, int lastDirection) {
        if (index > points->n) {
            // 走回起点 (下标 0), 且最后一步与上一步方向不同才计数.
            int finalDirection = Direction(*points, order[index - 1], order[0]);
            if (finalDirection % 2 != 0 && lastDirection != finalDirection) {
                ++answer;
            }
            return;
        }
        for (int i = index; i <= points->n; ++i) {
            swap(order[index], order[i]);
            int nextDirection = Direction(*points, order[index - 1], order[index]);
            // 方向必须非零 (不能原地不动), 且与上一步不同.
            if (nextDirection % 2 != 0 && nextDirection != lastDirection) {
                Compute(index + 1, nextDirection);
            }
            swap(order[index], order[i]);
        }
    }
};

int main() {
    Points points;
    points.Read();
    Search search;
    search.points = &points;
    search.order.resize(points.n + 1);
    for (int i = 0; i <= points.n; ++i) search.order[i] = i;
    search.Compute(1, 0);
    printf("%d\n", search.answer);
    return 0;
}