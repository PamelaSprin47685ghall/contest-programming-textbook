#include <cstdio>
#include <cstring>

using namespace std;

const long long MOD = 1000000007;
const int MAXN = 305;
const int MAXI = 25;

int N, Q;
int digitVal[MAXN];
long long waysRight[MAXN][MAXN];   // 每个区间中拼出 <= B 的方案数
long long waysLeft[MAXN][MAXN];    // 每个区间中拼出 <= A - 1 的方案数
long long dp[MAXI][MAXI][3];       // dp[x][y][c]：当前纸片拼出的数有 y - x + 1 位，
                                   // 与 limit[x..y] 相比的关系为 c（0 小于，1 等于，2 大于）

// 0 表示小于，1 表示等于，2 表示大于
int signum(int a, int b) {
    if (a < b) return 0;
    if (a == b) return 1;
    return 2;
}

// 统计所有区间里能拼出不超过 limit 的整数的方案数（不含一张纸都不选的空堆）
void solve(long long limit, long long out[MAXN][MAXN]) {
    char buf[MAXI];
    int d = sprintf(buf, "%lld", limit);
    int lim[MAXI];
    for (int i = 0; i < d; i++) lim[i] = buf[i] - '0';

    for (int j = 0; j < N; j++) {
        memset(dp, 0, sizeof(dp));
        for (int k = j; k < N; k++) {
            int dk = digitVal[k];

            // 当前这一段纸片拼出的数始终是最终数字串的一段连续子串：
            // 放到堆顶就是在左端插入一位，放到堆底就是在右端插入一位
            for (int x = 0; x < d; x++) {
                for (int y = d - 1; y > x; y--) {
                    // 放堆顶：新的一位落在 limit[x] 处，它一比出来就决定了大小关系
                    if (dk > lim[x]) {
                        dp[x][y][2] += dp[x + 1][y][0] + dp[x + 1][y][1] + dp[x + 1][y][2];
                    } else if (dk == lim[x]) {
                        for (int c = 0; c < 3; c++) dp[x][y][c] += dp[x + 1][y][c];
                    } else {
                        dp[x][y][0] += dp[x + 1][y][0] + dp[x + 1][y][1] + dp[x + 1][y][2];
                    }
                    // 放堆底：新的一位落在 limit[y] 处，只有内部已经相等时它才能决定大小关系
                    dp[x][y][2] += dp[x][y - 1][2];
                    dp[x][y][signum(dk, lim[y])] += dp[x][y - 1][1];
                    dp[x][y][0] += dp[x][y - 1][0];
                    for (int c = 0; c < 3; c++) dp[x][y][c] %= MOD;
                }
            }
            // 只用这一张纸片，放堆顶与放堆底是两种不同的方案
            for (int x = 0; x < d; x++) dp[x][x][signum(dk, lim[x])] += 2;

            // 把这一段对齐到 limit 的右端：位数比 limit 少时必然更小，随便什么关系都算；
            // 位数恰好等于 limit 时（x == 0）才要求关系不是「大于」
            long long total = 0;
            for (int x = 0; x < d; x++) {
                total += dp[x][d - 1][0] + dp[x][d - 1][1];
                if (x != 0) total += dp[x][d - 1][2];
                total %= MOD;
            }
            out[j][k] = total;
        }
    }
}

int main() {
    long long A, B;
    scanf("%d %lld %lld", &N, &A, &B);

    // 纸片上的数字可能带空格分隔，也可能连成一片，逐字符读入
    for (int i = 0; i < N; i++) {
        int c;
        do {
            c = getchar();
        } while (c == ' ' || c == '\n' || c == '\r' || c == '\t');
        digitVal[i] = c - '0';
    }

    solve(B, waysRight);
    solve(A - 1, waysLeft);

    scanf("%d", &Q);
    for (int q = 0; q < Q; q++) {
        int l, r;
        scanf("%d %d", &l, &r);
        // 空堆得到 0，而 A >= 1，所以 0 在两次统计里都被算进又被减掉，直接相减即可
        long long ans = (waysRight[l - 1][r - 1] - waysLeft[l - 1][r - 1] + MOD) % MOD;
        printf("%lld\n", ans);
    }
    return 0;
}
