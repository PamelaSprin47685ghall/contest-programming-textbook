#include <cstdio>

using namespace std;

const int MAXN = 1001;      // 坐标最多到 1000, 差分数组每维开 1001 才容得下右边界上的修正
const int LIMIT = 1000;     // 只有左下角为 (i, j) 的 1x1 小格算面积, 即 i, j 都在 0..999

int diff[MAXN][MAXN];       // 二维差分: 每格记"涂色次数在此处的变化量"

int main() {
    int rectangleCount, targetCoats;
    scanf("%d%d", &rectangleCount, &targetCoats);
    for (int i = 0; i < rectangleCount; i++) {
        int x1, y1, x2, y2;
        scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
        // 一次矩形涂色, 等价于在差分数组的四个角做修改
        diff[x1][y1]++;
        diff[x1][y2]--;
        diff[x2][y1]--;
        diff[x2][y2]++;
    }

    int answer = 0;
    for (int i = 0; i < LIMIT; i++) {
        for (int j = 0; j < LIMIT; j++) {
            // 二维前缀和: 把差分累加成 (i, j) 这一格真正被涂的次数.
            // 只算到 999, 因为坐标 1000 上的差分修正永远不会影响这些小格.
            if (i > 0) diff[i][j] += diff[i - 1][j];
            if (j > 0) diff[i][j] += diff[i][j - 1];
            if (i > 0 && j > 0) diff[i][j] -= diff[i - 1][j - 1];
            if (diff[i][j] == targetCoats) answer++;
        }
    }
    printf("%d\n", answer);
    return 0;
}
