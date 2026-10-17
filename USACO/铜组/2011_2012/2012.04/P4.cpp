#include <cstdio>
#include <vector>

using namespace std;

// 三个图形各占 10x10 的网格, true 表示该格有方块.
struct Figure {
    bool cells[10][10] = {};

    void Read(int cellCount) {
        for (int i = 0; i < cellCount; ++i) {
            int x, y;
            scanf("%d%d", &x, &y);
            cells[x][y] = true;
        }
    }
};

// 判断图形 a 与图形 b 在相对偏移 (dx,dy) 下是否互不重叠.
bool PairSeparate(const Figure& a, const Figure& b, int dx, int dy) {
    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
            if (!a.cells[x][y]) continue;
            int bx = x + dx;
            int by = y + dy;
            if (bx < 0 || 10 <= bx || by < 0 || 10 <= by) continue;
            if (b.cells[bx][by]) return false;  // 两块图形重叠
        }
    }
    return true;
}

// 预计算每对图形在 [-20,20] 偏移下是否互不重叠, 便于 BFS 中 O(1) 判断.
struct OverlapTable {
    bool separate[3][3][41][41] = {};
};

int main() {
    int cellCount[3];
    scanf("%d%d%d", &cellCount[0], &cellCount[1], &cellCount[2]);
    Figure f[3];
    for (int i = 0; i < 3; ++i) f[i].Read(cellCount[i]);

    // 预计算所有偏移下的两两关系.
    OverlapTable table;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int dx = -20; dx <= 20; ++dx) {
                for (int dy = -20; dy <= 20; ++dy) {
                    table.separate[i][j][dx + 20][dy + 20] =
                        PairSeparate(f[i], f[j], dx, dy);
                }
            }
        }
    }
    auto pairOk = [&](int i, int j, int dx, int dy) {
        if (dx < -20 || 20 < dx || dy < -20 || 20 < dy) return true;
        return table.separate[i][j][dx + 20][dy + 20];
    };
    // 三块图形两两都不重叠.
    auto allOk = [&](int dx1, int dy1, int dx2, int dy2) {
        return pairOk(0, 1, dx1, dy1) &&
               pairOk(0, 2, dx2, dy2) &&
               pairOk(1, 2, dx2 - dx1, dy2 - dy1);
    };

    // BFS 搜索偏移状态 (dx1,dy1,dx2,dy2), 范围限制在 [-20,20].
    const int LIM = 20;
    const int W = 2 * LIM + 1;  // 41
    vector<char> seen(W * W * W * W, 0);
    vector<int> qx1, qy1, qx2, qy2;
    auto encode = [&](int a, int b, int c, int d) {
        return ((a + LIM) * W + (b + LIM)) * W * W +
               (c + LIM) * W + (d + LIM);
    };

    auto tryAdd = [&](int dx1, int dy1, int dx2, int dy2) {
        if (dx1 < -LIM || LIM < dx1 || dy1 < -LIM || LIM < dy1 ||
            dx2 < -LIM || LIM < dx2 || dy2 < -LIM || LIM < dy2) return;
        if (!allOk(dx1, dy1, dx2, dy2)) return;
        int key = encode(dx1, dy1, dx2, dy2);
        if (seen[key]) return;
        seen[key] = 1;
        qx1.push_back(dx1);
        qy1.push_back(dy1);
        qx2.push_back(dx2);
        qy2.push_back(dy2);
    };

    tryAdd(0, 0, 0, 0);
    const int DIR[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    for (int head = 0; head < (int)qx1.size(); ++head) {
        int dx1 = qx1[head], dy1 = qy1[head];
        int dx2 = qx2[head], dy2 = qy2[head];
        // 图形 2 被推到右上角、图形 3 被推到左下角, 三者必然分开.
        if (dx1 == LIM && dy1 == LIM && dx2 == -LIM && dy2 == -LIM) {
            printf("1\n");
            return 0;
        }
        for (int d = 0; d < 4; ++d) {
            int mx = DIR[d][0], my = DIR[d][1];
            tryAdd(dx1 + mx, dy1 + my, dx2, dy2);        // 移动图形 2
            tryAdd(dx1, dy1, dx2 + mx, dy2 + my);        // 移动图形 3
            tryAdd(dx1 + mx, dy1 + my, dx2 + mx, dy2 + my);  // 移动图形 1
        }
    }
    printf("0\n");
    return 0;
}
