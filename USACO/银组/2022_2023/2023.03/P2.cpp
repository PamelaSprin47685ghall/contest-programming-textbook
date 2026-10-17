#include <cstdio>

using namespace std;

const int MAXN = 100005;
const int MAXMASK = 1 << 18;   // C <= 18

int C, N;
int team[MAXN];        // 每个队伍的二进制掩码，第 i 位为 1 表示第 i 头牛是 Holstein
int dist[MAXMASK];     // dist[x] = 与 x 的汉明距离最小的队伍的汉明距离
int que[MAXMASK];

int main() {
    scanf("%d %d", &C, &N);
    for (int k = 0; k < N; ++k) {
        char s[32];
        scanf("%s", s);
        int mask = 0;
        for (int i = 0; i < C; ++i) {
            if (s[i] == 'H') mask |= 1 << i;
        }
        team[k] = mask;
    }

    // 把所有队伍当作源点做多源 BFS：每翻转一位就是图上的一条边，
    // 于是 dist[x] 就是 x 到最近队伍的距离。
    int total = 1 << C;
    for (int x = 0; x < total; ++x) dist[x] = -1;
    int head = 0, tail = 0;
    for (int k = 0; k < N; ++k) {
        if (dist[team[k]] == -1) {     // 相同队伍只需入队一次，距离都是 0
            dist[team[k]] = 0;
            que[tail++] = team[k];
        }
    }
    while (head < tail) {
        int x = que[head++];
        for (int b = 0; b < C; ++b) {
            int y = x ^ (1 << b);
            if (dist[y] == -1) {
                dist[y] = dist[x] + 1;
                que[tail++] = y;
            }
        }
    }

    // 与队伍 x 差异最大的队伍 = x 取反后距离最近的队伍，
    // 差异 = C - dist[~x]。
    int full = total - 1;
    for (int k = 0; k < N; ++k) {
        printf("%d\n", C - dist[team[k] ^ full]);
    }
    return 0;
}
