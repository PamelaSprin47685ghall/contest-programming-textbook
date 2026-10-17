#include <cstdio>

using namespace std;

const int MAXN = 2000;
const int MOD = 1000000007;

int n;
char rawProgram[2][MAXN + 5];   // 原始的 Bessie / Elsie 程序
char program[2][MAXN + 5];      // 约简之后的程序，乘法统一记成 '2'
int progLen[2];

// 约简规则：
//   1) ×1 是恒等变换，直接删掉；
//   2) 遇到 ×0 时，它之前的结果全部被清零，所以把前面已读的指令全部丢弃；
//   3) 剩下的乘法只有"乘一个数"这一种语义角色，统一记成 '2'。
// 之所以把 ×0 本身保留成一条乘法（而不是连同前缀一起删掉），是因为它清零的
// 是"交错之后排在它前面的所有指令"，位置本身仍有意义。
void reduce(int idx) {
    int len = 0;
    for (int i = 0; i < n; ++i) {
        char c = rawProgram[idx][i];
        if (c == '1') {
            continue;
        }
        if (c == '0') {
            len = 0;
        }
        if (c != '+') {
            c = '2';
        }
        program[idx][len++] = c;
    }
    progLen[idx] = len;
}

// dp[i][j][k]：用掉 Bessie 的前 i 条、Elsie 的前 j 条指令，k=0 表示最后一条
// 来自 Bessie，k=1 表示来自 Elsie 的规范化交错方案数。
// 规范化：不允许出现"Elsie 的指令紧挨在 Bessie 同类型指令前面"。
// 任何含这种相邻对的方案都可以不断交换这两条指令，唯一地化成不含它的方案，
// 所以合法表达式的个数恰好等于规范化交错方案数。
// 下面按 i 滚动，pre 表示 dp[i-1][*]，cur 表示 dp[i][*]。
long long pre0[MAXN + 5], pre1[MAXN + 5];
long long cur0[MAXN + 5], cur1[MAXN + 5];

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        scanf("%s", rawProgram[0]);
        scanf("%s", rawProgram[1]);
        reduce(0);
        reduce(1);
        int lenA = progLen[0], lenB = progLen[1];

        // i = 0 的边界：只可能排 Elsie 的指令，且方案唯一
        pre0[0] = 1;
        pre1[0] = 0;
        for (int j = 1; j <= lenB; ++j) {
            pre0[j] = 0;
            pre1[j] = 1;
        }

        for (int i = 1; i <= lenA; ++i) {
            for (int j = 0; j <= lenB; ++j) {
                // 把 Bessie 的第 i 条（下标 i-1）接在末尾
                long long ways0 = pre0[j];
                if (j > 0 && program[1][j - 1] != program[0][i - 1]) {
                    // 上一状态末尾是 Elsie 的第 j 条，只有类型不同才允许接
                    ways0 += pre1[j];
                }
                cur0[j] = ways0 % MOD;
                if (j == 0) {
                    cur1[0] = 0;
                } else {
                    // 把 Elsie 的第 j 条接在末尾，对前一个状态的末尾类型没有限制
                    cur1[j] = (cur0[j - 1] + cur1[j - 1]) % MOD;
                }
            }
            for (int j = 0; j <= lenB; ++j) {
                pre0[j] = cur0[j];
                pre1[j] = cur1[j];
            }
        }

        long long answer = (pre0[lenB] + pre1[lenB]) % MOD;
        printf("%lld\n", answer);
    }
    return 0;
}
