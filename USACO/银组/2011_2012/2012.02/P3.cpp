#include <algorithm>
#include <cstdio>
#include <functional>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

// 解法：一天的路程 = 农场到第一个集市 + 集市之间按访问顺序的若干段最短路 +
// 最后一个集市回到农场。集市之间的部分只取决于访问顺序（至多 K! 种），
// 农场的两段只取决于农场位置，所以分别用 Dijkstra 求出最短路后枚举即可。
const int MAXN = 10005;
const int INF = 1000000000;

int n, m, k;
vector<pair<int, int> > roads[MAXN]; // roads[u] 里存 (道路长度, 另一端点)
int market[5];                       // 各个集市所在的小镇
bool hasMarket[MAXN];                // 小镇里有没有集市，用来排除不能建农场的镇
int dist[5][MAXN];                   // dist[i][x]：第 i 个集市到小镇 x 的最短距离

// 以第 index 个集市为起点求单源最短路
void dijkstra(int index) {
    for (int x = 1; x <= n; ++x) dist[index][x] = INF;
    dist[index][market[index]] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
    pq.push(make_pair(0, market[index]));
    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        if (d > dist[index][u]) continue; // 堆里可能还留着同一个镇的旧记录
        for (int i = 0; i < (int)roads[u].size(); ++i) {
            int v = roads[u][i].second;
            int nextDist = d + roads[u][i].first;
            if (nextDist < dist[index][v]) {
                dist[index][v] = nextDist;
                pq.push(make_pair(nextDist, v));
            }
        }
    }
}

int main() {
    scanf("%d %d %d", &n, &m, &k);
    for (int i = 0; i < k; ++i) {
        scanf("%d", &market[i]);
        hasMarket[market[i]] = true;
    }
    for (int i = 0; i < m; ++i) {
        int a, b, len;
        scanf("%d %d %d", &a, &b, &len);
        roads[a].push_back(make_pair(len, b));
        roads[b].push_back(make_pair(len, a)); // 道路是双向的
    }
    for (int i = 0; i < k; ++i) dijkstra(i);

    int order[5];
    for (int i = 0; i < k; ++i) order[i] = i;
    long long best = 0;
    bool first = true;
    do { // 枚举集市的访问顺序
        long long travel = 0;
        for (int i = 1; i < k; ++i) { // 集市之间的路程与农场在哪无关，先按顺序加起来
            travel += dist[order[i - 1]][market[order[i]]];
        }
        // 农场的两段：从农场到第一个集市、从最后一个集市回农场。
        // 道路无向，回来的距离就是下标反过来查的距离
        long long bestHome = 2LL * INF;
        for (int x = 1; x <= n; ++x) {
            if (hasMarket[x]) continue; // 有集市的小镇不能建农场
            long long trip = (long long)dist[order[0]][x] + dist[order[k - 1]][x];
            if (trip < bestHome) bestHome = trip;
        }
        if (first || travel + bestHome < best) {
            best = travel + bestHome;
            first = false;
        }
    } while (next_permutation(order, order + k));

    printf("%lld\n", best);
    return 0;
}
