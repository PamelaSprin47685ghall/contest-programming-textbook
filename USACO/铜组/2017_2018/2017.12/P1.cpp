#include <algorithm>
#include <cstdio>

using namespace std;

// 一个矩形, 用左下角 (X0, Y0) 和右上角 (X1, Y1) 表示.
struct Rectangle {
    int X0, Y0, X1, Y1;

    void Read() {
        scanf("%d%d%d%d", &X0, &Y0, &X1, &Y1);
    }

    // 两个矩形的交仍是矩形: 每边取更靠内的那条边.
    Rectangle Intersect(const Rectangle& other) const {
        Rectangle result;
        result.X0 = max(X0, other.X0);
        result.Y0 = max(Y0, other.Y0);
        result.X1 = min(X1, other.X1);
        result.Y1 = min(Y1, other.Y1);
        return result;
    }

    // 不相交时宽或高为负, 面积按 0 处理.
    int Area() const {
        return max(0, X1 - X0) * max(0, Y1 - Y0);
    }
};

int main() {
    Rectangle billboard0, billboard1, truck;
    billboard0.Read();
    billboard1.Read();
    truck.Read();
    // 被卡车遮住的部分不算可见, 从每块广告牌面积里扣掉.
    int visible0 = billboard0.Area() - billboard0.Intersect(truck).Area();
    int visible1 = billboard1.Area() - billboard1.Intersect(truck).Area();
    printf("%d\n", visible0 + visible1);
    return 0;
}