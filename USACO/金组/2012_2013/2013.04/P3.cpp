#include <cstdio>
#include <cstring>

using namespace std;

const int MAXN = 305;

int n;
char grid[MAXN][MAXN + 5];

// badPref[r][c] = 第 r 行前 c 个格子中瑕疵 ('*') 的个数, 用来 O(1) 判断一段是否全无瑕
int badPref[MAXN][MAXN + 1];

// dp[L][j]: 固定"下矩形的左右边界恰为 [L, L+sz-1]"以及"共享行是第 j 行"时,
// 上矩形(左右边界含于 [L, L+sz-1] 的任意子区间)能取到的最大面积.
// 注意 dp 是跨 sz 累积的: 每一轮先用上一轮的 dp 做种子, 所以某一行不会被清空,
// 保留的旧值恰好对应"更窄的合法子区间".
int dp[MAXN][MAXN];
int prevDp[MAXN][MAXN];

// 第 r 行在列区间 [L, R] 上是否一个瑕疵都没有
bool rowClean(int r, int L, int R) {
    return badPref[r][R + 1] - badPref[r][L] == 0;
}

int main() {
    scanf("%d", &n);
    for (int r = 0; r < n; r++) {
        scanf("%s", grid[r]);
        badPref[r][0] = 0;
        for (int c = 0; c < n; c++) {
            badPref[r][c + 1] = badPref[r][c] + (grid[r][c] == '*' ? 1 : 0);
        }
    }

    long long answer = 0;

    // sz 枚举下矩形的宽度. 之所以把它放在最外层, 是为了让 dp[L][j] 的含义
    // "上矩形的左右边界含于 [L, L+sz-1]" 能一路继承下来:
    // 任何子区间要么不含最后一列 (上一轮的 dp[L][j]), 要么不含第一列 (上一轮的 dp[L+1][j]).
    for (int sz = 3; sz <= n; sz++) {
        memcpy(prevDp, dp, sizeof(dp));

        for (int L = 0; L + sz <= n; L++) {
            int R = L + sz - 1;

            // 第 j 行要当"共享行"(下矩形的上边), 它必须在 [L, R] 上完全无瑕;
            // 而两条竖边只要求各自的格子在上下矩形的高度范围内无瑕.
            // 于是把行按"两条竖边是否都无瑕"切成若干极大连续段:
            // 共享行落在某段内时, 上矩形的顶边只能是该段里第一个整行无瑕的行,
            // 下矩形的底边只能是该段里最后一个整行无瑕的行.
            int j = 0;
            while (j < n) {
                if (grid[j][L] != '.' || grid[j][R] != '.') {
                    j++;
                    continue;
                }
                int lo = j;
                while (j < n && grid[j][L] == '.' && grid[j][R] == '.') j++;
                int hi = j - 1;

                int firstClean = -1;  // 段内第一个整行无瑕的行 (上矩形顶边的最高可能位置)
                int lastClean = -1;   // 段内最后一个整行无瑕的行 (下矩形底边的最低可能位置)
                for (int t = lo; t <= hi; t++) {
                    if (rowClean(t, L, R)) {
                        if (firstClean < 0) firstClean = t;
                        lastClean = t;
                    }
                }
                if (firstClean < 0) continue;  // 这一段没有一行能当共享行

                for (int t = firstClean; t <= hi; t++) {
                    if (!rowClean(t, L, R)) continue;  // 只有整行无瑕的行才能当共享行

                    int best = prevDp[L][t];
                    if (prevDp[L + 1][t] > best) best = prevDp[L + 1][t];
                    // 上矩形就用整段 [L, R] 时的面积
                    int h = t - firstClean - 1;
                    if (h > 0) {
                        int area = h * (sz - 2);
                        if (area > best) best = area;
                    }
                    dp[L][t] = best;

                    // 组装: 下矩形取整段 [L, R] 与最大高度 [t, lastClean]
                    int bottomH = lastClean - t - 1;
                    if (bottomH > 0) {
                        long long score = (long long)best * bottomH * (sz - 2);
                        if (score > answer) answer = score;
                    }
                }
            }
        }
    }

    printf("%lld\n", answer > 0 ? answer : -1);
    return 0;
}
