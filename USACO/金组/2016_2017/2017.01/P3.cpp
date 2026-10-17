#include <cstdio>

using namespace std;

const int MAXN = 20;
// 两头"影子牛"各自由 (位置, 朝向) 描述, 状态总数是 16 * N^4
const int MAXSTATE = 16 * MAXN * MAXN * MAXN * MAXN;

char grid[MAXN][MAXN + 1];
int dist[MAXSTATE];
int queueBuf[MAXSTATE];

// 朝向编号: 0 = 上, 1 = 右, 2 = 下, 3 = 左; 行 0 是输入的第一行(最上面), 列 0 是最左边
const int dr[4] = {-1, 0, 1, 0};
const int dc[4] = {0, 1, 0, -1};

int n;

// 把一个状态压成整数, 便于当作数组下标: (行, 列, 朝向) x (行, 列, 朝向)
int encode(int r1, int c1, int d1, int r2, int c2, int d2) {
    int code = (((r1 * n + c1) * 4 + d1) * n + r2) * n + c2;
    return code * 4 + d2;
}

// 前进一格: 撞墙或撞到干草堆都原地不动; 已经站上终点的牛会忽略之后的指令
void stepForward(int &r, int &c, int d) {
    if (r == 0 && c == n - 1) {
        return;
    }
    int nr = r + dr[d];
    int nc = c + dc[d];
    if (nr < 0 || nr >= n || nc < 0 || nc >= n || grid[nr][nc] == 'H') {
        return;
    }
    r = nr;
    c = nc;
}

int main() {
    scanf("%d", &n);
    for (int r = 0; r < n; ++r) {
        scanf("%s", grid[r]);
    }

    int total = 16 * n * n * n * n;
    for (int i = 0; i < total; ++i) {
        dist[i] = -1;
    }

    // 起点是左下角 (n-1, 0), 两种可能的初始朝向分别是 上(0) 和 右(1)
    int start = encode(n - 1, 0, 0, n - 1, 0, 1);
    int head = 0, tail = 0;
    dist[start] = 0;
    queueBuf[tail++] = start;

    int answer = -1;
    while (head < tail) {
        int code = queueBuf[head++];
        // 从编码里还原出两头的 (行, 列, 朝向)
        int d2 = code % 4;
        int rest = code / 4;
        int c2 = rest % n;
        rest /= n;
        int r2 = rest % n;
        rest /= n;
        int d1 = rest % 4;
        rest /= 4;
        int c1 = rest % n;
        int r1 = rest / n;

        // 两头都到达右上角 (0, n-1) 时, 这条指令序列对两种初始朝向都成立
        if (r1 == 0 && c1 == n - 1 && r2 == 0 && c2 == n - 1) {
            answer = dist[code];
            break;
        }

        for (int cmd = 0; cmd < 3; ++cmd) {
            int nr1 = r1, nc1 = c1, nd1 = d1;
            int nr2 = r2, nc2 = c2, nd2 = d2;
            if (cmd == 0) {  // 前进
                stepForward(nr1, nc1, d1);
                stepForward(nr2, nc2, d2);
            } else if (cmd == 1) {  // 左转
                nd1 = (d1 + 3) % 4;
                nd2 = (d2 + 3) % 4;
            } else {  // 右转
                nd1 = (d1 + 1) % 4;
                nd2 = (d2 + 1) % 4;
            }
            int next = encode(nr1, nc1, nd1, nr2, nc2, nd2);
            // 每条边权都是 1, 第一次访问到的距离就是最短路, 可以直接入队
            if (dist[next] < 0) {
                dist[next] = dist[code] + 1;
                queueBuf[tail++] = next;
            }
        }
    }
    printf("%d\n", answer);
    return 0;
}
