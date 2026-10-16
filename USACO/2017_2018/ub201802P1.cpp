#include <algorithm>
#include <cstdio>
#include <cstdlib>

int main() {
    int a, b, x, y;
    scanf("%d%d%d%d", &a, &b, &x, &y);
    // 三种走法: 不走传送门, 正着用传送门, 反着用传送门.
    int direct = std::abs(a - b);
    int viaXY = std::abs(a - x) + std::abs(y - b);
    int viaYX = std::abs(a - y) + std::abs(x - b);
    printf("%d\n", std::min(direct, std::min(viaXY, viaYX)));
    return 0;
}