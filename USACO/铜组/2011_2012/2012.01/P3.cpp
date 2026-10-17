#include <cstdio>

using namespace std;

const int SIZE = 5;

const int DX[4] = {1, 0, -1, 0};
const int DY[4] = {0, 1, 0, -1};

// 在 5x5 草地上找一条从左上角到右下角的哈密顿路:
// 每个可走的格子恰好经过一次, 不可走的格子是给定的障碍.
struct Search {
    int blockedCount = 0;          // 障碍数
    bool blocked[SIZE][SIZE] = {}; // 该格是否不可走
    bool visited[SIZE][SIZE] = {}; // 该格是否已走过
    int answer = 0;

    void Compute(int step, int x, int y) {
        if (x == SIZE - 1 && y == SIZE - 1) {
            // 到达终点时, 走过的格子数必须等于全部可走格数.
            if (step + blockedCount == SIZE * SIZE) ++answer;
            return;
        }
        visited[x][y] = true;
        for (int d = 0; d < 4; ++d) {
            int nextX = x + DX[d];
            int nextY = y + DY[d];
            if (nextX < 0 || SIZE <= nextX || nextY < 0 || SIZE <= nextY) continue;
            if (blocked[nextX][nextY] || visited[nextX][nextY]) continue;
            Compute(step + 1, nextX, nextY);
        }
        visited[x][y] = false;
    }
};

int main() {
    int k;
    scanf("%d", &k);

    Search search;
    search.blockedCount = k;
    for (int i = 0; i < k; ++i) {
        int x, y;
        scanf("%d%d", &x, &y);
        search.blocked[x - 1][y - 1] = true;
    }

    search.Compute(1, 0, 0);
    printf("%d\n", search.answer);
    return 0;
}