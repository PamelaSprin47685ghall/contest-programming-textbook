#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 505;

int N;
int grid[MAXN][MAXN];
bool visited[MAXN][MAXN];
// 手写栈代替递归洪水填充, 免得 N=500 时递归 25 万层把栈撑爆.
int stackR[MAXN * MAXN], stackC[MAXN * MAXN];

const int DR[4] = {1, -1, 0, 0};
const int DC[4] = {0, 0, 1, -1};

// 判断落差为 d 的拖拉机能否开到至少一半的格子:
// 能互相到达的格子连成连通块, 只要有一个连通块够大就行.
bool Possible(int d) {
    int need = (N * N + 1) / 2;  // 至少一半, 奇数时向上取整
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c)
            visited[r][c] = false;

    for (int sr = 0; sr < N; ++sr) {
        for (int sc = 0; sc < N; ++sc) {
            if (visited[sr][sc]) continue;
            int top = 0, size = 0;
            visited[sr][sc] = true;
            stackR[top] = sr;
            stackC[top] = sc;
            ++top;
            while (top > 0) {
                --top;
                int cr = stackR[top], cc = stackC[top];
                ++size;
                for (int k = 0; k < 4; ++k) {
                    int nr = cr + DR[k], nc = cc + DC[k];
                    if (nr < 0 || nr >= N || nc < 0 || nc >= N) continue;
                    if (visited[nr][nc]) continue;
                    int diff = grid[nr][nc] - grid[cr][cc];
                    if (diff < 0) diff = -diff;
                    if (diff > d) continue;
                    visited[nr][nc] = true;
                    stackR[top] = nr;
                    stackC[top] = nc;
                    ++top;
                }
            }
            if (size >= need) return true;
        }
    }
    return false;
}

int main() {
    scanf("%d", &N);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            scanf("%d", &grid[i][j]);

    // 可行性关于价格单调: 价格越高能走的相邻格对越多. 二分最小可行价格.
    // 高度最大 1,000,000, 所以 hi 取 1,000,000 时整个网格必然连通.
    int lo = -1, hi = 1000000;
    while (hi > lo + 1) {
        int mid = (lo + hi) / 2;
        if (Possible(mid)) hi = mid;
        else lo = mid;
    }

    printf("%d\n", hi);
    return 0;
}
