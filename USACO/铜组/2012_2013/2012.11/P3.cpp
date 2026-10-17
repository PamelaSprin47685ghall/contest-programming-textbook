#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

const int DX[4] = {1, 0, -1, 0};
const int DY[4] = {0, 1, 0, -1};

char buffer[80];

// 棋盘: 越界访问统一返回 outside, 便于 DFS 把边界当障碍.
struct Board {
    int n;
    vector<string> data;
    char outside = 'X';

    void Read() {
        scanf("%d", &n);
        data.resize(n);
        for (string& row : data) {
            scanf("%s", buffer);
            row = buffer;
        }
    }

    char& At(int x, int y) {
        if (x < 0 || n <= x) return outside;
        if (y < 0 || n <= y) return outside;
        return data[x][y];
    }
};

// 搜索能走出的最长的合法马蹄铁序列 (先若干 '(' 后相同数量 ')').
struct DFS {
    Board* board;
    int answer = 0;

    void Compute(int x, int y, int sizeLeft, int sizeRight) {
        char cell = board->At(x, y);
        // 已访问, 或出现右括号后又遇到左括号则非法.
        if (cell == 'X' || (sizeRight != 0 && cell == '(')) return;
        board->At(x, y) = 'X';
        if (cell == '(') ++sizeLeft;
        else if (cell == ')') ++sizeRight;
        if (sizeLeft == sizeRight) answer = max(answer, sizeLeft + sizeRight);

        for (int i = 0; i < 4; ++i) {
            Compute(x + DX[i], y + DY[i], sizeLeft, sizeRight);
        }
        // 回溯, 还原格子以便其他路径使用.
        board->At(x, y) = cell;
    }
};

int main() {
    Board board;
    board.Read();
    DFS dfs;
    dfs.board = &board;
    dfs.Compute(0, 0, 0, 0);
    printf("%d\n", dfs.answer);
    return 0;
}
