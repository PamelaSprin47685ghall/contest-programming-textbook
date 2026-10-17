#include <algorithm>
#include <cstdio>

using namespace std;

const int INF = 1000000;

int rowBits[9];         // rowBits[r] 记录第 r 行初始状态的 9 位模式 (第 c 位对应第 c 列)
int popCnt[1 << 9];     // popCnt[m]: 模式 m 中 1 的个数, 也就是把某行改成 m 需要翻转的次数
int segOdd[1 << 9][3];  // segOdd[m][k]: 模式 m 在第 k 个 3 列段中 1 的个数是否为奇数
int evenMasks[1 << 9];  // 所有含偶数个 1 的行模式 (行内必须偶数个 1)
int evenCount;

// 状态的第二维: 当前 3x3 宫所在"宫行"里 3 个宫的奇偶性
int dp[1 << 9][1 << 3], ndp[1 << 9][1 << 3];

int main() {
    char buf[16];
    for (int r = 0; r < 9; r++) {
        scanf("%s", buf);
        int mask = 0;
        for (int c = 0; c < 9; c++) {
            if (buf[c] == '1') {
                mask |= 1 << c;
            }
        }
        rowBits[r] = mask;
    }

    for (int m = 0; m < (1 << 9); m++) {
        int cnt = 0;
        for (int c = 0; c < 9; c++) {
            if (m >> c & 1) {
                cnt++;
            }
        }
        popCnt[m] = cnt;
        for (int k = 0; k < 3; k++) {
            // 只看 m 在第 k 段的 3 位, 判断其中 1 的个数奇偶
            int seg = (m >> (3 * k)) & 7;
            int p = 0;
            for (int b = 0; b < 3; b++) {
                if (seg >> b & 1) {
                    p ^= 1;
                }
            }
            segOdd[m][k] = p;
        }
        // 行内 1 的个数必须为偶数, 其余模式不可能出现在合法方案里
        if (cnt % 2 == 0) {
            evenMasks[evenCount++] = m;
        }
    }

    // dp[colmask][boxmask]: 处理完若干行, 各列的累计奇偶性为 colmask,
    // 当前宫行内 3 个宫的累计奇偶性为 boxmask 时的最少翻转数.
    for (int cm = 0; cm < (1 << 9); cm++) {
        for (int bm = 0; bm < (1 << 3); bm++) {
            dp[cm][bm] = INF;
        }
    }
    dp[0][0] = 0;

    for (int r = 0; r < 9; r++) {
        for (int cm = 0; cm < (1 << 9); cm++) {
            for (int bm = 0; bm < (1 << 3); bm++) {
                ndp[cm][bm] = INF;
            }
        }
        for (int cm = 0; cm < (1 << 9); cm++) {
            for (int bm = 0; bm < (1 << 3); bm++) {
                if (dp[cm][bm] == INF) {
                    continue;
                }
                for (int k = 0; k < evenCount; k++) {
                    int m = evenMasks[k];
                    // 这一行改成 m, 各列奇偶性和各宫奇偶性都要异或上 m 的贡献
                    int ncm = cm ^ m;
                    int nbm = bm;
                    for (int b = 0; b < 3; b++) {
                        nbm ^= segOdd[m][b] << b;
                    }
                    int val = dp[cm][bm] + popCnt[m ^ rowBits[r]];
                    if (r % 3 == 2) {
                        // 一个宫行结束: 这 3 个宫必须已经偶数个 1, 之后清零重新统计
                        if (nbm == 0 && val < ndp[ncm][0]) {
                            ndp[ncm][0] = val;
                        }
                    } else if (val < ndp[ncm][nbm]) {
                        ndp[ncm][nbm] = val;
                    }
                }
            }
        }
        for (int cm = 0; cm < (1 << 9); cm++) {
            for (int bm = 0; bm < (1 << 3); bm++) {
                dp[cm][bm] = ndp[cm][bm];
            }
        }
    }

    // 9 行处理完后各列、各宫都必须是偶数个 1, 即 colmask = 0, boxmask = 0
    printf("%d\n", dp[0][0]);
    return 0;
}
