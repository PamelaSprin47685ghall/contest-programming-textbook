#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 1005;
const int MAXD = 11;    // 每块草地最多 10 个邻居

int quality[MAXN];      // 每块草地的草质量
int deg[MAXN];          // 每块草地的邻居个数
int adj[MAXN][MAXD];    // adj[u][0..deg[u]-1] 是 u 的邻居
int order[MAXN];        // 草地编号按草质量升序排列
int dp[MAXN];           // dp[u]: 最后一口吃在 u 时能积累的最大能量 (含 u 自己那一口)
int dist[MAXN];         // BFS 得到的距离, 单位是边数
int que[MAXN];          // BFS 用的手写队列

// 按草质量升序排列, 于是从后往前处理时质量更小的草地都已经算完了
bool cmpByQuality(int a, int b) {
    return quality[a] < quality[b];
}

int main() {
    int N, cost;
    scanf("%d %d", &N, &cost);      // cost: 走一条相邻路径要消耗的能量
    for (int i = 0; i < N; i++) {
        scanf("%d %d", &quality[i], &deg[i]);
        for (int k = 0; k < deg[i]; k++) {
            scanf("%d", &adj[i][k]);
            adj[i][k]--;            // 输入用 1..N 编号, 转成 0..N-1
        }
        order[i] = i;
    }
    sort(order, order + N, cmpByQuality);

    int result = 0;
    // 质量互不相同, 而且吃过某种质量后不再吃同等或更差的草,
    // 所以她吃过的草地质量严格递增, 从大到小处理就不会再用到没算好的状态
    for (int idx = N - 1; idx >= 0; idx--) {
        int u = order[idx];

        // BFS 求 u 到每块草地的最短距离. 每条相邻路径长度都是 1, 所以第一次访问即最短
        for (int i = 0; i < N; i++) dist[i] = -1;
        int head = 0, tail = 0;
        dist[u] = 0;
        que[tail++] = u;
        while (head < tail) {
            int v = que[head++];
            for (int k = 0; k < deg[v]; k++) {
                int nv = adj[v][k];
                if (dist[nv] == -1) {
                    dist[nv] = dist[v] + 1;
                    que[tail++] = nv;
                }
            }
        }

        // 上一口可以吃在质量更小的 v 上, 之后沿最短路走到 u, 只花 cost * dist[v] 的能量.
        // best 从 0 起算, 相当于"从 u 开始吃, 前面没有别的草地".
        // 质量比 u 大的草地此时 dp 还是 0, 候选值 quality[u] - cost * dist[v] 不会超过 quality[u],
        // 也挤不掉 best, 所以不用特意跳过它们.
        int best = 0;
        for (int v = 0; v < N; v++) {
            if (dist[v] == -1) continue;      // 不连通, 走不过去
            int gain = dp[v] - cost * dist[v];
            if (gain > best) best = gain;
        }
        dp[u] = quality[u] + best;
        if (dp[u] > result) result = dp[u];
    }
    printf("%d\n", result);
    return 0;
}
