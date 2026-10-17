#include <cstdio>
#include <cstring>

using namespace std;

const int MAXN = 1005;
const int MOD = 2012;

char s[MAXN];
// balance[i]: 前 i 个字符的整体深度, 即左括号数减右括号数
int balance[MAXN];
// dp[i][a]: 前 i 个字符中分给 Holstein 的"还没配对的左括号"数为 a 的方案数.
// 分给 Guernsey 的未配对左括号数 = balance[i] - a, 所以不必再开一维
int dp[MAXN][MAXN];

int main() {
    scanf("%s", s);
    int n = strlen(s);

    balance[0] = 0;
    for (int i = 0; i < n; ++i) {
        balance[i + 1] = balance[i] + (s[i] == '(' ? 1 : -1);
    }

    dp[0][0] = 1;
    for (int i = 0; i < n; ++i) {
        for (int a = 0; a <= i; ++a) {
            if (dp[i][a] == 0) {
                continue;
            }
            int b = balance[i] - a;  // Guernsey 那边还没配对的左括号数
            if (s[i] == '(') {
                // 左括号分给哪个品种都会让它多出一个未配对左括号
                dp[i + 1][a + 1] = (dp[i + 1][a + 1] + dp[i][a]) % MOD;
                dp[i + 1][a] = (dp[i + 1][a] + dp[i][a]) % MOD;
            } else {
                // 右括号只能分给"还欠着左括号"的那一边, 否则那个品种的前缀就变负了
                if (a > 0) {
                    dp[i + 1][a - 1] = (dp[i + 1][a - 1] + dp[i][a]) % MOD;
                }
                if (b > 0) {
                    dp[i + 1][a] = (dp[i + 1][a] + dp[i][a]) % MOD;
                }
            }
        }
    }

    // 两个品种都恰好没有剩余未配对的左括号时方案才合法;
    // 整体深度为 0 时, a = 0 就同时保证了 Guernsey 那边也是 0
    if (balance[n] != 0) {
        printf("0\n");
    } else {
        printf("%d\n", dp[n][0]);
    }
    return 0;
}
