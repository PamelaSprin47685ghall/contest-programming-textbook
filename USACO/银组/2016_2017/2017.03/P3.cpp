#include <cstdio>

using namespace std;

const int MAXN = 20;
const int MAXRECT = 44100;      // 矩形总数上界: 左右端点 C(21,2) 种选法, 再乘上下端点 C(21,2)

struct Rectangle {
    int top, left, bottom, right;
};

int n;
char image[MAXN][MAXN + 1];
int visited[MAXN][MAXN];        // 当前矩形内每个格子所属的连通块编号, 0 表示还没访问
Rectangle validList[MAXRECT];   // 先存下所有满足"两色 + 一色一块、另一色多块"的矩形
int validCount = 0;

// 在给定矩形内, 从 (row, col) 出发把同色连通块全部染上编号 id.
void floodFill(int row, int col, int top, int left, int bottom, int right, int id) {
    visited[row][col] = id;
    char color = image[row][col];
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};
    for (int d = 0; d < 4; d++) {
        int nr = row + dr[d];
        int nc = col + dc[d];
        if (nr < top || nr > bottom || nc < left || nc > right) {
            continue;
        }
        if (visited[nr][nc] != 0) {
            continue;
        }
        if (image[nr][nc] != color) {
            continue;
        }
        floodFill(nr, nc, top, left, bottom, right, id);
    }
}

bool isPcl(int top, int left, int bottom, int right) {
    for (int row = top; row <= bottom; row++) {
        for (int col = left; col <= right; col++) {
            visited[row][col] = 0;
        }
    }

    int componentCount[26] = {0};   // 每种颜色在矩形内形成了几个连通块
    int colorCount = 0;
    int id = 0;
    for (int row = top; row <= bottom; row++) {
        for (int col = left; col <= right; col++) {
            if (visited[row][col] != 0) {
                continue;
            }
            int color = image[row][col] - 'A';
            if (componentCount[color] == 0) {
                colorCount++;       // 第一次遇到这种颜色
            }
            componentCount[color]++;
            id++;
            floodFill(row, col, top, left, bottom, right, id);
        }
    }

    // 必须恰好两种颜色, 且一种颜色只有一块、另一种颜色至少两块.
    if (colorCount != 2) {
        return false;
    }
    bool hasSingle = false, hasMultiple = false;
    for (int color = 0; color < 26; color++) {
        if (componentCount[color] == 1) {
            hasSingle = true;
        }
        if (componentCount[color] >= 2) {
            hasMultiple = true;
        }
    }
    return hasSingle && hasMultiple;
}

// a 是否包含在 b 内部?
bool isInside(const Rectangle &a, const Rectangle &b) {
    return a.top >= b.top && a.bottom <= b.bottom && a.left >= b.left && a.right <= b.right;
}

int main() {
    scanf("%d", &n);
    for (int row = 0; row < n; row++) {
        scanf("%s", image[row]);
    }

    for (int top = 0; top < n; top++) {
        for (int bottom = top; bottom < n; bottom++) {
            for (int left = 0; left < n; left++) {
                for (int right = left; right < n; right++) {
                    if (isPcl(top, left, bottom, right)) {
                        validList[validCount].top = top;
                        validList[validCount].left = left;
                        validList[validCount].bottom = bottom;
                        validList[validCount].right = right;
                        validCount++;
                    }
                }
            }
        }
    }

    // PCL 是"不被任何其他 PCL 包含"的矩形. 直接在所有合法矩形里两两比较:
    // 一个合法矩形只要被另一个合法矩形包住, 它就不是 PCL.
    int answer = 0;
    for (int i = 0; i < validCount; i++) {
        bool maximal = true;
        for (int j = 0; j < validCount; j++) {
            if (i != j && isInside(validList[i], validList[j])) {
                maximal = false;
                break;
            }
        }
        if (maximal) {
            answer++;
        }
    }
    printf("%d\n", answer);
    return 0;
}
