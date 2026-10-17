#include <cstdio>
#include <cstring>
using namespace std;

// N <= 20，每个 combo 长度 <= 15，trie 节点数至多 20*15+1
const int MAXNODES = 20 * 15 + 5;
const int MAXK = 1000;
const int NEG = -1000000000;

// child[u][c]：节点 u 沿字符 c 转移到的节点（补全后是 AC 自动机的 go 表）
int child[MAXNODES][3];
int fail[MAXNODES];
// score[u]：AC 自动机走到节点 u 时，本次按键新增的得分（u 本身及其失败链上所有 combo 结尾）
int score[MAXNODES];
int nodeCnt = 1;  // 节点 0 是根

// dp[k][u]：按了 k 个键、AC 状态停在 u 时的最大得分
int dp[MAXK + 1][MAXNODES];

char buf[20];

int main() {
    int N, K;
    scanf("%d %d", &N, &K);
    memset(child, -1, sizeof(child));
    for (int i = 0; i < N; i++) {
        scanf("%s", buf);
        int u = 0;
        for (int j = 0; buf[j]; j++) {
            int c = buf[j] - 'A';
            if (child[u][c] == -1) {
                child[u][c] = nodeCnt;
                nodeCnt++;
            }
            u = child[u][c];
        }
        score[u]++;  // 该节点是某个 combo 的结尾
    }

    // 广搜建立失败指针，并用数组当队列避免引入 <queue>
    int queue_[MAXNODES];
    int head = 0, tail = 0;
    for (int c = 0; c < 3; c++) {
        if (child[0][c] == -1) child[0][c] = 0;  // 根缺儿子时回到根
        else {
            fail[child[0][c]] = 0;
            queue_[tail++] = child[0][c];
        }
    }
    while (head < tail) {
        int u = queue_[head++];
        score[u] += score[fail[u]];  // 失败链上的 combo 也在此刻结尾，得分要累加
        for (int c = 0; c < 3; c++) {
            int v = child[u][c];
            if (v == -1) {
                child[u][c] = child[fail[u]][c];  // 沿失败链转移
            } else {
                fail[v] = child[fail[u]][c];
                queue_[tail++] = v;
            }
        }
    }

    for (int k = 0; k <= K; k++)
        for (int u = 0; u < nodeCnt; u++)
            dp[k][u] = NEG;
    dp[0][0] = 0;
    for (int k = 0; k < K; k++) {
        for (int u = 0; u < nodeCnt; u++) {
            if (dp[k][u] == NEG) continue;
            for (int c = 0; c < 3; c++) {
                int v = child[u][c];
                int cand = dp[k][u] + score[v];
                if (cand > dp[k + 1][v]) dp[k + 1][v] = cand;
            }
        }
    }

    int best = 0;
    for (int u = 0; u < nodeCnt; u++)
        if (dp[K][u] > best) best = dp[K][u];
    printf("%d\n", best);
    return 0;
}
