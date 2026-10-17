#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 1005;
const int MAXEDGE = MAXN * (MAXN - 1) / 2 + 5;

// 每两头牛之间都有一条候选边, 权值取它们的平方距离.
// 花 X 元的对讲机正好能让"平方距离不超过 X"的两头牛直接通话,
// 所以答案就是让全图连通所需的最大边权的最小值, 全程不需要开方.
struct Edge {
    int from;
    int to;
    int weight;
};

int cowX[MAXN];
int cowY[MAXN];
Edge edge[MAXEDGE];
int parent[MAXN];

bool compareEdge(const Edge &a, const Edge &b) {
    return a.weight < b.weight;
}

// 并查集: 返回 x 所在树的根, 顺便把路上的结点改挂到根下, 让下一次查找更快.
int findRoot(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d%d", &cowX[i], &cowY[i]);
    }

    int edgeCount = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            int dx = cowX[i] - cowX[j];
            int dy = cowY[i] - cowY[j];
            edge[edgeCount].from = i;
            edge[edgeCount].to = j;
            edge[edgeCount].weight = dx * dx + dy * dy;
            ++edgeCount;
        }
    }

    // Kruskal: 从最小的边开始加, 图第一次连通时加进去的那条边的权值就是答案.
    // 若这时的权值 w 还不够用, 说明所有不超过 w 的边都没能把图连通, 与刚连通矛盾;
    // 而每条边又都必要, 所以 w 也不可能更小.
    sort(edge, edge + edgeCount, compareEdge);

    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }
    int componentCount = n;
    int answer = 0;
    for (int k = 0; k < edgeCount && componentCount > 1; ++k) {
        int rootFrom = findRoot(edge[k].from);
        int rootTo = findRoot(edge[k].to);
        // 两端已经在同一个连通块里的边只会成环, 换成更小的权值也一样不该加.
        if (rootFrom != rootTo) {
            parent[rootFrom] = rootTo;
            --componentCount;
            answer = edge[k].weight;
        }
    }
    printf("%d\n", answer);
    return 0;
}
