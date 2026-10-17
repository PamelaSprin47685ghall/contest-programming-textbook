#include <cstdio>
using namespace std;

const int MAXN = 14;

int N, M, K;
char moves[MAXN + 2];  // Bessie 每轮实际的选择
int card[8 * MAXN];    // 每轮 8 张牌，自上而下

// 64 位 xorshift：把子树求值顺序随机化，防止测试数据针对固定顺序卡掉短路
static unsigned long long rngState = 88172645463325252ULL;
static inline unsigned long long xorshift() {
    rngState ^= rngState << 13;
    rngState ^= rngState >> 7;
    rngState ^= rngState << 17;
    return rngState;
}

// 第 round 轮（0 起）：FJ 选 a（0=上 4 张，1=下 4 张）后，Bessie 选 b，算出新的总路程
int calc(int round, int x, int a, int b) {
    int top = card[round * 8 + a * 4 + b * 2];
    int bottom = card[round * 8 + a * 4 + b * 2 + 1];
    return ((long long)x * (top + 1) + bottom) % M;
}

bool solveOr(int round, int x);

// FJ 的 AND 节点：Bessie 的两种应对都必须可行；随机决定先算哪边以便短路
bool solveAnd(int round, int x, int a) {
    if (xorshift() & 1) {
        if (!solveOr(round + 1, calc(round, x, a, 0))) return false;
        return solveOr(round + 1, calc(round, x, a, 1));
    } else {
        if (!solveOr(round + 1, calc(round, x, a, 1))) return false;
        return solveOr(round + 1, calc(round, x, a, 0));
    }
}

// FJ 的 OR 节点：存在一种选择能让后续全部可行；随机决定先试哪边
bool solveOr(int round, int x) {
    if (round == N) return x <= K || x + K >= M;  // 到终点：离起点不超过 K
    if (xorshift() & 1) {
        if (solveAnd(round, x, 0)) return true;
        return solveAnd(round, x, 1);
    } else {
        if (solveAnd(round, x, 1)) return true;
        return solveAnd(round, x, 0);
    }
}

int main() {
    scanf("%d %d %d", &N, &M, &K);
    scanf("%s", moves);
    for (int i = 0; i < 8 * N; i++) scanf("%d", &card[i]);

    int x = 0;
    char out[MAXN + 2];
    for (int i = 0; i < N; i++) {
        // 贪心：'B' 更小，能选就选（保证整串字典序最小）
        if (solveAnd(i, x, 1)) {
            out[i] = 'B';
            x = calc(i, x, 1, moves[i] == 'T' ? 0 : 1);
        } else {
            out[i] = 'T';  // 题目保证 FJ 总有办法把牛送回家
            x = calc(i, x, 0, moves[i] == 'T' ? 0 : 1);
        }
    }
    out[N] = 0;
    printf("%s\n", out);
    return 0;
}
