#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

char Buffer[1001];

// 方向编码: 0=右 1=上 2=左 3=下
const int DX[4]{0, -1, 0, 1};
const int DY[4]{1, 0, -1, 0};

// reflect[mirror][direction]: '/' 和 '\' 各自把入射方向映射到反射方向.
const int REFLECT[2][4] = {
    {1, 0, 3, 2}, // '/': 右->上 上->右 左->下 下->左
    {3, 2, 1, 0}  // '\': 右->下 下->右 上->左 左->上
};

struct Board {
    int n, m;
    vector<string> grid;

    void Read() {
        scanf("%d%d", &n, &m);
        grid.resize(n);
        for (string& row : grid) {
            scanf("%s", Buffer);
            row = Buffer;
        }
    }

    int Solve() const {
        int answer = 0;
        // 从四条边的每个格子射入光束, 统计能照亮的格子数, 取最大.
        for (int i = 0; i < n; ++i) {
            answer = max(answer, Visit(0, i, 0));     // 左边界向右射
            answer = max(answer, Visit(2, i, m - 1)); // 右边界向左射
        }
        for (int i = 0; i < m; ++i) {
            answer = max(answer, Visit(1, n - 1, i)); // 下边界向上射
            answer = max(answer, Visit(3, 0, i));     // 上边界向下射
        }
        return answer;
    }

    // 从 (x, y) 以 direction 方向射入, 返回离开网格前照亮的格子数.
    int Visit(int direction, int x, int y) const {
        int count = 0;
        while (0 <= x && x < n && 0 <= y && y < m) {
            ++count;
            if (grid[x][y] == '/' || grid[x][y] == '\\') {
                int mirror = grid[x][y] == '/' ? 0 : 1;
                direction = REFLECT[mirror][direction];
            }
            x += DX[direction];
            y += DY[direction];
        }
        return count;
    }
};

int main() {
    Board board;
    board.Read();
    printf("%d\n", board.Solve());
    return 0;
}