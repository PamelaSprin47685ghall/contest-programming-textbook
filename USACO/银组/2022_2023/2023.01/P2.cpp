#include <cstdio>

using namespace std;

const int MAXN = 1502;

int n;
char dir[MAXN][MAXN];  // 每个格子的路牌方向
int cost[MAXN][MAXN];  // 饲料槽的价格（第 n 行或第 n 列上的格子）
int passCnt[MAXN][MAXN]; // 经过该格子的奶牛数

int main() {
    scanf("%d", &n);
    char buf[MAXN];
    for (int i = 0; i < n; i++) {
        scanf("%s %d", buf, &cost[i][n]);
        for (int j = 0; j < n; j++) dir[i][j] = buf[j];
    }
    for (int j = 0; j < n; j++) scanf("%d", &cost[n][j]);

    // 按行优先顺序扫描：格子 (i, j) 的值只可能来自上方和左方，
    // 它们都已经被处理完，所以处理到 (i, j) 时它的奶牛数已经确定
    long long answer = 0;
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == n || j == n) { // 饲料槽：累加成本
                answer += (long long)passCnt[i][j] * cost[i][j];
                continue;
            }
            passCnt[i][j] += 1; // 每头牛从自己所在的格子出发
            if (dir[i][j] == 'R') passCnt[i][j + 1] += passCnt[i][j];
            else passCnt[i + 1][j] += passCnt[i][j];
        }
    }

    int q;
    scanf("%d", &q);
    printf("%lld\n", answer);
    for (int k = 0; k < q; k++) {
        int i, j;
        scanf("%d %d", &i, &j);
        int y = i - 1, x = j - 1;
        int cnt = passCnt[y][x]; // 这头牛的整条路径都带上/减去这个流量

        // 先撤掉原来的走法：路径上除起点外的格子都减少 cnt
        int r = y, c = x;
        while (r != n && c != n) {
            if (dir[r][c] == 'R') c++;
            else r++;
            passCnt[r][c] -= cnt;
        }
        answer -= (long long)cnt * cost[r][c];

        dir[y][x] = (dir[y][x] == 'R') ? 'D' : 'R';

        // 再加上新走法
        r = y;
        c = x;
        while (r != n && c != n) {
            if (dir[r][c] == 'R') c++;
            else r++;
            passCnt[r][c] += cnt;
        }
        answer += (long long)cnt * cost[r][c];

        printf("%lld\n", answer);
    }
    return 0;
}
