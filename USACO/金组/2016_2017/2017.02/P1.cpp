#include <cstdio>
#include <queue>

using namespace std;

const int MAXN = 105;
const long long INF = 0x3f3f3f3f3f3f3f3fLL;

int grass[MAXN][MAXN];
// dist[k][r][c]: 走到 (r,c) 且距上一次吃草刚好又走了 k 步时的最小总耗时.
// k 只可能是 0,1,2: 走到第三个格子就必须停下吃草, 所以不存在 k >= 3 的状态.
long long dist[3][MAXN][MAXN];

int stepR[4] = {-1, 1, 0, 0};
int stepC[4] = {0, 0, -1, 1};

int main() {
    int n;
    long long t;
    scanf("%d%lld", &n, &t);
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            scanf("%d", &grass[r][c]);
        }
    }

    for (int k = 0; k < 3; ++k) {
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) {
                dist[k][r][c] = INF;
            }
        }
    }
    dist[0][0][0] = 0;  // 起点本身不吃草, 所以当作"刚吃完"的 k = 0 开始

    // 边权全部非负, 用 Dijkstra; 状态编号 k * n * n + r * n + c.
    priority_queue<pair<long long, int> > heap;
    heap.push(make_pair(0, 0));
    while (!heap.empty()) {
        long long cur = -heap.top().first;
        int id = heap.top().second;
        heap.pop();
        int k = id / (n * n);
        int r = (id / n) % n;
        int c = id % n;
        if (cur != dist[k][r][c]) {
            continue;  // 这是同一状态被更优解覆盖后留下的过期记录
        }

        for (int i = 0; i < 4; ++i) {
            int nr = r + stepR[i];
            int nc = c + stepC[i];
            if (nr < 0 || nr >= n || nc < 0 || nc >= n) {
                continue;
            }
            long long cost = cur + t;  // 过一条马路
            int nextK = k + 1;
            if (nextK == 3) {
                // 这一格是路上访问的第三格, 必须停下吃草, 它的草量计入总耗时
                cost += grass[nr][nc];
                nextK = 0;
            }
            if (cost < dist[nextK][nr][nc]) {
                dist[nextK][nr][nc] = cost;
                heap.push(make_pair(-cost, nextK * n * n + nr * n + nc));
            }
        }
    }

    // 到达终点时不必再吃草, 三种 k 都算到达
    long long answer = INF;
    for (int k = 0; k < 3; ++k) {
        if (dist[k][n - 1][n - 1] < answer) {
            answer = dist[k][n - 1][n - 1];
        }
    }
    printf("%lld\n", answer);
    return 0;
}
