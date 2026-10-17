#include <cstdio>
#include <vector>

struct Cell {
    int type;  // 0: 跳板, 1: 目标
    int val;
};

int main() {
    int n, s;
    scanf("%d%d", &n, &s);
    --s;  // 转换为 0-based

    std::vector<Cell> grid(n);
    for (int i = 0; i < n; ++i) {
        scanf("%d%d", &grid[i].type, &grid[i].val);
    }

    std::vector<bool> broken(n, false);
    int brokenCount = 0;

    int pos = s;
    long long power = 1;
    int dir = 1;  // +1: 向右, -1: 向左

    // 循环模拟: 如果步数超过上限 (例如 5000000 步) 则必然陷入循环
    for (int step = 0; step < 5000000; ++step) {
        if (pos < 0 || pos >= n) break;

        if (grid[pos].type == 0) {
            // 跳板: 反转方向, 威力增加 v
            dir = -dir;
            power += grid[pos].val;
        } else {
            // 目标: 如果威力 >= 目标阻力且未被击碎
            if (!broken[pos] && power >= grid[pos].val) {
                broken[pos] = true;
                ++brokenCount;
            }
        }

        pos += dir * power;
    }

    printf("%d\n", brokenCount);
    return 0;
}
