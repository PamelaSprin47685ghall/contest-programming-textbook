#include <algorithm>
#include <cstdio>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int DX[4] = {1, -1, 0, 0};
const int DY[4] = {0, 0, 1, -1};

int main() {
    int n, m;
    if (scanf("%d%d", &n, &m) != 2) return 0;

    vector<string> grid(n);
    char buffer[1005];
    for (int i = 0; i < n; ++i) {
        scanf("%s", buffer);
        grid[i] = buffer;
    }

    // 统计草地和奶牛对之间的关系
    // 每块草地如果周围恰好有 2 头牛, 则这块草地是一个候选聚会点
    // grassToPair[r][c]: 该草地连接的两头牛 (以坐标 pair 对从小到大存储)
    set<pair<pair<int, int>, pair<int, int>>> directPairs;
    map<pair<pair<int, int>, pair<int, int>>, int> pairGrassCount;

    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < m; ++c) {
            if (grid[r][c] == 'G') {
                vector<pair<int, int>> adjCows;
                for (int d = 0; d < 4; ++d) {
                    int nr = r + DX[d];
                    int nc = c + DY[d];
                    if (0 <= nr && nr < n && 0 <= nc && nc < m && grid[nr][nc] == 'C') {
                        adjCows.push_back({nr, nc});
                    }
                }

                if (adjCows.size() == 2) {
                    auto c1 = adjCows[0];
                    auto c2 = adjCows[1];
                    if (c1 > c2) swap(c1, c2);
                    pairGrassCount[{c1, c2}]++;
                } else if (adjCows.size() > 2) {
                    // 如果该草地周围有 >= 3 头牛, 优先用它配对任意一对牛, 且必然不与其他唯一选择冲突
                    // 但 Bronze 经典贪心做法: 
                    // 先配对那些只有 1 块草地可供见面的牛对, 剩下的任意配对
                }
            }
        }
    }

    // 简化建模: 
    // 两头牛之间如果可以做朋友, 它们曼哈顿距离为 2 (同一个草地邻接).
    // 如果一对牛 (c1, c2) 之间恰好只有 1 块公共草地, 那么这块草地只能给它们 (增加 1 友谊).
    // 如果一对牛之间有 2 块公共草地 (即恰好在对角), 它们最多也只能成为一次朋友 (增加 1 友谊).
    // 如果一块草地连接了 >= 3 头牛, 它可以用来撮合任意一对牛.
    // 观察到: 每块连接 >= 3 头牛的草地, 必定可以贪心消耗该草地促成 1 对朋友!
    int answer = 0;
    set<pair<int, int>> usedGrass;

    // 第一轮: 优先处理周围恰好连接 2 头牛, 且两头牛之间只有这 1 块草地的关系
    // 统计每对牛之间的草地
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < m; ++c) {
            if (grid[r][c] == 'G') {
                vector<pair<int, int>> adjCows;
                for (int d = 0; d < 4; ++d) {
                    int nr = r + DX[d];
                    int nc = c + DY[d];
                    if (0 <= nr && nr < n && 0 <= nc && nc < m && grid[nr][nc] == 'C') {
                        adjCows.push_back({nr, nc});
                    }
                }
                if (adjCows.size() > 2) {
                    ++answer;
                } else if (adjCows.size() == 2) {
                    auto c1 = adjCows[0];
                    auto c2 = adjCows[1];
                    if (c1 > c2) swap(c1, c2);
                    if (pairGrassCount[{c1, c2}] == 1) {
                        ++answer;
                    } else if (pairGrassCount[{c1, c2}] == 2) {
                        // 两头对角牛共享 2 块草地: 只有遇到第一块时计入 1 次
                        directPairs.insert({c1, c2});
                    }
                }
            }
        }
    }

    answer += directPairs.size();
    printf("%d\n", answer);
    return 0;
}
