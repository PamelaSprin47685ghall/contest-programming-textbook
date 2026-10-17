#include <cstdio>
#include <string>
#include <vector>

using namespace std;

int n, k;
vector<string> grid;
int pathCount;

// (r, c): 当前所在坐标
// dir: 当前朝向, 0=向右, 1=向下
// turnsLeft: 还可以拐弯的剩余次数
void Dfs(int r, int c, int dir, int turnsLeft) {
    if (r == n - 1 && c == n - 1) {
        ++pathCount;
        return;
    }

    // 1. 继续沿当前方向直行
    int nr = r + (dir == 1 ? 1 : 0);
    int nc = c + (dir == 0 ? 1 : 0);
    if (nr < n && nc < n && grid[nr][nc] == '.') {
        Dfs(nr, nc, dir, turnsLeft);
    }

    // 2. 尝试拐弯 (向右变向下, 或向下变向右)
    if (turnsLeft > 0) {
        int nextDir = 1 - dir;
        int tr = r + (nextDir == 1 ? 1 : 0);
        int tc = c + (nextDir == 0 ? 1 : 0);
        if (tr < n && tc < n && grid[tr][tc] == '.') {
            Dfs(tr, tc, nextDir, turnsLeft - 1);
        }
    }
}

void Solve() {
    scanf("%d%d", &n, &k);
    grid.resize(n);
    char buffer[64];
    for (int i = 0; i < n; ++i) {
        scanf("%s", buffer);
        grid[i] = buffer;
    }

    pathCount = 0;

    // 从起点 (0, 0) 出发, 初始可以先向右走 (dir=0), 或先向下走 (dir=1)
    if (1 < n && grid[0][1] == '.') {
        Dfs(0, 1, 0, k);
    }
    if (1 < n && grid[1][0] == '.') {
        Dfs(1, 0, 1, k);
    }

    printf("%d\n", pathCount);
}

int main() {
    int t;
    if (scanf("%d", &t) == 1) {
        while (t--) {
            Solve();
        }
    }
    return 0;
}
