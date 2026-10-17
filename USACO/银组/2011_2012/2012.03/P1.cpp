#include <cstdio>
#include <deque>
#include <utility>

using namespace std;

// 干草捆的坐标在 1..1000, 而终点原点 (0,0) 在这一圈之外, 所以把网格取成 0..1001:
// 干草捆不会出现在最外一圈, 最外一圈永远畅通, 绕到更远的地方不可能更省.
const int SIDE = 1002;
const int INF = 1000000000;

bool hasBale[SIDE][SIDE];      // 这个格子有没有干草捆
int balesNeeded[SIDE][SIDE];   // 从拖拉机出发走到这个格子, 至少要搬走几捆干草

int main() {
    int n = 0, startX = 0, startY = 0;
    scanf("%d %d %d", &n, &startX, &startY);
    for (int i = 0; i < n; ++i) {
        int x = 0, y = 0;
        scanf("%d %d", &x, &y);
        hasBale[x][y] = true;
    }

    for (int x = 0; x < SIDE; ++x)
        for (int y = 0; y < SIDE; ++y)
            balesNeeded[x][y] = INF;

    // 把"搬走几捆草"看成边权: 走进有草捆的格子代价 1, 走进空格代价 0.
    // 边权只有 0 和 1, 用双端队列做 0-1 BFS 就够: 代价 0 的邻居放到队首,
    // 代价 1 的放到队尾, 于是出队顺序是按代价从小到大, 等价于 Dijkstra.
    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};
    balesNeeded[startX][startY] = 0;
    deque<pair<int, int> > waiting;
    waiting.push_back(make_pair(startX, startY));
    while (!waiting.empty()) {
        pair<int, int> cur = waiting.front();
        waiting.pop_front();
        for (int d = 0; d < 4; ++d) {
            int nextX = cur.first + dx[d];
            int nextY = cur.second + dy[d];
            if (nextX < 0 || nextX >= SIDE || nextY < 0 || nextY >= SIDE) continue;
            int stepCost = hasBale[nextX][nextY] ? 1 : 0;
            if (balesNeeded[cur.first][cur.second] + stepCost < balesNeeded[nextX][nextY]) {
                balesNeeded[nextX][nextY] = balesNeeded[cur.first][cur.second] + stepCost;
                if (stepCost == 0) {
                    waiting.push_front(make_pair(nextX, nextY));
                } else {
                    waiting.push_back(make_pair(nextX, nextY));
                }
            }
        }
    }

    // 走到原点时路径上经过的草捆数, 就是要搬走的草捆数.
    printf("%d\n", balesNeeded[0][0]);
    return 0;
}
