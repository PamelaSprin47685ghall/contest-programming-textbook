#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 100000;

int n, k;
vector<int> neighbors[MAXN]; // 邻接表
// 滚动数组: dp[r % 3][i] = 离牧场 i 不超过 r 条小径的奶牛总数
// 算 r 层时只用到 r-1 与 r-2 两层, 所以 3 个下标轮着用就够了
long long dp[3][MAXN];

int main() {
    scanf("%d %d", &n, &k);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        u--;
        v--;
        neighbors[u].push_back(v);
        neighbors[v].push_back(u);
    }
    for (int i = 0; i < n; i++) {
        scanf("%lld", &dp[0][i]); // 半径 0: 只有本牧场原有的奶牛
    }

    // 半径 1: 本牧场加上所有邻居的半径 0
    for (int i = 0; i < n; i++) {
        long long sum = dp[0][i];
        for (size_t j = 0; j < neighbors[i].size(); j++) {
            sum += dp[0][neighbors[i][j]];
        }
        dp[1][i] = sum;
    }

    // 半径 r: 把每个邻居的半径 r-1 答案加起来. 因为树中任意两牧场间路径唯一,
    // 离 i 不超过 r-2 的奶牛会被 i 的每个邻居算一遍, 一共多算了 deg(i)-1 次, 全部减掉
    for (int r = 2; r <= k; r++) {
        int cur = r % 3, prev = (r - 1) % 3, prev2 = (r - 2) % 3;
        for (int i = 0; i < n; i++) {
            long long sum = 0;
            for (size_t j = 0; j < neighbors[i].size(); j++) {
                sum += dp[prev][neighbors[i][j]];
            }
            sum -= (long long)(neighbors[i].size() - 1) * dp[prev2][i];
            dp[cur][i] = sum;
        }
    }

    int last = k % 3;
    for (int i = 0; i < n; i++) {
        printf("%lld\n", dp[last][i]);
    }
    return 0;
}
