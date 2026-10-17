#include <cstdio>

using namespace std;

const int MAXRC = 1000;
// 状态数: 位置最多 1000*1000 种, 每种位置乘 5 种"进入方向"状态, 再乘 2 种气味状态
const int MAXSTATE = 10000010;

int n, m;
// 迷宫下标用 1..n、1..m; 外面预留一圈 0(红色), 这样越界会自然被当成走不通, 省掉边界判断
int grid[MAXRC + 5][MAXRC + 5];
int dist[MAXSTATE];   // -1 表示还没访问过
int que[MAXSTATE];

int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, -1, 0, 1};

// 状态 = 位置 (r, c) + 进入这一格时的滑动方向 lastDir (-1 表示没有在滑动) + 是否有橘子味。
// 先给每个位置一个 0..n*m-1 的编号, 再乘 10 留出低位放 2*(lastDir+1)+气味(最大 9),
// 于是任意两个不同状态编码不同; 编号用乘 m 的方式取, 保证行与列不会互相进位。
int encode(int r, int c, int lastDir, int smell) {
    return 10 * ((r - 1) * m + (c - 1)) + 2 * (lastDir + 1) + smell;
}

int main() {
    scanf("%d %d", &n, &m);
    for (int r = 1; r <= n; r++) {
        for (int c = 1; c <= m; c++) {
            scanf("%d", &grid[r][c]);
        }
    }

    int limit = 10 * n * m;
    for (int i = 0; i < limit; i++) dist[i] = -1;

    // 每条边都恰好花 1 步, 所以直接 BFS; 起点没有在滑动、也没有气味
    int head = 0;
    int tail = 0;
    int start = encode(1, 1, -1, 0);
    dist[start] = 0;
    que[tail++] = start;

    while (head < tail) {
        int code = que[head++];
        int cell = code / 10;
        int r = cell / m + 1;
        int c = cell % m + 1;
        int lastDir = code / 2 % 5 - 1;
        int smell = code % 2;

        if (r == n && c == m) {
            printf("%d\n", dist[code]);
            return 0;
        }

        int forced = 0;   // 是否被紫色格强制滑走
        if (grid[r][c] == 4 && lastDir != -1) {
            int nr = r + dr[lastDir];
            int nc = c + dc[lastDir];
            int color = grid[nr][nc];
            // 紫色格会洗掉橘子味, 所以滑行途中蓝色格一定过不去(0 和 3 都是撞墙)
            if (color != 0 && color != 3) {
                int next = encode(nr, nc, lastDir, color == 2 ? 1 : 0);
                if (dist[next] == -1) {
                    dist[next] = dist[code] + 1;
                    que[tail++] = next;
                }
                forced = 1;
            }
        }
        if (forced) continue;   // 滑行是被迫的, 撞墙停下之前不能自己选方向

        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            int color = grid[nr][nc];
            if (color == 0) continue;
            if (color == 3 && !smell) continue;
            int nextSmell = smell;
            if (color == 2) nextSmell = 1;        // 橙色格染上橘子味
            else if (color == 4) nextSmell = 0;   // 紫色格洗掉橘子味
            int next = encode(nr, nc, d, nextSmell);
            if (dist[next] != -1) continue;
            dist[next] = dist[code] + 1;
            que[tail++] = next;
        }
    }

    printf("-1\n");
    return 0;
}
