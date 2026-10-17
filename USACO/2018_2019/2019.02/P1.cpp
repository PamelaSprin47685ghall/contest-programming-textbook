#include <algorithm>
#include <cstdio>

int main() {
    int p[3];
    scanf("%d%d%d", &p[0], &p[1], &p[2]);
    std::sort(p, p + 3);

    // 最小步数:
    // 1. 如果已经连续 (如 3, 4, 5), 需 0 步;
    // 2. 如果存在一对相邻相隔恰好为 2 (例如 3, 5, 9), 只需把 9 插入 3 和 5 中间 (即 4), 1 步完成;
    // 3. 否则两步必定可以搞定 (先移端点到中间旁边相距 2, 再移另一端点).
    int minMoves;
    if (p[0] + 1 == p[1] && p[1] + 1 == p[2]) {
        minMoves = 0;
    } else if (p[1] - p[0] == 2 || p[2] - p[1] == 2) {
        minMoves = 1;
    } else {
        minMoves = 2;
    }

    // 最大步数:
    // 每次只能将端点移入内部, 相当于消耗一段间隙.
    // 第一次移动选消除较小的那个间隙, 留出最大的间隙逐格移动.
    int maxMoves = std::max(p[1] - p[0], p[2] - p[1]) - 1;

    printf("%d\n%d\n", minMoves, maxMoves);
    return 0;
}
