#include <cstdio>
#include <string>
#include <vector>

using namespace std;

struct Point {
    int x;
    int y;
};

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    vector<string> board(n);
    char buffer[80];
    for (int i = 0; i < n; ++i) {
        scanf("%s", buffer);
        board[i] = buffer;
    }
    // 越界一律视为黑格，这样边界上的线索也能被正确识别。
    auto cell = [&](int x, int y) -> char {
        if (x < 0 || n <= x) return '#';
        if (y < 0 || m <= y) return '#';
        return board[x][y];
    };
    // 横向线索起点：左边是黑格(或越界)，且向右连续三格都是白格(词长至少 3)。
    auto horizontalStart = [&](int x, int y) {
        return cell(x - 1, y) == '#' && cell(x, y) == '.' &&
               cell(x + 1, y) == '.' && cell(x + 2, y) == '.';
    };
    // 纵向线索起点：上边是黑格(或越界)，且向下连续三格都是白格。
    auto verticalStart = [&](int x, int y) {
        return cell(x, y - 1) == '#' && cell(x, y) == '.' &&
               cell(x, y + 1) == '.' && cell(x, y + 2) == '.';
    };
    auto isStart = [&](int x, int y) {
        return horizontalStart(x, y) || verticalStart(x, y);
    };
    vector<Point> answer;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (isStart(i, j)) answer.push_back(Point{i, j});
        }
    }
    printf("%d\n", int(answer.size()));
    for (const Point& p : answer) {
        printf("%d %d\n", p.x + 1, p.y + 1);
    }
    return 0;
}
