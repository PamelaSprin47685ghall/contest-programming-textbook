#include <cstdio>

using namespace std;

const int MAXRC = 1005;  // 角点网格的行列上限：N+1、M+1 都不超过 1001，留一点余量
const int MAXSTACK = MAXRC * MAXRC;

int rowCount, columnCount, queryCount;
char grid[MAXRC][MAXRC];  // 画布颜色，行、列都从 1 开始；下标 0 那一圈代表"画布外"

// 角点网格上的标记，后面都会原地变成各自的二维前缀和
int verticalEdge[MAXRC][MAXRC];    // 竖边 (row,col)-(row+1,col) 两侧格子颜色不同
int horizontalEdge[MAXRC][MAXRC];  // 横边 (row,col)-(row,col+1) 两侧格子颜色不同
int componentTop[MAXRC][MAXRC];    // 这个角点是所在连通块"最上、其次最左"的角点

// 每个角点所在连通块的代表角点，行号为 0 表示还没访问过
int ownerRow[MAXRC][MAXRC];
int ownerCol[MAXRC][MAXRC];
int deleted[MAXRC][MAXRC];  // 询问过程中的临时标记：这个连通块已经扣掉过了

int stackRow[MAXSTACK];  // 手写栈，避免连通块很长时递归过深
int stackCol[MAXSTACK];

// 画布外的格子当成"另一种颜色"，这样画布自身的边界也算颜色分界线
char cellColor(int row, int col) {
    if (row < 1 || row > rowCount || col < 1 || col > columnCount) return 0;
    return grid[row][col];
}

// 二维前缀和查询：取闭矩形 [row1,row2] × [col1,col2] 内的标记之和
int rangeSum(int table[MAXRC][MAXRC], int row1, int col1, int row2, int col2) {
    if (row1 > row2 || col1 > col2) return 0;
    return table[row2][col2] - table[row1 - 1][col2] - table[row2][col1 - 1] + table[row1 - 1][col1 - 1];
}

// 角点 (row,col) 所在的连通块，如果代表角点落在矩形内部（先前被数过一次），
// 就把它扣掉，并记下标记避免同一个连通块被扣第二次
int dropComponent(int row, int col, int x1, int y1, int x2, int y2) {
    int topRow = ownerRow[row][col];
    int topCol = ownerCol[row][col];
    if (topRow > x1 && topRow <= x2 && topCol > y1 && topCol <= y2 && !deleted[topRow][topCol]) {
        deleted[topRow][topCol] = 1;
        return 1;
    }
    return 0;
}

void restoreComponent(int row, int col) {
    deleted[ownerRow[row][col]][ownerCol[row][col]] = 0;
}

int main() {
    scanf("%d%d%d", &rowCount, &columnCount, &queryCount);
    for (int row = 1; row <= rowCount; ++row) {
        // 第 0 列留给"画布外"，所以颜色串从下标 1 开始存
        scanf("%s", grid[row] + 1);
    }

    // 竖边两侧的格子是 (row,col−1) 和 (row,col)；横边两侧是 (row−1,col) 和 (row,col)
    for (int row = 1; row <= rowCount; ++row) {
        for (int col = 1; col <= columnCount + 1; ++col) {
            if (cellColor(row, col - 1) != cellColor(row, col)) verticalEdge[row][col] = 1;
        }
    }
    for (int row = 1; row <= rowCount + 1; ++row) {
        for (int col = 1; col <= columnCount; ++col) {
            if (cellColor(row - 1, col) != cellColor(row, col)) horizontalEdge[row][col] = 1;
        }
    }

    // 按行、列递增的顺序遍历，每个连通块第一次遇到的角点就是它"最上、其次最左"的角点
    for (int row = 1; row <= rowCount + 1; ++row) {
        for (int col = 1; col <= columnCount + 1; ++col) {
            if (ownerRow[row][col] != 0) continue;
            ownerRow[row][col] = row;
            ownerCol[row][col] = col;
            componentTop[row][col] = 1;
            int top = 0;
            stackRow[top] = row;
            stackCol[top] = col;
            ++top;
            while (top > 0) {
                --top;
                int i = stackRow[top];
                int j = stackCol[top];
                // 入栈时就把代表角点填好，保证每个角点只进栈一次
                if (verticalEdge[i][j] && ownerRow[i + 1][j] == 0) {
                    ownerRow[i + 1][j] = row;
                    ownerCol[i + 1][j] = col;
                    stackRow[top] = i + 1;
                    stackCol[top] = j;
                    ++top;
                }
                if (verticalEdge[i - 1][j] && ownerRow[i - 1][j] == 0) {
                    ownerRow[i - 1][j] = row;
                    ownerCol[i - 1][j] = col;
                    stackRow[top] = i - 1;
                    stackCol[top] = j;
                    ++top;
                }
                if (horizontalEdge[i][j] && ownerRow[i][j + 1] == 0) {
                    ownerRow[i][j + 1] = row;
                    ownerCol[i][j + 1] = col;
                    stackRow[top] = i;
                    stackCol[top] = j + 1;
                    ++top;
                }
                if (horizontalEdge[i][j - 1] && ownerRow[i][j - 1] == 0) {
                    ownerRow[i][j - 1] = row;
                    ownerCol[i][j - 1] = col;
                    stackRow[top] = i;
                    stackCol[top] = j - 1;
                    ++top;
                }
            }
        }
    }

    for (int row = 1; row <= rowCount + 1; ++row) {
        for (int col = 1; col <= columnCount + 1; ++col) {
            verticalEdge[row][col] +=
                verticalEdge[row - 1][col] + verticalEdge[row][col - 1] - verticalEdge[row - 1][col - 1];
            horizontalEdge[row][col] +=
                horizontalEdge[row - 1][col] + horizontalEdge[row][col - 1] - horizontalEdge[row - 1][col - 1];
            componentTop[row][col] +=
                componentTop[row - 1][col] + componentTop[row][col - 1] - componentTop[row - 1][col - 1];
        }
    }

    while (queryCount--) {
        int x1, y1, x2, y2;
        scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
        // 用欧拉公式：把颜色分界线看成平面图的边、角点看成顶点，
        // 则矩形内部的"面数"正好等于所需的笔画数
        int edgeCount = rangeSum(verticalEdge, x1, y1 + 1, x2, y2) +
                        rangeSum(horizontalEdge, x1 + 1, y1, x2, y2);
        int cornerCount = (x2 - x1) * (y2 - y1);
        int componentCount = rangeSum(componentTop, x1 + 1, y1 + 1, x2, y2);
        // 代表角点在内部、但连通块其实碰到了矩形边框的，要扣掉
        for (int row = x1; row <= x2 + 1; ++row) {
            componentCount -= dropComponent(row, y1, x1, y1, x2, y2);
            componentCount -= dropComponent(row, y2 + 1, x1, y1, x2, y2);
        }
        for (int col = y1; col <= y2 + 1; ++col) {
            componentCount -= dropComponent(x1, col, x1, y1, x2, y2);
            componentCount -= dropComponent(x2 + 1, col, x1, y1, x2, y2);
        }
        printf("%d\n", edgeCount - cornerCount + componentCount + 1);
        for (int row = x1; row <= x2 + 1; ++row) {
            restoreComponent(row, y1);
            restoreComponent(row, y2 + 1);
        }
        for (int col = y1; col <= y2 + 1; ++col) {
            restoreComponent(x1, col);
            restoreComponent(x2 + 1, col);
        }
    }
    return 0;
}
