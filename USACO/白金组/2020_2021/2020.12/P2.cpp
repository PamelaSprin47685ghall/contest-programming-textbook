#include <cstdio>

using namespace std;

const int MAXNODE = 120;     // 节点数上限 = 房间数 60 + 询问数 60
const int MAXV = 60;         // 房间数、按钮数、询问数各自的上限
const int MOD = 1000000007;  // 方案数取模

bool door[MAXNODE][MAXNODE];  // door[i][j]：房间 i 到房间 j 有没有门
int startButton[MAXV];        // 每组询问起始时按下的按钮（换成 0 起编号）
int startRoom[MAXV];          // 每组询问起始房间（换成 0 起编号）
int endButton[MAXV];          // 每组询问结束时按下的按钮
int endRoom[MAXV];            // 每组询问结束房间

// dp[a][b]：从节点 a 走到节点 b、只按编号不超过当前按钮的按钮的方案数。
// 节点 0..n-1 是房间；节点 n+j 在作为起点时表示"第 j 组询问的 s 并已经按下 b_s"，
// 作为终点时表示"第 j 组询问的 t 且最后一个按钮是 b_t"
int dp[MAXNODE][MAXNODE];
int nextDp[MAXNODE][MAXNODE];
int leftWays[MAXNODE];   // 按下当前按钮之前的部分，按起点分类
int rightWays[MAXNODE];  // 按下当前按钮之后的部分，按终点分类

int main() {
    int n, k, q;
    scanf("%d %d %d", &n, &k, &q);
    char row[MAXV + 2];
    for (int i = 0; i < n; ++i) {
        scanf("%s", row);
        for (int j = 0; j < n; ++j) door[i][j] = (row[j] == '1');
    }
    for (int j = 0; j < q; ++j) {
        int bs, s, bt, t;
        scanf("%d %d %d %d", &bs, &s, &bt, &t);
        startButton[j] = bs - 1;
        startRoom[j] = s - 1;
        endButton[j] = bt - 1;
        endRoom[j] = t - 1;
    }
    int total = n + q;

    for (int h = 0; h < k; ++h) {
        // 一个只按编号 <= h 的按钮的序列，自然也满足"只按编号 <= h+1 的按钮"
        for (int a = 0; a < total; ++a) {
            for (int b = 0; b < total; ++b) nextDp[a][b] = dp[a][b];
        }
        // 枚举"按钮 h（0 起编号）是在哪个房间按下的"；一个序列里按钮 h 至多按一次，
        // 因为按完之后要比它更大的按钮才能让它重新可用
        for (int c = 0; c < n; ++c) {
            for (int a = 0; a < total; ++a) leftWays[a] = 0;
            for (int b = 0; b < total; ++b) rightWays[b] = 0;
            leftWays[c] = 1;   // 左半段为空：序列从房间 c 出发，第一件事就是按下按钮 h
            rightWays[c] = 1;  // 右半段为空：按下按钮 h 之后就在房间 c 停下
            for (int j = 0; j < q; ++j) {
                // 第 j 组询问的"起点标记"出现在这里，说明这个按钮就是 b_s
                if (startButton[j] == h && startRoom[j] == c) leftWays[n + j] = 1;
                // 第 j 组询问的"终点标记"出现在这里，说明这个按钮就是 b_t
                if (endButton[j] == h && endRoom[j] == c) rightWays[n + j] = 1;
            }
            if (h > 0) {
                // 左半段：起点 a 走到某个真实房间 b，再从 b 走进房间 c
                for (int a = 0; a < total; ++a) {
                    for (int b = 0; b < n; ++b) {
                        if (door[b][c]) {
                            leftWays[a] = (leftWays[a] + dp[a][b]) % MOD;
                        }
                    }
                }
                // 右半段：从房间 c 走进某个真实房间 a，再一直走到终点 b
                for (int b = 0; b < total; ++b) {
                    for (int a = 0; a < n; ++a) {
                        if (door[c][a]) {
                            rightWays[b] = (rightWays[b] + dp[a][b]) % MOD;
                        }
                    }
                }
            }
            for (int a = 0; a < total; ++a) {
                if (leftWays[a] == 0) continue;
                for (int b = 0; b < total; ++b) {
                    if (rightWays[b] == 0) continue;
                    nextDp[a][b] = (int)((nextDp[a][b] +
                                          (long long)leftWays[a] * rightWays[b]) % MOD);
                }
            }
        }
        for (int a = 0; a < total; ++a) {
            for (int b = 0; b < total; ++b) dp[a][b] = nextDp[a][b];
        }
    }

    for (int j = 0; j < q; ++j) printf("%d\n", dp[n + j][n + j]);
    return 0;
}
