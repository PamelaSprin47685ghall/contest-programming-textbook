#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 1005;
const long long INF = 1000000000000000000LL;

int n;                          // 逃出围栏的奶牛数
int pos[MAXN];                  // 排序后的所有位置, 其中包含 FJ 的出发点 0
long long best[MAXN][MAXN][2];  // best[i][j][0/1]: 已经安抚了排序后第 i 到第 j 个位置上的奶牛,
                                // FJ 此刻站在区间左端(0)或右端(1)时的最小累计损失

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &pos[i]);
    pos[n] = 0;                     // 位置 0 上没有奶牛, 只是起点, 放进序列方便定位区间
    sort(pos, pos + n + 1);

    int start = 0;                  // 出发点的下标
    while (pos[start] != 0) start++;  // 各位置互不相同, 一定找得到

    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++)
            best[i][j][0] = best[i][j][1] = INF;
    best[start][start][0] = best[start][start][1] = 0;

    // 已经安抚的奶牛在排序后一定是连续一段 (否则必然会经过中间那些奶牛),
    // 所以只需要对区间做 DP; i 从大到小、j 从小到大枚举, 保证转移来源已经算好
    for (int i = start; i >= 0; i--) {
        for (int j = start; j <= n; j++) {
            long long remaining = n - (j - i);   // 区间 [i,j] 里安抚了 j-i 头, 其余还在破坏
            if (remaining == 0) continue;        // 全部安抚完毕, 不会再有新的损失

            // FJ 在左端 pos[i] 时向左走到 pos[i-1], 或在右端 pos[j] 时向左走到 pos[i-1]
            if (i > 0) {
                long long step = remaining * (pos[i] - pos[i - 1]);
                if (best[i][j][0] + step < best[i - 1][j][0]) best[i - 1][j][0] = best[i][j][0] + step;
                step = remaining * (pos[j] - pos[i - 1]);
                if (best[i][j][1] + step < best[i - 1][j][0]) best[i - 1][j][0] = best[i][j][1] + step;
            }
            // 同理向右扩展
            if (j < n) {
                long long step = remaining * (pos[j + 1] - pos[i]);
                if (best[i][j][0] + step < best[i][j + 1][1]) best[i][j + 1][1] = best[i][j][0] + step;
                step = remaining * (pos[j + 1] - pos[j]);
                if (best[i][j][1] + step < best[i][j + 1][1]) best[i][j + 1][1] = best[i][j][1] + step;
            }
        }
    }

    long long answer = best[0][n][0];
    if (best[0][n][1] < answer) answer = best[0][n][1];
    printf("%lld\n", answer);
    return 0;
}
