#include <cstdio>
#include <vector>
#include <queue>
#include <utility>
#include <functional>

using namespace std;

const int MAXN = 100005;

int n, m;
vector<int> friends[MAXN];  // 邻接表
int degree[MAXN];           // 当前剩余子图里的度数
bool inGraph[MAXN];         // 是否还没被删掉

int removeOrder[MAXN];     // 第 step 个被删掉的点
int degreeAtRemove[MAXN];  // 删它时它还有几个朋友 = 当时它所在连通块的最小度数

// 度数会不断变小，所以用堆 + 懒惰删除：点的度数一变就塞一份新记录，
// 出堆时若发现记录里的度数已经不是它的当前度数（或它已被删掉）就丢弃
priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> candidates;

int parent[MAXN];
int compSize[MAXN];

// 并查集：倒着加点时用来维护连通块大小（隔代路径压缩，避免递归深度问题）
int findRoot(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        int u, v;
        scanf("%d %d", &u, &v);
        friends[u].push_back(v);
        friends[v].push_back(u);
        ++degree[u];
        ++degree[v];
    }

    for (int i = 1; i <= n; ++i) {
        inGraph[i] = true;
        candidates.push(make_pair(degree[i], i));
    }

    // 每一步删掉当前度数最小的点，并记下它当时的度数
    for (int step = 0; step < n; ++step) {
        int u = -1;
        while (true) {
            pair<int, int> top = candidates.top();
            candidates.pop();
            if (inGraph[top.second] && degree[top.second] == top.first) {
                u = top.second;
                break;
            }
        }
        removeOrder[step] = u;
        degreeAtRemove[step] = degree[u];
        inGraph[u] = false;
        for (int k = 0; k < (int)friends[u].size(); ++k) {
            int v = friends[u][k];
            if (!inGraph[v]) {
                continue;  // 已经删掉的邻居不算度数
            }
            --degree[v];
            candidates.push(make_pair(degree[v], v));
        }
    }

    // 倒着把点加回去，用并查集维护连通块大小。
    // 关键结论：设最优友谊组为 G，取 G 中最早被删的点 w，删 w 时全图的最小度数不超过
    // G 内的最小度数，而 G 的规模不超过 w 当时的连通块；反过来，删 w 那一刻的最大连通块 C
    // 里每个点的度数都不小于当时的最小度数，所以 C 的强度不小于"最小度数 * |C|"。
    // 两边夹逼：答案就是各时刻（当时最小度数 * 当时最大连通块大小）的最大值。
    for (int i = 1; i <= n; ++i) {
        parent[i] = i;
        compSize[i] = 1;
        inGraph[i] = false;
    }
    int largestComp = 0;
    long long answer = 0;
    for (int step = n - 1; step >= 0; --step) {
        int u = removeOrder[step];
        inGraph[u] = true;
        if (largestComp < 1) {
            largestComp = 1;
        }
        for (int k = 0; k < (int)friends[u].size(); ++k) {
            int v = friends[u][k];
            if (!inGraph[v]) {
                continue;
            }
            int rootU = findRoot(u);
            int rootV = findRoot(v);
            if (rootU == rootV) {
                continue;
            }
            if (compSize[rootU] < compSize[rootV]) {
                int tmp = rootU;
                rootU = rootV;
                rootV = tmp;
            }
            parent[rootV] = rootU;
            compSize[rootU] += compSize[rootV];
            if (compSize[rootU] > largestComp) {
                largestComp = compSize[rootU];
            }
        }
        long long strength = (long long)degreeAtRemove[step] * largestComp;
        if (strength > answer) {
            answer = strength;
        }
    }

    printf("%lld\n", answer);
    return 0;
}
