#include <cstdio>

using namespace std;

const int MAXC = 1001;
bool hasCow[MAXC][MAXC];
int adjCows[MAXC][MAXC];

const int DX[4] = {1, -1, 0, 0};
const int DY[4] = {0, 0, 1, -1};

int main() {
    int n;
    scanf("%d", &n);

    int comfortableCount = 0;

    for (int step = 0; step < n; ++step) {
        int x, y;
        scanf("%d%d", &x, &y);
        hasCow[x][y] = true;

        // 1. 检查新加入的这头牛本身周围是否有 3 头邻居牛
        for (int d = 0; d < 4; ++d) {
            int nx = x + DX[d];
            int ny = y + DY[d];
            if (0 <= nx && nx < MAXC && 0 <= ny && ny < MAXC && hasCow[nx][ny]) {
                adjCows[x][y]++;
            }
        }
        if (adjCows[x][y] == 3) {
            ++comfortableCount;
        }

        // 2. 新加入的牛会导致它的 4 个邻居的邻居数各增加 1
        for (int d = 0; d < 4; ++d) {
            int nx = x + DX[d];
            int ny = y + DY[d];
            if (0 <= nx && nx < MAXC && 0 <= ny && ny < MAXC && hasCow[nx][ny]) {
                if (adjCows[nx][ny] == 3) {
                    // 原本是舒适的, 增加后变成 4, 不再舒适
                    --comfortableCount;
                }
                adjCows[nx][ny]++;
                if (adjCows[nx][ny] == 3) {
                    // 增加后恰好达到 3, 变为舒适
                    ++comfortableCount;
                }
            }
        }

        printf("%d\n", comfortableCount);
    }

    return 0;
}
