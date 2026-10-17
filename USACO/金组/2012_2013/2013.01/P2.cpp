#include <cstdio>
#include <deque>

using namespace std;

const int MAXR = 55;
const int MAXC = 55;
const int MAXISLAND = 16;
const int INF = 1000000000;

int rows, cols;
char grid[MAXR][MAXC];
int islandId[MAXR][MAXC];    // 格子所属的岛屿编号, -1 表示不是岛
int islandCnt;
int swim[MAXISLAND][MAXISLAND];  // swim[i][j]: 从岛 i 到岛 j 最少要游过的浅水格数

int cellDist[MAXR * MAXC];       // 从当前起点岛出发到每个格子的最少游泳代价
int dp[1 << MAXISLAND][MAXISLAND];  // dp[已访问岛屿集合][当前所在岛屿]

int floodQueue[MAXR * MAXC];

const int dr[4] = {-1, 0, 0, 1};
const int dc[4] = {0, -1, 1, 0};

// 洪水填充: 每个四连通的 'X' 块编号为一座岛
void labelIslands() {
    islandCnt = 0;
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            islandId[r][c] = -1;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] != 'X' || islandId[r][c] != -1) continue;
            int head = 0, tail = 0;
            islandId[r][c] = islandCnt;
            floodQueue[tail++] = r * cols + c;
            while (head < tail) {
                int cur = floodQueue[head++];
                int cr = cur / cols, cc = cur % cols;
                for (int d = 0; d < 4; d++) {
                    int nr = cr + dr[d], nc = cc + dc[d];
                    if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
                    if (grid[nr][nc] != 'X' || islandId[nr][nc] != -1) continue;
                    islandId[nr][nc] = islandCnt;
                    floodQueue[tail++] = nr * cols + nc;
                }
            }
            islandCnt++;
        }
    }
}

// 从岛屿 source 出发做 0-1 BFS:
// 踩到 'S' 的代价是 1 (浅水格至多被踩一次, 所以代价就是"踩过的浅水格数"),
// 踩到 'X' 的代价是 0, '.' 是深水不能走;
// 边权只有 0 和 1, 用双端队列即可做到线性
void shortestSwimFrom(int source) {
    for (int i = 0; i < rows * cols; i++) cellDist[i] = INF;

    deque<int> dq;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (islandId[r][c] == source) {
                cellDist[r * cols + c] = 0;  // 起点整座岛都是免费的
                dq.push_back(r * cols + c);
            }
        }
    }

    while (!dq.empty()) {
        int cur = dq.front();
        dq.pop_front();
        int cr = cur / cols, cc = cur % cols;
        for (int d = 0; d < 4; d++) {
            int nr = cr + dr[d], nc = cc + dc[d];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            if (grid[nr][nc] == '.') continue;  // 深水不可通行
            int step = (grid[nr][nc] == 'S') ? 1 : 0;
            int next = nr * cols + nc;
            if (cellDist[cur] + step < cellDist[next]) {
                cellDist[next] = cellDist[cur] + step;
                if (step == 0) dq.push_front(next);
                else dq.push_back(next);
            }
        }
    }

    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            if (islandId[r][c] >= 0 && cellDist[r * cols + c] < swim[source][islandId[r][c]])
                swim[source][islandId[r][c]] = cellDist[r * cols + c];
}

// 岛屿数量不超过 15, 用状压 DP 解决旅行商问题
int minSwimToVisitAll() {
    int full = (1 << islandCnt) - 1;
    for (int mask = 0; mask <= full; mask++)
        for (int i = 0; i < islandCnt; i++)
            dp[mask][i] = INF;
    for (int i = 0; i < islandCnt; i++) dp[1 << i][i] = 0;  // 直升机能直接降落在任意一座岛上

    for (int mask = 1; mask <= full; mask++) {
        for (int i = 0; i < islandCnt; i++) {
            if (!((mask >> i) & 1) || dp[mask][i] == INF) continue;
            for (int j = 0; j < islandCnt; j++) {
                if ((mask >> j) & 1) continue;
                int next = dp[mask][i] + swim[i][j];
                if (next < dp[mask | (1 << j)][j]) dp[mask | (1 << j)][j] = next;
            }
        }
    }

    int best = INF;
    for (int i = 0; i < islandCnt; i++)
        if (dp[full][i] < best) best = dp[full][i];
    return best;
}

int main() {
    scanf("%d %d", &rows, &cols);
    for (int r = 0; r < rows; r++) scanf("%s", grid[r]);

    labelIslands();

    for (int i = 0; i < islandCnt; i++)
        for (int j = 0; j < islandCnt; j++)
            swim[i][j] = INF;
    for (int i = 0; i < islandCnt; i++) shortestSwimFrom(i);

    printf("%d\n", minSwimToVisitAll());
    return 0;
}
