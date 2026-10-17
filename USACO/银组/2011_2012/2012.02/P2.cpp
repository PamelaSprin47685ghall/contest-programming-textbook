#include <cstdio>

using namespace std;

// 解法：长度不超过 L 的合法编号共 C(L,K) 个（长度恰为 l 的有 C(l-1,K-1) 个），
// 先据此定出答案的位数 L，再逐位决定每一位填 0 还是 1。
const int MAXM = 5000; // K >= 2 时答案长度不超过 4500（K=2、N=10^7 时约 4473 位）

long long choose[MAXM + 1][11]; // choose[m][j] = C(m,j)：长度 m 的串里放 j 个 1 的方案数

int main() {
    int N, K;
    scanf("%d %d", &N, &K);

    // K = 1 时合法编号只有 1、10、100……，第 N 个是 1 后面跟 N-1 个 0，
    // 长度最大会到 N（10^7），套不进下面的组合数表，单独处理
    if (K == 1) {
        putchar('1');
        for (int i = 0; i < N - 1; ++i) putchar('0');
        putchar('\n');
        return 0;
    }

    // 杨辉三角：长度为 m 的串里放 j 个 1，最后一位填 0 或填 1 两种情况相加。
    // 组合数只需和 N 比较大小关系，一旦超过 N 就截断成 N，既不会溢出不影响判断
    for (int m = 0; m <= MAXM; ++m) choose[m][0] = 1;
    for (int m = 1; m <= MAXM; ++m) {
        for (int j = 1; j <= K; ++j) {
            long long value = choose[m - 1][j - 1] + choose[m - 1][j];
            choose[m][j] = value > N ? N : value;
        }
    }

    // 找最小的 L 使位数不超过 L 的编号数量 C(L,K) 不低于 N，答案就有 L 位
    int length = K;
    while (length < MAXM && choose[length][K] < N) ++length;

    // 更短的编号一共有 C(L-1,K) 个，所以答案是长度为 L 的编号里第 rank 个
    long long rank = N - choose[length - 1][K];

    putchar('1');         // 最高位一定是 1
    int onesLeft = K - 1; // 剩下 free 位里还要放 onesLeft 个 1
    for (int free = length - 1; free > 0; --free) {
        // 这一位填 0 的编号共 C(free-1,onesLeft) 个，并且都排在填 1 的前面
        if (rank <= choose[free - 1][onesLeft]) {
            putchar('0');
        } else {
            rank -= choose[free - 1][onesLeft];
            putchar('1');
            --onesLeft;
        }
    }
    putchar('\n');
    return 0;
}
