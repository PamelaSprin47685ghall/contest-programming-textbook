#include <cstdio>
#include <queue>
#include <string>
#include <vector>

using namespace std;

const int DX[4] = {1, 0, -1, 0};
const int DY[4] = {0, 1, 0, -1};

// 优先队列里的一项: 记录到达某个格子的代价. 重载 < 使代价小的先出队.
struct QueueItem {
    int cost;
    int index;

    bool operator<(const QueueItem& other) const {
        // 反着比较, 让最小代价的项排在堆顶.
        if (cost != other.cost) return cost > other.cost;
        return index < other.index;
    }
};

// 把二维坐标压成一个整数, 便于放进优先队列.
int Encode(int x, int y, int width) {
    return x * width + y;
}

int main() {
    int rows, cols;
    scanf("%d%d", &rows, &cols);
    vector<string> grid(rows);
    char buffer[80];
    for (string& row : grid) {
        scanf("%s", buffer);
        row = buffer;
    }

    // 找第一个 X 作为起点.
    int startX = -1, startY = -1;
    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < cols; ++y) {
            if (grid[x][y] == 'X') {
                startX = x;
                startY = y;
                break;
            }
        }
        if (startX != -1) break;
    }

    // 走到 X 代价为 0, 走到 . 代价为 1. 求到另一块 X 的最短代价.
    const int infinity = 1 << 30;
    vector<int> distance(rows * cols, infinity);
    priority_queue<QueueItem> queue;

    int start = Encode(startX, startY, cols);
    distance[start] = 0;
    queue.push(QueueItem{0, start});

    int answer = -1;
    while (!queue.empty()) {
        QueueItem item = queue.top();
        queue.pop();
        if (item.cost != distance[item.index]) continue;  // 过期的旧记录, 跳过.

        int x = item.index / cols;
        int y = item.index % cols;
        // 找到另一块 X (代价非零) 即完成.
        if (item.cost != 0 && grid[x][y] == 'X') {
            answer = item.cost;
            break;
        }

        for (int d = 0; d < 4; ++d) {
            int nextX = x + DX[d];
            int nextY = y + DY[d];
            if (nextX < 0 || rows <= nextX || nextY < 0 || cols <= nextY) continue;
            int stepCost = (grid[nextX][nextY] == 'X') ? 0 : 1;
            int nextIndex = Encode(nextX, nextY, cols);
            if (distance[nextIndex] > item.cost + stepCost) {
                distance[nextIndex] = item.cost + stepCost;
                queue.push(QueueItem{distance[nextIndex], nextIndex});
            }
        }
    }

    printf("%d\n", answer);
    return 0;
}