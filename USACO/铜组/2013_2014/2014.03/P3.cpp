#include <algorithm>
#include <cstdio>
#include <functional>
#include <string>
#include <vector>

using namespace std;

char Buffer[101];

const int DX[4]{1, 0, -1, 0};
const int DY[4]{0, 1, 0, -1};

struct Board {
    vector<string> grid;

    void Read() {
        int n;
        scanf("%d", &n);
        grid.resize(n);
        for (string& row : grid) {
            scanf("%s", Buffer);
            row = Buffer;
        }
    }
};

// 统计连通分量个数. sameColor 决定两个格子是否算同一颜色,
// 用于区分"原色"与"把 G 看成 R"两种视角.
int CountRegions(const Board& board,
                 const function<bool(char, char)>& sameColor) {
    int size = board.grid.size();
    vector<vector<bool>> visited(size,
                                 vector<bool>(size, false));
    int answer = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (visited[i][j]) continue;
            ++answer;
            // 深度优先扩展当前连通分量.
            vector<int> stack;
            stack.push_back(i);
            stack.push_back(j);
            visited[i][j] = true;
            while (!stack.empty()) {
                int y = stack.back();
                stack.pop_back();
                int x = stack.back();
                stack.pop_back();
                for (int d = 0; d < 4; ++d) {
                    int nx = x + DX[d], ny = y + DY[d];
                    if (nx < 0 || nx >= size || ny < 0 || ny >= size) continue;
                    if (visited[nx][ny]) continue;
                    if (!sameColor(board.grid[x][y], board.grid[nx][ny])) {
                        continue;
                    }
                    visited[nx][ny] = true;
                    stack.push_back(nx);
                    stack.push_back(ny);
                }
            }
        }
    }
    return answer;
}

// 原色视角: 颜色必须完全相同才连通.
bool SameExact(char a, char b) { return a == b; }

// 色盲视角: G 和 R 视为同色.
bool SameColorblind(char a, char b) {
    if (a == 'G') a = 'R';
    if (b == 'G') b = 'R';
    return a == b;
}

int main() {
    Board board;
    board.Read();
    int normal = CountRegions(board, SameExact);
    int colorblind = CountRegions(board, SameColorblind);
    printf("%d %d\n", normal, colorblind);
    return 0;
}