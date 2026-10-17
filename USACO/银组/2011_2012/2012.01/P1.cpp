#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <functional>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

const int MAXN = 105;                                  // 农场数量上限
const long long INF = 1000000000000000000LL;

const int DX[4] = {1, -1, 0, 0};
const int DY[4] = {0, 0, 1, -1};

int n;
int farmX[MAXN], farmY[MAXN];

// 关键坐标只取 x-1, x, x+1: 栅格上绕开一个农场所需的最小偏移就是一格,
// 于是所有"拐弯点"都能落在这些坐标上, 网格规模从 1e6 降到 O(n).
int xs[3 * MAXN], ys[3 * MAXN];
int nx, ny;
int farmPos[MAXN];                                     // 每个农场在压缩网格中的编号

vector<char> blocked;
vector<long long> dist;
priority_queue<pair<long long, int>, vector<pair<long long, int> >, greater<pair<long long, int> > > heap;

// 在压缩网格上求从 start 到 goal 的最短路, 被封锁的格子已记录在 blocked 中.
long long ShortestPath(int start, int goal) {
    fill(dist.begin(), dist.end(), INF);
    while (!heap.empty()) heap.pop();

    dist[start] = 0;
    heap.push(make_pair(0LL, start));
    while (!heap.empty()) {
        pair<long long, int> cur = heap.top();
        heap.pop();
        int u = cur.second;
        if (cur.first != dist[u]) continue;            // 堆里的过期记录

        if (u == goal) return dist[u];

        int gx = u / ny, gy = u % ny;
        for (int dir = 0; dir < 4; dir++) {
            int tx = gx + DX[dir], ty = gy + DY[dir];
            if (tx < 0 || tx >= nx || ty < 0 || ty >= ny) continue;

            int v = tx * ny + ty;
            if (blocked[v]) continue;

            // 相邻关键坐标之间的整段直线可以直接走: 中间格子的横坐标(或纵坐标)
            // 严格夹在两个关键值之间, 不可能等于任何农场的坐标, 所以不会撞上农场.
            long long cost = (DX[dir] != 0) ? abs(xs[tx] - xs[gx]) : abs(ys[ty] - ys[gy]);
            if (dist[u] + cost < dist[v]) {
                dist[v] = dist[u] + cost;
                heap.push(make_pair(dist[v], v));
            }
        }
    }
    return -1;                                         // 该段行程无法绕开其他农场
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d%d", &farmX[i], &farmY[i]);
        xs[3 * i] = farmX[i] - 1;
        xs[3 * i + 1] = farmX[i];
        xs[3 * i + 2] = farmX[i] + 1;
        ys[3 * i] = farmY[i] - 1;
        ys[3 * i + 1] = farmY[i];
        ys[3 * i + 2] = farmY[i] + 1;
    }
    sort(xs, xs + 3 * n);
    nx = int(unique(xs, xs + 3 * n) - xs);
    sort(ys, ys + 3 * n);
    ny = int(unique(ys, ys + 3 * n) - ys);
    for (int i = 0; i < n; i++) {
        int gx = int(lower_bound(xs, xs + nx, farmX[i]) - xs);
        int gy = int(lower_bound(ys, ys + ny, farmY[i]) - ys);
        farmPos[i] = gx * ny + gy;
    }
    blocked.assign(nx * ny, 0);
    dist.assign(nx * ny, INF);

    // 各段行程互不影响: 第 i 段只要求不经过除两端以外的农场, 绕路方式彼此独立.
    long long answer = 0;
    for (int leg = 0; leg < n; leg++) {
        int from = leg, to = (leg + 1) % n;
        for (int i = 0; i < nx * ny; i++) blocked[i] = 0;
        for (int i = 0; i < n; i++)
            if (i != from && i != to) blocked[farmPos[i]] = 1;

        long long length = ShortestPath(farmPos[from], farmPos[to]);
        if (length < 0) {
            answer = -1;
            break;
        }
        answer += length;
    }
    printf("%lld\n", answer);
    return 0;
}
