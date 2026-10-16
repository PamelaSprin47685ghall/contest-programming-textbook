#include <cstdio>
#include <vector>

const int FIGURE_SIZE = 10;  // 每个图形占 10x10
const int BOARD = 30;        // 坐标范围 0..30
const int HOME = 20;         // 三块图形的初始位置

// 一个图形: 10x10 的布尔网格, true 表示该格有方块.
struct Figure {
    bool cells[FIGURE_SIZE][FIGURE_SIZE] = {};

    void Read(int cellCount) {
        for (int i = 0; i < cellCount; ++i) {
            int x, y;
            scanf("%d%d", &x, &y);
            cells[x][y] = true;
        }
    }
};

// 判断把三块图形放在 (HOME,HOME), (x1,y1), (x2,y2) 是否互不重叠.
bool OverlapFree(const std::vector<Figure>& figures, int x1, int y1, int x2, int y2) {
    bool used[2 * BOARD + 1][2 * BOARD + 1] = {};
    auto place = [&](int figureIndex, int originX, int originY) {
        for (int i = 0; i < FIGURE_SIZE; ++i) {
            for (int j = 0; j < FIGURE_SIZE; ++j) {
                if (!figures[figureIndex].cells[i][j]) continue;
                bool& cell = used[originX + i][originY + j];
                if (cell) return false;  // 与已放置的图形重叠
                cell = true;
            }
        }
        return true;
    };
    if (!place(0, HOME, HOME)) return false;
    if (!place(1, x1, y1)) return false;
    if (!place(2, x2, y2)) return false;
    return true;
}

// 在 31x31 的范围内移动第二、三块图形 (第一块固定不动),
// 看能否到达目标位置 (0,0) 与 (30,30).
// 状态用 (x1, y1, x2, y2) 表示, 每步把其中一块沿一个方向平移一格.
bool Search(const std::vector<Figure>& figures,
            int x1, int y1, int x2, int y2,
            bool visited[BOARD + 1][BOARD + 1][BOARD + 1][BOARD + 1]) {
    if (x1 == 0 && y1 == 0 && x2 == BOARD && y2 == BOARD) return true;
    auto inRange = [](int v) { return 0 <= v && v <= BOARD; };
    if (!inRange(x1) || !inRange(y1) || !inRange(x2) || !inRange(y2)) return false;

    bool& seen = visited[x1][y1][x2][y2];
    if (seen) return false;
    seen = true;
    if (!OverlapFree(figures, x1, y1, x2, y2)) return false;

    // 第二块和第三块各可沿四个方向移动一格.
    for (int d = -1; d <= 1; d += 2) {
        if (Search(figures, x1 + d, y1, x2, y2, visited)) return true;
        if (Search(figures, x1, y1 + d, x2, y2, visited)) return true;
        if (Search(figures, x1, y1, x2 + d, y2, visited)) return true;
        if (Search(figures, x1, y1, x2, y2 + d, visited)) return true;
        // 两块同时沿同一方向平移一格.
        if (Search(figures, x1 + d, y1, x2 + d, y2, visited)) return true;
        if (Search(figures, x1, y1 + d, x2, y2 + d, visited)) return true;
    }
    return false;
}

int main() {
    int cellCount[3];
    scanf("%d%d%d", &cellCount[0], &cellCount[1], &cellCount[2]);
    std::vector<Figure> figures(3);
    for (int i = 0; i < 3; ++i) figures[i].Read(cellCount[i]);

    static bool visited[BOARD + 1][BOARD + 1][BOARD + 1][BOARD + 1] = {};
    printf("%d\n", Search(figures, HOME, HOME, HOME, HOME, visited) ? 1 : 0);
    return 0;
}