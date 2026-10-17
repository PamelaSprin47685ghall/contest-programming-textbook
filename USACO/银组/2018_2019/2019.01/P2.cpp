#include <cstdio>

using namespace std;

const int MAXN = 1000;
const int MAXCELL = (MAXN + 2) * (MAXN + 2);

char grid[MAXN + 2][MAXN + 2];   // 全局数组默认全 0, 天然在四周形成非 '#' 的边界
bool visited[MAXN + 2][MAXN + 2];
int queueCells[MAXCELL];         // 手写队列存格子编号, 避免递归爆栈
const int DX[4] = {-1, 1, 0, 0};
const int DY[4] = {0, 0, -1, 1};

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        scanf("%s", grid[i] + 1);
        // scanf 写入的第 n+1 个字符是 '\0', 它不等于 '#', 可以直接当边界用
    }

    int cols = n + 2;
    int bestArea = 0, bestPerimeter = 0;
    for (int si = 1; si <= n; ++si) {
        for (int sj = 1; sj <= n; ++sj) {
            if (grid[si][sj] != '#' || visited[si][sj]) {
                continue;  // 已访问过的格子的整块冰已被统计, 跳过保证每格只进队一次
            }
            // 一次 BFS 同时算出所在块儿的面积与周长: 周长就是每条朝外的边
            visited[si][sj] = true;
            int head = 0, tail = 0;
            queueCells[tail++] = si * cols + sj;
            int area = 0, perimeter = 0;
            while (head < tail) {
                int id = queueCells[head++];
                int r = id / cols, c = id % cols;
                ++area;
                for (int k = 0; k < 4; ++k) {
                    int nr = r + DX[k], nc = c + DY[k];
                    if (grid[nr][nc] != '#') {
                        ++perimeter;  // 邻居不是冰, 这条边暴露在边界(包括内洞)上
                    } else if (!visited[nr][nc]) {
                        visited[nr][nc] = true;
                        queueCells[tail++] = nr * cols + nc;
                    }
                }
            }
            if (area > bestArea || (area == bestArea && perimeter < bestPerimeter)) {
                bestArea = area;
                bestPerimeter = perimeter;
            }
        }
    }
    printf("%d %d\n", bestArea, bestPerimeter);
    return 0;
}
