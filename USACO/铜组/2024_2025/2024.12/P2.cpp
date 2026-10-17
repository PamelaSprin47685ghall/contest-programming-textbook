#include <cstdio>
#include <vector>

using namespace std;

int main() {
    int n, q;
    if (scanf("%d%d", &n, &q) != 2) return 0;

    // 分别统计 3 个投影方向上 (a, b) 处已经打通的方块数:
    // xy[x][y]: 在固定 (x, y) 处, 贯穿 z 轴方向打通的格子数
    // xz[x][z]: 在固定 (x, z) 处, 贯穿 y 轴方向打通的格子数
    // yz[y][z]: 在固定 (y, z) 处, 贯穿 x 轴方向打通的格子数
    vector<vector<int>> xy(n, vector<int>(n, 0));
    vector<vector<int>> xz(n, vector<int>(n, 0));
    vector<vector<int>> yz(n, vector<int>(n, 0));

    int completeCount = 0;

    for (int i = 0; i < q; ++i) {
        int x, y, z;
        scanf("%d%d%d", &x, &y, &z);

        if (++xy[x][y] == n) ++completeCount;
        if (++xz[x][z] == n) ++completeCount;
        if (++yz[y][z] == n) ++completeCount;

        printf("%d\n", completeCount);
    }

    return 0;
}
