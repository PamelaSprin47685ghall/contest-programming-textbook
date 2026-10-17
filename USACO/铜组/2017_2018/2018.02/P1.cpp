#include <algorithm>
#include <cstdio>
#include <cstdlib>

using namespace std;

int main() {
    int a, b, x, y;
    scanf("%d%d%d%d", &a, &b, &x, &y);
    // 三种走法: 不走传送门, 正着用传送门, 反着用传送门.
    int direct = abs(a - b);
    int viaXY = abs(a - x) + abs(y - b);
    int viaYX = abs(a - y) + abs(x - b);
    printf("%d\n", min(direct, min(viaXY, viaYX)));
    return 0;
}