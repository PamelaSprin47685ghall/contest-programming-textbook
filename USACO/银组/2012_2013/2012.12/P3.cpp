#include <cstdio>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

const int MAXN = 505;
const int MAXM = 505;
const long long INF = 4000000000000000000LL;

struct Pipe {
    int to;
    long long latency;
    long long cap;
};

int n, m;
vector<Pipe> pipes[MAXN + 1];
long long caps[MAXM];  // 所有出现过的容量, 最优路径的瓶颈一定是其中之一
long long dist[MAXN + 1];

// 只保留容量 >= minCap 的管道, 求 1 号点到 n 号点的最短延迟; 不可达时返回 INF.
// 此时路径的瓶颈 >= minCap, 时间 L + X/瓶颈 一定不劣于 L + X/minCap.
long long ShortestLatency(long long minCap) {
    for (int i = 1; i <= n; ++i) dist[i] = INF;
    priority_queue<pair<long long, int>, vector<pair<long long, int> >,
                   greater<pair<long long, int> > > pq;
    dist[1] = 0;
    pq.push(make_pair(0LL, 1));
    while (!pq.empty()) {
        pair<long long, int> cur = pq.top();
        pq.pop();
        int u = cur.second;
        if (cur.first > dist[u]) continue;  // 过期状态
        if (u == n) break;
        for (size_t k = 0; k < pipes[u].size(); ++k) {
            const Pipe& p = pipes[u][k];
            if (p.cap < minCap) continue;
            long long nd = cur.first + p.latency;
            if (nd < dist[p.to]) {
                dist[p.to] = nd;
                pq.push(make_pair(nd, p.to));
            }
        }
    }
    return dist[n];
}

int main() {
    long long x;
    scanf("%d%d%lld", &n, &m, &x);
    for (int i = 0; i < m; ++i) {
        int a, b;
        long long l, c;
        scanf("%d%d%lld%lld", &a, &b, &l, &c);
        pipes[a].push_back(Pipe{b, l, c});
        pipes[b].push_back(Pipe{a, l, c});
        caps[i] = c;
    }

    // 枚举瓶颈容量 C: 只走容量 >= C 的管道, 最短延迟就是该 C 下最好的 L,
    // 于是时间 L + X/C (答案要求向下取整, 先除再取整恰好等价).
    long long best = INF;
    for (int i = 0; i < m; ++i) {
        long long l = ShortestLatency(caps[i]);
        if (l == INF) continue;  // 被删掉的管道太多, 无法到达
        long long total = l + x / caps[i];
        if (total < best) best = total;
    }
    printf("%lld\n", best);
    return 0;
}
