#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100005;

int n;
int adjHead[MAXN], adjTo[2 * MAXN], adjNext[2 * MAXN], edgeCount = 0;
int degreeCount[MAXN];
int potionSpawn[MAXN];        // 第 i 次遍历前药水出现的房间
long long potionsAt[MAXN];    // 每个房间里被计入的药水个数
int parent[MAXN];
int visitOrder[MAXN];
int leafCount[MAXN];          // 子树里的叶子个数（起点的房间 1 不算叶子）
long long collected[MAXN];    // 子树里能捡到的药水个数

void addEdge(int u, int v) {
    adjTo[++edgeCount] = v;
    adjNext[edgeCount] = adjHead[u];
    adjHead[u] = edgeCount;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &potionSpawn[i]);
    }
    for (int e = 0; e < n - 1; ++e) {
        int a, b;
        scanf("%d%d", &a, &b);
        addEdge(a, b);
        addEdge(b, a);
        ++degreeCount[a];
        ++degreeCount[b];
    }

    // 覆盖整棵树的最少遍历次数就是叶子个数（房间 1 本身是起点，不算），
    // 之后的遍历不会发生，那些药水可以直接丢掉
    int traversalCount = 0;
    for (int v = 2; v <= n; ++v) {
        if (degreeCount[v] == 1) {
            ++traversalCount;
        }
    }
    for (int i = 0; i < traversalCount; ++i) {
        ++potionsAt[potionSpawn[i]];
    }

    // 从房间 1 出发做一次广度优先，得到 parent 和一个由浅到深的顺序
    parent[1] = 0;
    visitOrder[0] = 1;
    int orderLength = 1;
    for (int qi = 0; qi < orderLength; ++qi) {
        int v = visitOrder[qi];
        for (int e = adjHead[v]; e != 0; e = adjNext[e]) {
            int u = adjTo[e];
            if (u == parent[v]) {
                continue;
            }
            parent[u] = v;
            visitOrder[orderLength++] = u;
        }
    }

    // 按由深到浅的顺序处理，就能保证轮到 v 时子树信息已经算好
    for (int qi = orderLength - 1; qi >= 0; --qi) {
        int v = visitOrder[qi];
        if (v != 1 && degreeCount[v] == 1) {
            leafCount[v] = 1;   // 每个叶子最多一条路径经过自己
            collected[v] = min(1LL, potionsAt[v]);
            continue;
        }
        int leaves = 0;
        long long fromChildren = 0;
        for (int e = adjHead[v]; e != 0; e = adjNext[e]) {
            int u = adjTo[e];
            if (u == parent[v]) {
                continue;
            }
            leaves += leafCount[u];
            fromChildren += collected[u];
        }
        leafCount[v] = leaves;
        // 经过 v 的路径条数受两件事限制：子树里的叶子数，以及下限药水的配对能力
        collected[v] = min((long long)leaves, fromChildren + potionsAt[v]);
    }

    printf("%lld\n", collected[1]);
    return 0;
}
