#include <cstdio>

using namespace std;

const int MAXN = 10;

int N;
int canvas[MAXN][MAXN];

// 该颜色在最终画布上是否可见.
bool Appears(int color) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (canvas[i][j] == color) return true;
        }
    }
    return false;
}

// 颜色 color 的格子是否出现在颜色 other 的矩形范围内.
// 矩形取 other 所有可见格子的最小包围盒: 假设矩形尽量小,
// 可以让重叠最少, 从而不错杀任何"可能最先画"的颜色.
bool InsideRectangle(int color, int other) {
    int top = N, bottom = -1, left = N, right = -1;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (canvas[i][j] == other) {
                if (i < top) top = i;
                if (i > bottom) bottom = i;
                if (j < left) left = j;
                if (j > right) right = j;
            }
        }
    }
    for (int i = top; i <= bottom; ++i) {
        for (int j = left; j <= right; ++j) {
            if (canvas[i][j] == color) return true;
        }
    }
    return false;
}

int main() {
    scanf("%d", &N);
    for (int i = 0; i < N; ++i) {
        char row[MAXN + 2];
        scanf("%s", row);
        for (int j = 0; j < N; ++j) {
            canvas[i][j] = row[j] - '0';
        }
    }

    int answer = 0;
    for (int color = 1; color <= 9; ++color) {
        if (!Appears(color)) continue;
        // 只要有另一种颜色的矩形盖住了 color 的某个可见格子,
        // 那个颜色就一定画在 color 之后, 于是 color 不可能最先画.
        bool couldBeFirst = true;
        for (int other = 1; other <= 9; ++other) {
            if (other != color && Appears(other) &&
                InsideRectangle(color, other)) {
                couldBeFirst = false;
            }
        }
        if (couldBeFirst) ++answer;
    }

    printf("%d\n", answer);
    return 0;
}
