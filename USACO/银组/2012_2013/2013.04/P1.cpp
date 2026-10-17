#include <cstdio>

using namespace std;

const int MAXN = 500;
const int MAXM = 500;

int n, m;
char grid[MAXN][MAXM + 1];  // 第 r 行第 c 列的字符: '.' 空, '#' 阻挡, 'C' 起点, 'D' 终点

// fallDown[r][c] / fallUp[r][c]: 从 (r, c) 出发沿该方向一直下落, 最终停在第几行;
// -1 表示脚下没有格子, 直接飞出网格.
// 只有一直往下(上)一条路, 所以从底(顶)向另一边逐行递推就能 O(N*M) 全部求出,
// 之后每次模拟落体只要 O(1), 不必真的循环走 N 步.
int fallDown[MAXN][MAXM];
int fallUp[MAXN][MAXM];

// dist[r][c][g]: 以重力方向 g 停在 (r, c) 所需的最少翻转次数, -1 表示还没到过.
// 重力方向只可能是向下(0, 初始方向)或向上(1), 而它恰好等于已翻转次数的奇偶,
// 所以同一格的两种方向必须分开记录: 先到的那次不一定能代替后到的那次.
int dist[MAXN][MAXM][2];

// 状态编码: (r * m + c) * 2 + g. 所有状态共 2*N*M 个, 数组按此上界开.
int curBuf[2 * MAXN * MAXM + 5];  // 当前翻转次数下处理的一层状态(栈式增长)
int nxtBuf[2 * MAXN * MAXM + 5];  // 再翻一次即可到达的状态, 等到下一层再确认

int main() {
    scanf("%d %d", &n, &m);
    int cr = -1, cc = -1, dr = -1, dc = -1;
    for (int r = 0; r < n; ++r) {
        scanf("%s", grid[r]);
        for (int c = 0; c < m; ++c) {
            if (grid[r][c] == 'C') {
                cr = r;
                cc = c;
            } else if (grid[r][c] == 'D') {
                dr = r;
                dc = c;
            }
        }
    }

    for (int c = 0; c < m; ++c) {
        // 向下落: 结果只依赖下一行, 必须从最下面一行往上递推
        for (int r = n - 1; r >= 0; --r) {
            if (grid[r][c] == 'D') {
                fallDown[r][c] = r;  // 落到 D 上就停住
            } else if (r + 1 >= n) {
                fallDown[r][c] = -1;  // 下方没有格子, 飞出去
            } else if (grid[r + 1][c] == '#') {
                fallDown[r][c] = r;  // 下方有支撑, 停在原地
            } else {
                fallDown[r][c] = fallDown[r + 1][c];  // 继续往下落
            }
        }
        // 向上落: 结果只依赖上一行, 必须从最上面一行往下递推
        for (int r = 0; r < n; ++r) {
            if (grid[r][c] == 'D') {
                fallUp[r][c] = r;
            } else if (r - 1 < 0) {
                fallUp[r][c] = -1;
            } else if (grid[r - 1][c] == '#') {
                fallUp[r][c] = r;
            } else {
                fallUp[r][c] = fallUp[r - 1][c];
            }
        }
    }

    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < m; ++c) {
            dist[r][c][0] = -1;
            dist[r][c][1] = -1;
        }
    }

    int startRow = fallDown[cr][cc];  // 出发点本身也要先落下(初始重力向下)
    if (startRow < 0) {
        printf("-1\n");  // 一开始就飞出网格, 永远到不了
        return 0;
    }

    // 0-1 BFS 的分层写法: 左右移动不花翻转, 属于同一层, 用栈式扫描把整层做闭包;
    // 翻转花 1 次, 目标放进下一层. 这样每层开始前就能丢掉已被本层以更少翻转标记过的状态.
    int curCnt = 1;
    curBuf[0] = (startRow * m + cc) * 2 + 0;
    dist[startRow][cc][0] = 0;
    int flips = 0;
    while (curCnt > 0) {
        int nxtCnt = 0;
        for (int idx = 0; idx < curCnt; ++idx) {
            int s = curBuf[idx];
            int g = s & 1;
            int pos = s >> 1;
            int r = pos / m;
            int c = pos % m;

            for (int step = -1; step <= 1; step += 2) {  // 左右各试一次
                int c2 = c + step;
                if (c2 < 0 || c2 >= m || grid[r][c2] == '#') {
                    continue;  // 越界或被阻挡的格子不能走
                }
                int r2 = (g == 0 ? fallDown[r][c2] : fallUp[r][c2]);
                if (r2 < 0) {
                    continue;  // 走到这里会掉出网格, 该走法无效
                }
                if (dist[r2][c2][g] == -1) {
                    dist[r2][c2][g] = flips;
                    curBuf[curCnt++] = (r2 * m + c2) * 2 + g;
                }
            }

            int ng = g ^ 1;  // 翻转一次, 重力方向取反
            int r2 = (ng == 0 ? fallDown[r][c] : fallUp[r][c]);
            if (r2 >= 0 && dist[r2][c][ng] == -1) {
                nxtBuf[nxtCnt++] = (r2 * m + c) * 2 + ng;
            }
        }

        curCnt = 0;
        for (int i = 0; i < nxtCnt; ++i) {
            int s = nxtBuf[i];
            int g = s & 1;
            int pos = s >> 1;
            int r = pos / m;
            int c = pos % m;
            if (dist[r][c][g] == -1) {  // 可能刚被本层的左右移动以更少的翻转到达, 那就丢弃
                dist[r][c][g] = flips + 1;
                curBuf[curCnt++] = s;
            }
        }
        ++flips;
    }

    int ans = -1;  // 两种重力方向下到达 D 的较小值就是答案
    for (int g = 0; g < 2; ++g) {
        if (dist[dr][dc][g] >= 0 && (ans < 0 || dist[dr][dc][g] < ans)) {
            ans = dist[dr][dc][g];
        }
    }
    printf("%d\n", ans);
    return 0;
}
