#include <algorithm>
#include <cstdio>

// 矩形用左下角与右上角两个点表示.
struct Box {
    int left;
    int bottom;
    int right;
    int top;

    Box& Read() {
        scanf("%d%d%d%d", &left, &bottom, &right, &top);
        return *this;
    }

    // 题目要求合并后是正方形, 边长必须同时盖住宽和高.
    int SquareArea() const {
        int side = std::max(right - left, top - bottom);
        return side * side;
    }
};

// 合并后的边界取所有坐标的极值, 才能把两块地都包住.
Box Combine(const Box& first, const Box& second) {
    return Box{std::min(first.left, second.left),
               std::min(first.bottom, second.bottom),
               std::max(first.right, second.right),
               std::max(first.top, second.top)};
}

int main() {
    Box first = Box{}.Read();
    Box second = Box{}.Read();
    printf("%d\n", Combine(first, second).SquareArea());
    return 0;
}
