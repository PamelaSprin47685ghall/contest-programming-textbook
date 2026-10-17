#include <cstdio>

using namespace std;

const int MOD = 1000000007;
const int MAXN = 2005;

// 2 在模 1e9+7 下的逆元：折线每拐一次弯就多钉死一个格子，权值乘 1/2
const long long HALF = 500000004;

int n;

// 全局数组自动清零：第 n+1 行、第 n+1 列保持为 0，天然表示"网格外不能放喷头"
char grid[MAXN][MAXN];

// prefRight[j] 是上一行及更上方的 R 之和（R：最后一段水平、结束在该列）
long long prefRight[MAXN];
// curRight[j] 是当前行及更上方的 R 之和
long long curRight[MAXN];

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) scanf("%s", grid[i] + 1);

    int freeCount = 0;  // 没有被牛占据的格子数
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            if (grid[i][j] == '.') ++freeCount;

    // 第一行：折线从 (1,1) 一路向右，还没有拐过弯，每条都有 p^1 的权值
    for (int j = 1; j <= n; ++j) {
        prefRight[j] = (grid[1][j] == '.') ? HALF : 0;
    }

    // 折线从右边界穿出的那部分权值和（第一行的 R 就是 N=1 时的全部答案）
    long long exitRight = prefRight[n];
    long long answer = 0;
    // 从第 2 行推到第 n+1 行（第 n+1 行代表折线从下边界穿出）
    for (int i = 2; i <= n + 1; ++i) {
        // 第 0 列上的虚拟前缀：折线沿第 1 列一路向下到第 i 行，同样没有拐弯
        long long prefDown = (grid[i - 1][1] == '.') ? HALF : 0;  // 竖直前缀和，随 j 向右累加

        for (int j = 1; j <= n; ++j) {
            // 在 (i,j) 结束一段水平线：由上方的竖直段在 (i,j) 处拐弯而来，多一个 p。
            // 拐弯处必须能放下喷头，所以该格必须是空格。
            long long endRight = 0;
            if (grid[i][j] == '.') endRight = prefDown * HALF % MOD;

            // 在 (i,j) 结束一段竖直线：由左侧的水平段拐弯而来，同样多一个 p。
            long long endDown = 0;
            if (grid[i - 1][j + 1] == '.') endDown = prefRight[j] * HALF % MOD;

            prefDown = (prefDown + endDown) % MOD;             // 竖直方向的前缀和
            curRight[j] = (prefRight[j] + endRight) % MOD;     // 水平方向的前缀和
        }

        if (i == n) exitRight = curRight[n];      // 折线从右边界穿出
        if (i == n + 1) answer = (exitRight + prefDown) % MOD;  // 折线从下边界穿出（或穿过右边界）

        for (int j = 1; j <= n; ++j) prefRight[j] = curRight[j];
    }

    // 每个没牛的格子都有"不放喷头 / 放同类喷头"两种选择，把 2^{空格数} 乘回去
    long long choices = 1;
    for (int i = 0; i < freeCount; ++i) choices = choices * 2 % MOD;

    printf("%lld\n", answer * choices % MOD);
    return 0;
}
