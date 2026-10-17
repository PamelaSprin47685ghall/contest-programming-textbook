#include <cstdio>

using namespace std;

const int OFFSET = 1000;    // 坐标平移量，保证补出来的牛也有合法下标
const int SIZE = 3000;      // 初始坐标在 [0,1000]，补出的牛只会落在半径 1000 的菱形内
const int MAXQ = 2100000;   // 上述菱形的格子数为 2*1000*1000+2*1000+1，每个格子最多入队一次

int n;
char present[SIZE][SIZE];   // 该位置是否已经有牛
char queued[SIZE][SIZE];    // 该位置是否已经在待处理队列里（防止同一格子重复入队）
int qx[MAXQ], qy[MAXQ];
int head, tail;

const int dx[4] = {1, -1, 0, 0};
const int dy[4] = {0, 0, 1, -1};

// 若 (x,y) 有牛且恰好有 3 头相邻的牛，则它缺的那个邻居是必须补上的
void check(int x, int y) {
    if (!present[x][y]) {
        return;
    }
    int adj = 0;
    for (int d = 0; d < 4; ++d) {
        if (present[x + dx[d]][y + dy[d]]) {
            ++adj;
        }
    }
    if (adj != 3) {
        return;
    }
    for (int d = 0; d < 4; ++d) {
        int nx = x + dx[d], ny = y + dy[d];
        if (!present[nx][ny] && !queued[nx][ny]) {
            queued[nx][ny] = 1;
            qx[tail] = nx;
            qy[tail] = ny;
            ++tail;
        }
    }
}

int main() {
    scanf("%d", &n);
    int total = 0;  // 当前草地上牛的总数（含被迫补上的）
    for (int i = 1; i <= n; ++i) {
        int x, y;
        scanf("%d%d", &x, &y);
        x += OFFSET;
        y += OFFSET;
        if (!present[x][y] && !queued[x][y]) {
            queued[x][y] = 1;
            qx[tail] = x;
            qy[tail] = y;
            ++tail;
        }
        // 一头牛一旦被补上就永远留在草地上，所以前后两次询问之间不需要清空状态，
        // 直接把队列处理干净即可得到前 i 头牛的闭包
        while (head < tail) {
            int cx = qx[head], cy = qy[head];
            ++head;
            queued[cx][cy] = 0;
            if (present[cx][cy]) {
                continue;  // 说明这头牛早就被别的格子顺带补上了
            }
            present[cx][cy] = 1;
            ++total;
            // 只有「新牛自己」和「它的 4 个邻居」的相邻牛数发生了变化
            check(cx, cy);
            for (int d = 0; d < 4; ++d) {
                check(cx + dx[d], cy + dy[d]);
            }
        }
        printf("%d\n", total - i);
    }
    return 0;
}
