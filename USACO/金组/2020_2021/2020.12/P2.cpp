#include <cstdio>
#include <string>

using namespace std;

const int MAXN = 100000;       // 基因串长度上限
const int MOD = 1000000007;    // 方案数取模
const char BASES[5] = "AGCT";  // 四种碱基，'?' 可以是其中任意一种

char rawGenome[MAXN + 5];      // 先读进字符缓冲
string edited;                 // 编辑后（可能带 '?'）的基因串

// dp[n][l][m]：把编辑串的前 k 个字符切成若干段后，
// 最后一段以第 l 种碱基开头、以第 m 种碱基结尾，倒数第二段以第 n 种碱基开头。
// 相邻两段之间"左段首字母 = 右段末字母"的条件不马上检查：
// 最后一处的条件推迟到答案里检查，更靠前的条件在"再开一段"时结算
long long dp[4][4][4];
long long nextDp[4][4][4];

// 第 pos 个字符能否取第 which 种碱基（'?' 什么都能取）
bool matches(int pos, int which) {
    return edited[pos] == '?' || edited[pos] == BASES[which];
}

int main() {
    scanf("%s", rawGenome);
    edited = rawGenome;
    int len = (int)edited.size();

    // 只切出一段时还没有倒数第二段，用哪一行当占位都行，四行都填上
    for (int n = 0; n < 4; ++n) {
        for (int l = 0; l < 4; ++l) {
            if (matches(0, l)) dp[n][l][l] = 1;
        }
    }

    for (int k = 1; k < len; ++k) {
        for (int n = 0; n < 4; ++n) {
            for (int l = 0; l < 4; ++l) {
                for (int m = 0; m < 4; ++m) nextDp[n][l][m] = 0;
            }
        }
        for (int m2 = 0; m2 < 4; ++m2) {
            if (!matches(k, m2)) continue;
            for (int n = 0; n < 4; ++n) {
                for (int l = 0; l < 4; ++l) {
                    for (int m = 0; m < 4; ++m) {
                        long long ways = dp[n][l][m];
                        if (ways == 0) continue;
                        // 第一种转移：把最后一段延长一格。
                        // 段内不能有相邻相同的字母，否则约翰当初会在这里切开
                        if (m != m2) {
                            nextDp[n][l][m2] = (nextDp[n][l][m2] + ways) % MOD;
                        }
                        // 第二种转移：在第 k 个字符处另起一段，新段只有一个字符。
                        // 这时原来的"最后一段"变成倒数第二段，它和它前面一段之间
                        // 的条件必须结算：前一段的首字母 n 要等于原来最后一段的末字母 m
                        if (n == m) {
                            nextDp[l][m2][m2] = (nextDp[l][m2][m2] + ways) % MOD;
                        }
                    }
                }
            }
        }
        for (int n = 0; n < 4; ++n) {
            for (int l = 0; l < 4; ++l) {
                for (int m = 0; m < 4; ++m) dp[n][l][m] = nextDp[n][l][m];
            }
        }
    }

    // 收尾：最后剩下的那处相邻条件也要满足，即倒数第二段首字母等于最后一段末字母
    long long answer = 0;
    for (int l = 0; l < 4; ++l) {
        for (int m = 0; m < 4; ++m) {
            answer = (answer + dp[m][l][m]) % MOD;
        }
    }
    printf("%lld\n", answer);
    return 0;
}
