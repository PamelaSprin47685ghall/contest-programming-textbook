#include <algorithm>
#include <cstdio>

using namespace std;

// 一个矩形, 用左下角 (X0, Y0) 和右上角 (X1, Y1) 表示.
struct Rectangle {
    int X0, Y0, X1, Y1;

    void Read() {
        scanf("%d%d%d%d", &X0, &Y0, &X1, &Y1);
    }

    // 两矩形的交仍是矩形: 每边取更靠内的那条边.
    Rectangle Intersect(const Rectangle& other) const {
        Rectangle result;
        result.X0 = max(X0, other.X0);
        result.Y0 = max(Y0, other.Y0);
        result.X1 = min(X1, other.X1);
        result.Y1 = min(Y1, other.Y1);
        return result;
    }

    // 不相交时宽或高为负, 按 0 处理.
    int Area() const {
        return max(0, X1 - X0) * max(0, Y1 - Y0);
    }
};

int main() {
    // b0 是被遮挡的广告牌, b1 是遮挡它的广告牌(题目保证 b1 与 b0 有重叠).
    Rectangle billboard, blocker;
    billboard.Read();
    blocker.Read();
    Rectangle overlap = billboard.Intersect(blocker);
    // 数一数 b0 有几条边没有被遮挡矩形贴住.
    // 若遮挡只盖住 b0 的一角(两条边), 剩余部分仍是整块, 面积不变;
    // 若遮挡贴住了三条边以上, 剩余部分才是"整块面积减遮挡面积".
    int uncoveredEdges = 0;
    uncoveredEdges += (overlap.X0 != billboard.X0) + (overlap.X1 != billboard.X1);
    uncoveredEdges += (overlap.Y0 != billboard.Y0) + (overlap.Y1 != billboard.Y1);
    if (2 <= uncoveredEdges) {
        printf("%d\n", billboard.Area());
    } else {
        printf("%d\n", billboard.Area() - overlap.Area());
    }
    return 0;
}