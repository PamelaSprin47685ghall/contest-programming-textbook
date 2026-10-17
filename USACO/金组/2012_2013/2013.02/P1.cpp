#include <cstdio>

using namespace std;

const int MAXN = 16;
const int INF = 1000000000;

int n, k;
int cows[MAXN][MAXN];        // cows[r][c]: 第 r 行第 c 列这个牧场里的奶牛数
int columnSum[MAXN];         // columnSum[c]: 当前行区间里第 c 列的奶牛总数
int rowBandMax[MAXN][MAXN];  // rowBandMax[i][j]: 第 i 到 j-1 行里, 单个列组的最大奶牛数
int bestSplit[MAXN][MAXN];   // bestSplit[b][j]: 前 j 行切成 b 条横带时, 最大组的最小可能值

// 竖围栏的位置已经由 mask 固定 (mask 的第 c 位表示第 c 列右边有一条围栏),
// 再额外允许 horizontalLeft 条横围栏, 求最大组的最小可能值
int bestWithVerticalMask(int mask, int horizontalLeft) {
    // 竖围栏固定后, 每一行列组都是固定的一段连续列, 先预处理出每条可能的横带里最胖的组
    for (int i = 0; i < n; i++) {
        for (int c = 0; c < n; c++) columnSum[c] = 0;
        for (int j = i + 1; j <= n; j++) {
            for (int c = 0; c < n; c++) columnSum[c] += cows[j - 1][c];
            int group = 0;    // 当前列组的奶牛数
            int biggest = 0;
            for (int c = 0; c < n; c++) {
                group += columnSum[c];
                // 第 n-1 列的右边是农场边界, 一定是一列的末尾
                if (c == n - 1 || ((mask >> c) & 1) != 0) {
                    if (group > biggest) biggest = group;
                    group = 0;
                }
            }
            rowBandMax[i][j] = biggest;
        }
    }

    int maxBands = horizontalLeft + 1;  // 横围栏把农场切成 围栏数+1 条横带
    if (maxBands > n) maxBands = n;     // 横带再多也超不过行数

    for (int b = 0; b <= maxBands; b++)
        for (int j = 0; j <= n; j++) bestSplit[b][j] = INF;
    bestSplit[0][0] = 0;

    int answer = INF;
    for (int b = 1; b <= maxBands; b++) {
        for (int j = 1; j <= n; j++) {
            for (int i = 0; i < j; i++) {
                if (bestSplit[b - 1][i] == INF) continue;
                // 最后一条横带覆盖第 i 到 j-1 行, 它的最大组要和前面已经切好的部分取较大者
                int candidate = bestSplit[b - 1][i];
                if (rowBandMax[i][j] > candidate) candidate = rowBandMax[i][j];
                if (candidate < bestSplit[b][j]) bestSplit[b][j] = candidate;
            }
        }
        if (bestSplit[b][n] < answer) answer = bestSplit[b][n];
    }
    return answer;
}

int main() {
    scanf("%d %d", &n, &k);
    for (int r = 0; r < n; r++)
        for (int c = 0; c < n; c++)
            scanf("%d", &cows[r][c]);

    int answer = INF;
    // 竖围栏只有 n-1 个可选位置, 直接枚举所有方案;
    // 竖围栏一定下来, 剩下的横围栏就只影响行的切法, 可以动态规划
    for (int mask = 0; mask < (1 << (n - 1)); mask++) {
        int verticalFences = 0;
        for (int c = 0; c < n - 1; c++) verticalFences += (mask >> c) & 1;
        if (verticalFences > k) continue;  // 预算已经花光, 这个方案不合法
        int candidate = bestWithVerticalMask(mask, k - verticalFences);
        if (candidate < answer) answer = candidate;
    }
    printf("%d\n", answer);
    return 0;
}
