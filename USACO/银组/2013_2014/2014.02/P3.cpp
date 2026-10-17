#include <cstdio>
#include <cstring>

using namespace std;

const int MAXN = 105;
const int MOD = 2014;

char s[MAXN];
int n;
// lcp[i][j]: 从 i 和 j 开始的最长公共前缀长度, 用来 O(1) 判断两段等长子串是否相同.
int lcp[MAXN][MAXN];
// ways[i][j]: 子串 s[i..j) 的分解方案数, 含"该子串本身就是初始串(不再做操作)"这一种.
int ways[MAXN][MAXN];

int main() {
    scanf("%s", s);
    n = strlen(s);
    for (int i = n - 1; i >= 0; --i) {
        for (int j = n - 1; j >= 0; --j) {
            if (s[i] == s[j]) {
                lcp[i][j] = lcp[i + 1][j + 1] + 1;
            } else {
                lcp[i][j] = 0;
            }
        }
    }

    // 倒着看: S 的最后一次操作是把某个较长的串 T 变成 S.
    // 设 S 中剩下的那截短串为 X(|X| = k), 则 T 是 S 左右两端中长度为 len-k 的那一段,
    // 且 X 必须恰好等于 T 的一个真前缀或真后缀(对应删掉 T 的末尾或开头),
    // 再接在 T 的前面或后面. 要求 k < |T| = len-k, 所以只需枚举 2k < len.
    for (int len = 0; len <= n; ++len) {
        for (int i = 0; i + len <= n; ++i) {
            int j = i + len;
            int ans = 1;  // 不再做操作: 当前子串自己就是初始串(长度至少 2)
            for (int k = 1; 2 * k < len; ++k) {
                // 情况一: S = T + X, T = s[i..j-k) 是 S 的前缀, X = s[j-k..j).
                if (lcp[i][j - k] >= k) {
                    ans += ways[i + k][j];  // X 是 T 的前缀: 删掉 T 末尾, 再把 T 接在 X 后面
                }
                if (lcp[j - 2 * k][j - k] >= k) {
                    ans += ways[i][j - k];  // X 是 T 的后缀: 删掉 T 开头, 再把 T 接在 X 后面
                }
                // 情况二: S = X + T, T = s[i+k..j) 是 S 的后缀, X = s[i..i+k).
                if (lcp[i][i + k] >= k) {
                    ans += ways[i + k][j];  // X 是 T 的前缀: 删掉 T 末尾, 再把 T 接在 X 前面
                }
                if (lcp[i][j - k] >= k) {
                    ans += ways[i][j - k];  // X 是 T 的后缀: 删掉 T 开头, 再把 T 接在 X 前面
                }
            }
            ways[i][j] = ans % MOD;
        }
    }

    // 减去"零次操作"(直接把原串当作初始串)的那种方案.
    printf("%d\n", (ways[0][n] + MOD - 1) % MOD);
    return 0;
}
