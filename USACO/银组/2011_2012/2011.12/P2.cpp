#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 105;
const int INF = 1000000000;

int n, m;
int len[MAXN][MAXN];  // 邻接矩阵, len[i][j] = INF 表示两块田地之间没有路
int dist[MAXN];
int from[MAXN];  // 最短路树上每个点的前驱, 用来还原原来的最短路
bool done[MAXN];

// 朴素 Dijkstra, 返回 1 到 n 的最短距离, 同时把前驱记在 from 里
int shortestPath() {
    for (int i = 1; i <= n; i++) {
        dist[i] = INF;
        done[i] = false;
        from[i] = 0;
    }
    dist[1] = 0;

    for (int step = 0; step < n; step++) {
        int u = 0;  // 本轮选出的 "未确定点中距离最小" 的点
        for (int i = 1; i <= n; i++)
            if (!done[i] && (u == 0 || dist[i] < dist[u])) u = i;
        if (u == 0 || dist[u] == INF) break;  // 剩下的点都不可达
        done[u] = true;

        for (int v = 1; v <= n; v++) {
            int nd = dist[u] + len[u][v];
            if (nd < dist[v]) {
                dist[v] = nd;
                from[v] = u;
            }
        }
    }
    return dist[n];
}

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) len[i][j] = (i == j ? 0 : INF);

    for (int i = 0; i < m; i++) {
        int a, b, l;
        scanf("%d %d %d", &a, &b, &l);
        len[a][b] = len[b][a] = l;  // 题目保证同一对田地之间至多有一条路
    }

    int original = shortestPath();

    // 把原来的最短路还原出来. 为什么要遍历它?
    // 若翻倍的边不在这条最短路 (或其他等长最短路) 上, 这条路依然可走,
    // 最短距离不会变; 只有这条最短路上的边才可能拉长答案.
    int path[MAXN], cnt = 0;
    for (int v = n; v != 0; v = from[v]) path[cnt++] = v;

    int best = original;
    for (int i = 0; i + 1 < cnt; i++) {
        int a = path[i], b = path[i + 1];
        len[a][b] *= 2;
        len[b][a] *= 2;
        best = max(best, shortestPath());
        len[a][b] /= 2;  // 换下一条边前恢复原长
        len[b][a] /= 2;
    }

    printf("%d\n", best - original);
    return 0;
}
