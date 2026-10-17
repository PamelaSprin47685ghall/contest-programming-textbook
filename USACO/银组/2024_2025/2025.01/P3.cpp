#include <cstdio>

using namespace std;

// 思路：行交换、列交换只是把表格变成 p_i + q_j（p、q 都是 1..N 的排列），
// 而值交换只是对数值整体重新标号——所以"某个值出现多少次"这个信息在重标号前后完全一致。
// 重标号前出现次数为 1 的值恰好是 2 和 2N，且值为 v 的出现次数是 min(v-1, 2N+1-v)，
// 因此它在 1..N 范围内的出现次数就等于这个值本身。
// 取一个出现次数为 1 的格子 (i, j) 当作重标号前的 2（此时 p_i = q_j = 1）：
// 第 k 行第 j 列的格子重标号前是 p_k + 1，它的出现次数正是 p_k；
// 第 i 行第 k 列的格子重标号前是 1 + q_k，它的出现次数正是 q_k。
// 这样就还原出了全部 p、q。若锚点其实对应 2N，得到的是 p、q 的"镜像"，
// 即另一个候选答案，所以两个候选只需都算出来比较字典序，取较小的那个输出。
const int MAXN = 1000;

int table[MAXN][MAXN];
int candidate[MAXN][MAXN]; // 第一个候选：p_i + q_j
int freq[2 * MAXN + 1];    // 每个数值在表格中出现的次数
int rowValue[MAXN];        // 还原出的 p_1..p_N
int colValue[MAXN];        // 还原出的 q_1..q_N

int main() {
    int N;
    scanf("%d", &N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            scanf("%d", &table[i][j]);
            ++freq[table[i][j]];
        }
    }
    // 找一个出现次数为 1 的值（重标号前的 2 或 2N）作为锚点
    int anchorRow = -1, anchorCol = -1;
    for (int v = 2; v <= 2 * N && anchorRow < 0; ++v) {
        if (freq[v] != 1) {
            continue;
        }
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (table[i][j] == v) {
                    anchorRow = i;
                    anchorCol = j;
                }
            }
        }
    }
    for (int k = 0; k < N; ++k) {
        rowValue[k] = freq[table[k][anchorCol]];
        colValue[k] = freq[table[anchorRow][k]];
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            candidate[i][j] = rowValue[i] + colValue[j];
        }
    }
    // 另一个候选是把每个格子 x 换成 2N+2-x，按自然顺序比较，取第一个不同处更小的那个
    int useSecond = 0; // 0 表示输出候选一，1 表示输出候选二
    bool decided = false;
    for (int i = 0; i < N && !decided; ++i) {
        for (int j = 0; j < N && !decided; ++j) {
            int other = 2 * (N + 1) - candidate[i][j];
            if (candidate[i][j] != other) {
                decided = true;
                useSecond = (other < candidate[i][j]);
            }
        }
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int value = candidate[i][j];
            if (useSecond) {
                value = 2 * (N + 1) - value;
            }
            if (j > 0) {
                printf(" ");
            }
            printf("%d", value);
        }
        printf("\n");
    }
    return 0;
}
