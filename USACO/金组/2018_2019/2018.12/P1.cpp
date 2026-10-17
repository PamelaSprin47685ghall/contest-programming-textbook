#include <cstdio>
#include <vector>
#include <queue>

using namespace std;

const int MAXN = 50005;
const long long INF = 1e18;

struct Edge {
    int to; // 小径另一端的牧场
    int w;  // 通过这条小径所需时间
};

struct State {
    long long d; // 到达该牧场时的花费
    int u;       // 牧场编号

    // priority_queue 默认是大根堆, 这里把比较反过来, 让它变成小根堆
    bool operator<(const State &other) const {
        return d > other.d;
    }
};

int n, m, k;
vector<Edge> adj[MAXN];
long long distBarn[MAXN]; // 各牧场到谷仓 (N 号牧场) 的最短时间
long long distEat[MAXN];  // 路上吃过一次干草后的最小"净花费"

// 通用 Dijkstra: 调用前把源点的初始距离写进 dist[] 并压入堆
void Dijkstra(long long dist[], priority_queue<State> &pq) {
    while (!pq.empty()) {
        State cur = pq.top();
        pq.pop();
        // 过期状态: 该点已被更短的路径更新过, 直接丢弃
        if (cur.d > dist[cur.u]) continue;
        for (size_t i = 0; i < adj[cur.u].size(); i++) {
            int v = adj[cur.u][i].to;
            int w = adj[cur.u][i].w;
            if (cur.d + w < dist[v]) {
                dist[v] = cur.d + w;
                State nxt;
                nxt.d = dist[v];
                nxt.u = v;
                pq.push(nxt);
            }
        }
    }
}

int main() {
    scanf("%d %d %d", &n, &m, &k);
    for (int i = 0; i < m; i++) {
        int a, b, t;
        scanf("%d %d %d", &a, &b, &t);
        Edge e;
        e.to = b;
        e.w = t;
        adj[a].push_back(e);
        e.to = a;
        adj[b].push_back(e);
    }

    // 第一遍: 以谷仓为源点, 反向求出每个牧场到谷仓的最短时间
    for (int i = 1; i <= n; i++) distBarn[i] = INF;
    priority_queue<State> pq;
    distBarn[n] = 0;
    State start;
    start.d = 0;
    start.u = n;
    pq.push(start);
    Dijkstra(distBarn, pq);

    // 第二遍: 在牧场 h 吃干草, 相当于把"从 h 回谷仓"这段路上省下 y 的代价,
    // 也就是让 h 处的初始花费变成 distBarn[h] - y. 把所有干草所在牧场
    // 当作多源起点再跑一次 Dijkstra, 得到 distEat[i] =
    // min over 干草 h ( dist(i,h) + distBarn[h] - y ).
    for (int i = 1; i <= n; i++) distEat[i] = INF;
    for (int i = 0; i < k; i++) {
        int h, y;
        scanf("%d %d", &h, &y);
        // 同一牧场可能有多块干草, 只保留最划算的那块
        if (distBarn[h] - y < distEat[h]) distEat[h] = distBarn[h] - y;
    }
    for (int i = 1; i <= n; i++) {
        if (distEat[i] < INF) {
            State src;
            src.d = distEat[i];
            src.u = i;
            pq.push(src);
        }
    }
    Dijkstra(distEat, pq);

    // 绕路去吃干草后总时间没有变长, 即增加的时间不超过干草的美味度
    for (int i = 1; i < n; i++) {
        printf("%d\n", distEat[i] <= distBarn[i] ? 1 : 0);
    }
    return 0;
}
