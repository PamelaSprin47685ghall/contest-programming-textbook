#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

const int MAX_TIME = 15000;

struct Edge {
    int from;
    int to;
    int time[2];  // 贝茜与艾尔希分别需要的时间。
};

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    vector<Edge> edges(m);
    for (int i = 0; i < m; ++i) {
        scanf("%d%d%d%d", &edges[i].from, &edges[i].to, &edges[i].time[0],
              &edges[i].time[1]);
    }
    // 按终点排序，使同一终点的边连续出现，便于按终点递推。
    sort(edges.begin(), edges.end(),
         [](const Edge& e0, const Edge& e1) { return e0.to < e1.to; });
    // reachable[who][city][t]：who 能否在时刻 t 到达 city。
    vector<vector<char>> reachable[2];
    for (int i = 0; i < 2; ++i) {
        reachable[i].resize(n);
        for (int city = 0; city < n; ++city) reachable[i][city].resize(MAX_TIME + 1);
        reachable[i][0][0] = true;
        for (const Edge& e : edges) {
            for (int t = e.time[i]; t <= MAX_TIME; ++t) {
                reachable[i][e.to - 1][t] |= reachable[i][e.from - 1][t - e.time[i]];
            }
        }
    }
    for (int t = 0; t <= MAX_TIME; ++t) {
        if (reachable[0][n - 1][t] && reachable[1][n - 1][t]) {
            printf("%d\n", t);
            return 0;
        }
    }
    printf("IMPOSSIBLE\n");
    return 0;
}
