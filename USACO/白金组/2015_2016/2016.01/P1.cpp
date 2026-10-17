#include <cstdio>

using namespace std;

const int MAXN = 200; // 网格边长上限

int n, m;
char grid[MAXN][MAXN + 1];          // 每行草地，按字符串读入（末尾有 '\0'）
int swampInRow[MAXN + 1][MAXN + 1]; // swampInRow[i][j]：第 i 行前 j 格里的沼泽数

int main() {
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n; ++i) {
        scanf("%s", grid[i]);
    }

    // 逐行做前缀和，就能 O(1) 判断"一行在 [left, right] 这段里有没有沼泽"
    for (int i = 0; i < n; ++i) {
        swampInRow[i][0] = 0;
        for (int j = 0; j < m; ++j) {
            swampInRow[i][j + 1] = swampInRow[i][j] + (grid[i][j] == 'X' ? 1 : 0);
        }
    }

    int best = 0;
    // 枚举矩形左右两条竖边所在的列
    for (int left = 0; left < m; ++left) {
        for (int right = left; right < m; ++right) {
            // top 是"最早还能当顶边"的行号：从 top 行到当前行之间，
            // 左右两条竖边经过的格子都是草地（否则 top 会被重置成 -1）
            int top = -1;
            for (int row = 0; row < n; ++row) {
                // 顶边、底边同样要避开沼泽，所以这一行在 [left, right] 上必须全为草地
                bool edgeOk = (swampInRow[row][right + 1] - swampInRow[row][left] == 0);
                if (edgeOk) {
                    int width = right - left + 1;
                    // 高度为 1 的矩形就是这一行本身
                    if (width > best) best = width;
                    if (top >= 0) {
                        int area = (row - top + 1) * width;
                        if (area > best) best = area;
                    }
                }
                // 竖边一旦在某一行踩到沼泽，竖直方向就被截断了
                if (grid[row][left] == 'X' || grid[row][right] == 'X') top = -1;
                // 上面没截断且这一行能当边，就可以从这里重新开始向下累积高度
                if (edgeOk && top == -1) top = row;
            }
        }
    }

    printf("%d\n", best);
    return 0;
}
