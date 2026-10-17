#include <cstdio>

using namespace std;

const int MAXN = 255;
const int INF = 1000000000;

int N, M;
int len[MAXN][MAXN];   // 邻接矩阵: len[a][b] 为小径长度, 无边为 INF
int dist[MAXN];        // 从牧场 1 出发的单源最短路
int pre[MAXN];         // 最短路上的前驱, 用于还原路径
bool visited[MAXN];

// 朴素 Dijkstra: 每次 O(N^2) 选出未访问的最近点并松弛
int shortestPath() {
    for (int i = 1; i <= N; i++) {
        dist[i] = INF;
        pre[i] = -1;
        visited[i] = false;
    }
    dist[1] = 0;
    for (int step = 1; step <= N; step++) {
        int u = -1;
        for (int i = 1; i <= N; i++) {
            if (!visited[i] && (u == -1 || dist[i] < dist[u])) u = i;
        }
        if (u == -1 || dist[u] == INF) break; // 剩下的点都不可达
        visited[u] = true;
        for (int v = 1; v <= N; v++) {
            if (len[u][v] == INF) continue;
            if (dist[u] + len[u][v] < dist[v]) {
                dist[v] = dist[u] + len[u][v];
                pre[v] = u;
            }
        }
    }
    return dist[N];
}

int main() {
    scanf("%d %d", &N, &M);
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) len[i][j] = INF;
    }
    for (int i = 0; i < M; i++) {
        int a, b, l;
        scanf("%d %d %d", &a, &b, &l);
        len[a][b] = l;
        len[b][a] = l;
    }

    int original = shortestPath(); // 原本的最短路长度

    // 从 N 沿最短路前驱走回 1, 把路上的点按 1 -> N 的顺序记下来
    int path[MAXN], cnt = 0;
    for (int v = N; v != -1; v = pre[v]) path[cnt++] = v;
    for (int i = 0; i < cnt / 2; i++) {
        int tmp = path[i];
        path[i] = path[cnt - 1 - i];
        path[cnt - 1 - i] = tmp;
    }

    // 只有把最短路上的边加长才可能让答案变大: 若某边不在最短路 (这条) 上,
    // 那么原来的最短路线依然完好, 距离不变. 于是只需枚举最短路上的 O(N) 条边.
    int best = original;
    for (int i = 0; i + 1 < cnt; i++) {
        int a = path[i], b = path[i + 1];
        len[a][b] *= 2; // 草垛把长度翻倍
        len[b][a] *= 2;
        int now = shortestPath();
        if (now > best) best = now;
        len[a][b] /= 2; // 还原, 继续试下一条边
        len[b][a] /= 2;
    }

    printf("%d\n", best - original);
    return 0;
}
