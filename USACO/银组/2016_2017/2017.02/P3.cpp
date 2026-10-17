#include <cstdio>

using namespace std;

const int MAXN = 105;

int n, k, r;
bool blockRoad[MAXN][MAXN][4];      // 格子 (r,c) 的四个方向上是否有路拦着
int cowHere[MAXN][MAXN];            // 每个格子上的奶牛数
int compId[MAXN][MAXN];             // 所在连通块编号, 0 表示还没访问
int compCow[MAXN * MAXN];           // 每个连通块里的奶牛数

// 方向顺序固定为 上 右 下 左, 与 blockRoad 的第三维下标一致
const int dr[4] = {-1, 0, 1, 0};
const int dc[4] = {0, 1, 0, -1};
const int back[4] = {2, 3, 0, 1};   // 相反方向的下标

int main() {
    scanf("%d%d%d", &n, &k, &r);
    for (int i = 0; i < r; i++) {
        int r1, c1, r2, c2;
        scanf("%d%d%d%d", &r1, &c1, &r2, &c2);
        // 题面保证两个格子相邻, 于是恰好有一个方向与之吻合; 路是双向的, 两边都要标记
        for (int d = 0; d < 4; d++) {
            if (r1 + dr[d] == r2 && c1 + dc[d] == c2) {
                blockRoad[r1][c1][d] = true;
                blockRoad[r2][c2][back[d]] = true;
            }
        }
    }
    for (int i = 0; i < k; i++) {
        int r0, c0;
        scanf("%d%d", &r0, &c0);
        cowHere[r0][c0]++;
    }

    // 手写队列: 被道路隔开的区域互不可达, 一个连通块就是牛能自由活动的范围
    int queueR[MAXN * MAXN], queueC[MAXN * MAXN];
    int compCount = 0;
    for (int sr = 1; sr <= n; sr++) {
        for (int sc = 1; sc <= n; sc++) {
            if (compId[sr][sc]) continue;
            compCount++;
            int head = 0, tail = 0;
            queueR[tail] = sr;
            queueC[tail] = sc;
            tail++;
            compId[sr][sc] = compCount;
            while (head < tail) {
                int cr = queueR[head], cc = queueC[head];
                head++;
                compCow[compCount] += cowHere[cr][cc];
                for (int d = 0; d < 4; d++) {
                    if (blockRoad[cr][cc][d]) continue;     // 这个方向有路, 走不过去
                    int nr = cr + dr[d], nc = cc + dc[d];
                    if (nr < 1 || nr > n || nc < 1 || nc > n) continue;
                    if (compId[nr][nc]) continue;
                    compId[nr][nc] = compCount;
                    queueR[tail] = nr;
                    queueC[tail] = nc;
                    tail++;
                }
            }
        }
    }

    // 两个连通块之间的任意一对牛都隔着至少一条路, 故"远距离"对数 = 总对数 - 同块内的对数
    long long answer = (long long) k * (k - 1);
    for (int i = 1; i <= compCount; i++) {
        answer -= (long long) compCow[i] * (compCow[i] - 1);
    }
    printf("%lld\n", answer / 2);
    return 0;
}
