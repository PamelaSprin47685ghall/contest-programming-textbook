#include <cstdio>
#include <cstring>

using namespace std;

const int MAXN = 50005;                  // 牛的数量上限
const int MAXK = 50;                     // 品种数量上限
const int MAXSTATE = (MAXK + 1) * MAXN;  // 状态总数：品种 0..K 与位置 1..N 的组合

int n, k;
int breed[MAXN];                 // breed[i]：第 i 头牛的品种；breed[n] 后来被换成虚拟品种 0
bool talk[MAXK + 1][MAXK + 1];   // talk[i][j]：品种 i 是否愿意把消息传给品种 j
int dist[MAXSTATE];              // 状态编码 b * (n + 1) + j 的最短时间，-1 表示还没到达
int curQ[MAXSTATE], nextQ[MAXSTATE];  // 分层 BFS 的两层队列

int main() {
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; ++i) scanf("%d", &breed[i]);
    for (int i = 1; i <= k; ++i) {
        char line[MAXK + 2];
        scanf("%s", line);
        for (int j = 1; j <= k; ++j) talk[i][j] = (line[j - 1] == '1');
    }

    // 第 n 头牛换成虚拟品种 0：只有"第 n 头牛真的收下了消息"才可能进入品种 0。
    // 若照原样保留它的品种，消息只是路过位置 n（还没被接收）也会被误判成答案。
    int endBreed = breed[n];
    for (int b = 1; b <= k; ++b) talk[b][0] = talk[b][endBreed];
    breed[n] = 0;

    // 状态 (b, j)：消息正由某头品种 b 的牛发出，此刻恰好落在位置 j，
    // 已经花掉的时间就是"从发送点走到 j"的距离。于是
    //   传一格（时间 +1）：(b, j) -> (b, j±1)
    //   被位置 j 的牛收下（时间 +0）：(b, j) -> (breed[j], j)，前提是 talk[b][breed[j]]
    // 边权只有 0 和 1，按时间分层处理即可（0 权边进当前层，1 权边进下一层）
    memset(dist, -1, sizeof(dist));
    int width = n + 1;
    int* cur = curQ;
    int* nxt = nextQ;
    int curCnt = 0, nxtCnt = 0;
    int start = breed[1] * width + 1;   // 消息一开始握在第 1 头牛手里，时间为 0
    dist[start] = 0;
    cur[curCnt++] = start;

    int d = 0;
    while (curCnt > 0) {
        nxtCnt = 0;
        for (int head = 0; head < curCnt; ++head) {
            int code = cur[head];
            int b = code / width, j = code % width;
            if (b == 0) continue;   // 虚拟品种只表示"已送达"，不需要再扩展
            int c = breed[j];
            if (talk[b][c] && dist[c * width + j] == -1) {
                dist[c * width + j] = d;          // 0 权边：被收下
                cur[curCnt++] = c * width + j;
            }
            if (j > 1 && dist[b * width + j - 1] == -1) {
                dist[b * width + j - 1] = d + 1;  // 1 权边：消息沿线走一格
                nxt[nxtCnt++] = b * width + j - 1;
            }
            if (j < n && dist[b * width + j + 1] == -1) {
                dist[b * width + j + 1] = d + 1;
                nxt[nxtCnt++] = b * width + j + 1;
            }
        }
        int* tmp = cur; cur = nxt; nxt = tmp;
        curCnt = nxtCnt;
        ++d;
    }

    // 虚拟品种 0 在位置 n 的状态编码正好是 0 * width + n = n，
    // 它表示"第 n 头牛收下了消息"；若仍为 -1 说明消息传不到
    printf("%d\n", dist[n]);
    return 0;
}
