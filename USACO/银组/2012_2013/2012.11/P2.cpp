#include <cstdio>
#include <queue>

using namespace std;

const int MAXN = 35;
const int INF = 1000000000;

int n;
int costSame, costDiff;
char grass[MAXN][MAXN];
int dist[MAXN][MAXN];

struct State {
    int r, c, d;  // 所在牧场与从起点走到这里的用时

    // priority_queue 默认是大根堆, 反过来比较就让用时最小的先出堆
    bool operator<(const State &other) const {
        return d > other.d;
    }
};

const int DR[4] = {-1, 1, 0, 0};
const int DC[4] = {0, 0, -1, 1};

// 以 (sr, sc) 为起点做 Dijkstra, 返回它能到达的最远牧场需要的用时
int FarthestFrom(int sr, int sc) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            dist[i][j] = INF;
        }
    }
    priority_queue<State> pq;
    State start;
    start.r = sr;
    start.c = sc;
    start.d = 0;
    dist[sr][sc] = 0;
    pq.push(start);

    int farthest = 0;
    while (!pq.empty()) {
        State cur = pq.top();
        pq.pop();
        // 同一牧场可能被重复入堆, 距离已经不是最新的那些条目直接丢掉
        if (cur.d != dist[cur.r][cur.c]) {
            continue;
        }
        if (cur.d > farthest) {
            farthest = cur.d;
        }
        for (int k = 0; k < 4; ++k) {
            int nr = cur.r + DR[k];
            int nc = cur.c + DC[k];
            if (nr < 0 || nr >= n || nc < 0 || nc >= n) {
                continue;
            }
            int step = (grass[cur.r][cur.c] == grass[nr][nc]) ? costSame : costDiff;
            int nd = cur.d + step;
            if (nd < dist[nr][nc]) {
                dist[nr][nc] = nd;
                State next;
                next.r = nr;
                next.c = nc;
                next.d = nd;
                pq.push(next);
            }
        }
    }
    return farthest;
}

int main() {
    scanf("%d%d%d", &n, &costSame, &costDiff);
    for (int i = 0; i < n; ++i) {
        scanf("%s", grass[i]);
    }

    // 网格里每个牧场都是节点, 边权只有 A、B 两种; 最长的最短路就是所有起点答案的最大值
    int answer = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int farthest = FarthestFrom(i, j);
            if (farthest > answer) {
                answer = farthest;
            }
        }
    }

    printf("%d\n", answer);
    return 0;
}
