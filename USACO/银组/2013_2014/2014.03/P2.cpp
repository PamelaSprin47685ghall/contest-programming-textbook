#include <cstdio>

using namespace std;

const int MAXN = 400;
// 旋转 45 度后坐标 u = r+c, v = c-r+(N-1) 的范围都是 0..2N-2, 存成 1..2N-1.
const int MAXD = 2 * MAXN + 2;

int N, K;
// sum[i][j]: 旋转坐标系下前 i 行前 j 列的前缀和, 下标从 1 开始
int sum[MAXD][MAXD];

// 查询旋转坐标系下矩形 [i1..i2] x [j1..j2] 的元素和(超界的部分自动忽略)
int RectSum(int i1, int i2, int j1, int j2) {
    if (i1 < 1) {
        i1 = 1;
    }
    if (j1 < 1) {
        j1 = 1;
    }
    if (i2 > 2 * N - 1) {
        i2 = 2 * N - 1;
    }
    if (j2 > 2 * N - 1) {
        j2 = 2 * N - 1;
    }
    if (i1 > i2 || j1 > j2) {
        return 0;
    }
    return sum[i2][j2] - sum[i1 - 1][j2] - sum[i2][j1 - 1] + sum[i1 - 1][j1 - 1];
}

int main() {
    scanf("%d %d", &N, &K);

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            int grass;
            scanf("%d", &grass);
            // 曼哈顿距离在 (u,v) = (r+c, c-r) 下变成切比雪夫距离,
            // 菱形可达区域随之变成轴对齐正方形, 便于用前缀和直接求和.
            sum[r + c + 1][c - r + N] += grass;
        }
    }

    for (int i = 1; i <= 2 * N - 1; i++) {
        for (int j = 1; j <= 2 * N - 1; j++) {
            sum[i][j] += sum[i - 1][j] + sum[i][j - 1] - sum[i - 1][j - 1];
        }
    }

    int best = 0;
    // 只把贝茜放在真正的格点上(旋转坐标系里这些点恰好对应原网格)
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            int u = r + c + 1;
            int v = c - r + N;
            // 曼哈顿半径 K 的菱形 = 旋转坐标系里边长 2K+1 的正方形
            int got = RectSum(u - K, u + K, v - K, v + K);
            if (got > best) {
                best = got;
            }
        }
    }

    printf("%d\n", best);
    return 0;
}
