#include <cstdio>

using namespace std;

const int MAXN = 55;           // 网格最大 50x50
const int MAXC = MAXN * MAXN;
const int INF = 1000000000;

int n, m;
char hide[MAXN][MAXN + 1];     // 牛皮图案: 'X' 是斑点的一部分, '.' 是空白
int spot[MAXN][MAXN];          // 0 表示空白, 1..3 表示属于第几个斑点
int dist[3][MAXN][MAXN];       // 第 s 个斑点到每个格子的最短步数
int qx[MAXC], qy[MAXC];        // 手写队列, 避免递归过深

const int DX[4] = {-1, 1, 0, 0};
const int DY[4] = {0, 0, -1, 1};

// 洪水填充: 把与 (si,sj) 四相邻连通的 'X' 全部标成编号 id
void floodFill(int si, int sj, int id) {
    int head = 0, tail = 0;
    spot[si][sj] = id;
    qx[tail] = si; qy[tail] = sj; tail++;
    while (head < tail) {
        int i = qx[head], j = qy[head];
        head++;
        for (int d = 0; d < 4; d++) {
            int ni = i + DX[d], nj = j + DY[d];
            if (ni < 0 || ni >= n || nj < 0 || nj >= m) continue;
            if (hide[ni][nj] != 'X' || spot[ni][nj] != 0) continue;
            spot[ni][nj] = id;
            qx[tail] = ni; qy[tail] = nj; tail++;
        }
    }
}

// 以第 s 个斑点的所有格子为源点做多源 BFS.
// 因为任何格子都可以被涂上颜料(路径可以穿过任意格子),
// 所以两格之间的最短步数就是曼哈顿距离, BFS 能直接求出.
void bfs(int s) {
    int head = 0, tail = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            if (spot[i][j] == s + 1) {
                dist[s][i][j] = 0;
                qx[tail] = i; qy[tail] = j; tail++;
            } else {
                dist[s][i][j] = -1;
            }
        }
    while (head < tail) {
        int i = qx[head], j = qy[head];
        head++;
        for (int d = 0; d < 4; d++) {
            int ni = i + DX[d], nj = j + DY[d];
            if (ni < 0 || ni >= n || nj < 0 || nj >= m) continue;
            if (dist[s][ni][nj] != -1) continue;
            dist[s][ni][nj] = dist[s][i][j] + 1;
            qx[tail] = ni; qy[tail] = nj; tail++;
        }
    }
}

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; i++) scanf("%s", hide[i]);

    // 三个斑点分别编号 1, 2, 3
    int numSpots = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (hide[i][j] == 'X' && spot[i][j] == 0)
                floodFill(i, j, ++numSpots);

    for (int s = 0; s < 3; s++) bfs(s);

    int best = INF;

    // 情况一(三岔形): 交汇点取空白格 (i,j), 从它向三个斑点各修一条最短路.
    // 到第 s 个斑点要走 d 步, 路上要新涂 d-1 个格子, 再加上交汇点自身.
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (spot[i][j] == 0) {
                int cost = dist[0][i][j] + dist[1][i][j] + dist[2][i][j] - 2;
                if (cost < best) best = cost;
            }

    // 情况二(链形): 三条两两连线里选最短的两条, 把三个斑点串成一串.
    // 两斑点间走 d 步时, 路径中需要新涂 d-1 个格子.
    // 交汇点若落在某个斑点内部, 修的路正好就是"从该斑点出发的两条边",
    // 已被这里覆盖, 所以无需再单独枚举.
    int link[3];
    link[0] = link[1] = link[2] = INF;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            if (spot[i][j] == 2 && dist[0][i][j] < link[0]) link[0] = dist[0][i][j];  // 斑点 1-2
            if (spot[i][j] == 3 && dist[1][i][j] < link[1]) link[1] = dist[1][i][j];  // 斑点 2-3
            if (spot[i][j] == 3 && dist[0][i][j] < link[2]) link[2] = dist[0][i][j];  // 斑点 1-3
        }
    for (int k = 0; k < 3; k++) link[k]--;  // m 步路中间有 m-1 个格子要新涂

    // 取最短的两条(等价于三点最小生成树), 三条边的三种两两组合都试一遍
    int chain = INF;
    for (int k = 0; k < 3; k++) {
        int other = link[(k + 1) % 3];
        if (link[k] < INF && other < INF && link[k] + other < chain)
            chain = link[k] + other;
    }
    if (chain < best) best = chain;

    printf("%d\n", best);
    return 0;
}
