#include <cstdio>
#include <utility>
#include <vector>

using namespace std;

const int MAXCITY = 1000;
const long long INF = 1LL << 60;  // 比任何真实花费都大, 用来表示"到不了"
const int INFHOP = 1 << 30;

// direct[u][v]: 买一张票, 一步从 u 飞到 v 的最小 (花费, 航班数).
// 花费是第一位、航班数是第二位, pair 自带"先比花费再比航班数"的字典序比较.
pair<long long, int> direct[MAXCITY + 1][MAXCITY + 1];
pair<long long, int> best[MAXCITY + 1];  // 从起点 A 到各城市的最小 (花费, 航班数)
bool done[MAXCITY + 1];

int main() {
    int A, B, N;
    scanf("%d %d %d", &A, &B, &N);

    for (int u = 1; u <= MAXCITY; u++) {
        for (int v = 1; v <= MAXCITY; v++) {
            direct[u][v] = make_pair(INF, INFHOP);
        }
        direct[u][u] = make_pair(0, 0);  // 待在原地不花钱也不坐飞机
    }

    for (int i = 0; i < N; i++) {
        long long routeCost;
        int routeLen;
        scanf("%lld %d", &routeCost, &routeLen);
        vector<int> route(routeLen);
        for (int j = 0; j < routeLen; j++) {
            scanf("%d", &route[j]);
            // 在这趟航线里, 任何一站 k 都能直接坐到当前的 j 站:
            // 机票钱只付一次, 航班数就是沿途经过的站数 j-k.
            for (int k = 0; k < j; k++) {
                pair<long long, int> candidate = make_pair(routeCost, j - k);
                if (candidate < direct[route[k]][route[j]]) {
                    direct[route[k]][route[j]] = candidate;
                }
            }
        }
    }

    for (int v = 1; v <= MAXCITY; v++) best[v] = make_pair(INF, INFHOP);
    best[A] = make_pair(0, 0);

    // 城市只有 1000 个, 边却可能很稠密, 用不借助堆的 O(V^2) Dijkstra 最省事.
    for (int round = 0; round < MAXCITY; round++) {
        int u = -1;
        for (int v = 1; v <= MAXCITY; v++) {
            if (done[v]) continue;
            if (u == -1 || best[v] < best[u]) u = v;
        }
        if (u == -1 || best[u].first >= INF) break;  // 剩下的城市都到不了
        done[u] = true;
        for (int v = 1; v <= MAXCITY; v++) {
            if (direct[u][v].first >= INF) continue;
            pair<long long, int> relaxed =
                make_pair(best[u].first + direct[u][v].first,
                          best[u].second + direct[u][v].second);
            if (relaxed < best[v]) best[v] = relaxed;
        }
    }

    if (best[B].first >= INF) printf("-1 -1\n");
    else printf("%lld %d\n", best[B].first, best[B].second);
    return 0;
}
