#include <cstdio>
#include <cstring>

using namespace std;

const int MAXN = 100;
const int WIDTH = 10;

int n, k;
int board[MAXN][WIDTH];      // 0 表示空格, 1..9 表示干草捆的颜色
int region[MAXN][WIDTH];     // 每个格子所属连通块的编号, 0 表示还没访问过
int regionSize[MAXN * WIDTH + 1];

// 从 (r, c) 出发染色: 把同色连通块内的格子都标上编号 id, 并累计它的大小.
void FloodFill(int r, int c, int id, int color) {
    if (r < 0 || r >= n || c < 0 || c >= WIDTH) return;
    if (board[r][c] != color || region[r][c] != 0) return;
    region[r][c] = id;
    ++regionSize[id];
    FloodFill(r - 1, c, id, color);
    FloodFill(r + 1, c, id, color);
    FloodFill(r, c - 1, id, color);
    FloodFill(r, c + 1, id, color);
}

// 消去所有大小达到 k 的连通块; 返回本轮是否真的消去了东西.
bool RemoveRegions() {
    memset(region, 0, sizeof(region));
    int id = 0;
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < WIDTH; ++c) {
            if (board[r][c] != 0 && region[r][c] == 0) {
                ++id;
                regionSize[id] = 0;
                FloodFill(r, c, id, board[r][c]);
            }
        }
    }
    // 题目要求同时消失: 必须先统计完全部连通块, 再统一清除,
    // 否则先清掉的一块会改变后面几块的连通性.
    bool removed = false;
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < WIDTH; ++c) {
            if (board[r][c] != 0 && regionSize[region[r][c]] >= k) {
                board[r][c] = 0;
                removed = true;
            }
        }
    }
    return removed;
}

// 重力: 每一列的干草捆落到底部, 彼此的上下顺序保持不变.
void ApplyGravity() {
    for (int c = 0; c < WIDTH; ++c) {
        int write = n - 1;  // 从底部开始放置已经落下的干草捆
        for (int r = n - 1; r >= 0; --r) {
            if (board[r][c] != 0) {
                board[write][c] = board[r][c];
                --write;
            }
        }
        for (; write >= 0; --write) {
            board[write][c] = 0;
        }
    }
}

int main() {
    scanf("%d%d", &n, &k);
    char line[WIDTH + 1];
    for (int r = 0; r < n; ++r) {
        scanf("%s", line);
        for (int c = 0; c < WIDTH; ++c) {
            board[r][c] = line[c] - '0';
        }
    }

    // 每轮都是"消除一轮 -> 下落", 直到某个时刻没有任何块可消.
    while (RemoveRegions()) {
        ApplyGravity();
    }

    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < WIDTH; ++c) {
            printf("%d", board[r][c]);
        }
        printf("\n");
    }
    return 0;
}
