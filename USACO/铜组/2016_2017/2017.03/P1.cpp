#include <cstdio>

using namespace std;

int main() {
    int x, y;
    scanf("%d %d", &x, &y);

    // 模拟之字形搜索: 第 k 段从 x 出发, 走到与 x 相距 step 的位置
    // (step = 1, 2, 4, ...), 方向左右交替.
    int answer = 0;
    int step = 1;
    int dir = 1;  // 1 表示向正方向走, -1 表示向负方向走
    while (true) {
        int target = x + dir * step;
        // 这一段从 x 走到 target, 覆盖区间 [min, max].
        int lo = x < target ? x : target;
        int hi = x < target ? target : x;
        if (lo <= y && y <= hi) {
            // 途中就碰到 Bessie 了, 只需再走 x 到 y 的距离.
            answer += y > x ? y - x : x - y;
            break;
        }
        // 没碰到: 走到 target 再折返回 x, 一共 2 * step;
        // 然后步长翻倍, 方向取反.
        answer += 2 * step;
        step *= 2;
        dir = -dir;
    }

    printf("%d\n", answer);
    return 0;
}
