#include <cstdio>

using namespace std;

const int MAXN = 100005;             // 单个子测试的字段数最多 10^5
const long long INF = 1000000000LL;  // 真实距离最大 10^5, 取它当"无穷大"

int parent[MAXN];    // 并查集: 每个字段的父节点
int compSize[MAXN];  // 按大小合并时用到的连通块大小
int rootOf[MAXN];    // 每个字段所在连通块的代表元
long long nearSrc[MAXN];  // 每个字段到"谷仓 1 所在块"中最近字段的距离
long long nearDst[MAXN];  // 每个字段到"谷仓 N 所在块"中最近字段的距离
long long bestSrc[MAXN];  // 每个连通块到谷仓 1 所在块的最近距离
long long bestDst[MAXN];  // 每个连通块到谷仓 N 所在块的最近距离

int Find(int x) {
    // 路径减半: 顺路把节点挂到自己的爷爷上, 树高很快降下来.
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

void Unite(int a, int b) {
    a = Find(a);
    b = Find(b);
    if (a == b) return;
    if (compSize[a] < compSize[b]) {
        int tmp = a;
        a = b;
        b = tmp;
    }
    parent[b] = a;  // 小的块挂到大的块下面, 避免长成一条链
    compSize[a] += compSize[b];
}

// 字段编号就是坐标, 所以一个字段到某个块中最近字段的距离, 必然来自
// 它左边最近的同块字段或右边最近的同块字段: 从左到右、从右到左各扫一遍即可.
void FillNearDistances(int n, int srcBlock, int dstBlock) {
    int lastSrc = 0;  // 0 表示这一侧还没有出现过该块中的字段
    int lastDst = 0;
    for (int v = 1; v <= n; ++v) {
        if (rootOf[v] == srcBlock) lastSrc = v;
        if (rootOf[v] == dstBlock) lastDst = v;
        nearSrc[v] = (lastSrc == 0) ? INF : (v - lastSrc);
        nearDst[v] = (lastDst == 0) ? INF : (v - lastDst);
    }
    lastSrc = 0;
    lastDst = 0;
    for (int v = n; v >= 1; --v) {
        if (rootOf[v] == srcBlock) lastSrc = v;
        if (rootOf[v] == dstBlock) lastDst = v;
        if (lastSrc != 0 && lastSrc - v < nearSrc[v]) nearSrc[v] = lastSrc - v;
        if (lastDst != 0 && lastDst - v < nearDst[v]) nearDst[v] = lastDst - v;
    }
}

int main() {
    int T;
    scanf("%d", &T);
    for (int tc = 0; tc < T; ++tc) {
        int N, M;
        scanf("%d%d", &N, &M);
        for (int v = 1; v <= N; ++v) {
            parent[v] = v;
            compSize[v] = 1;
            bestSrc[v] = INF;
            bestDst[v] = INF;
        }
        for (int e = 0; e < M; ++e) {
            int i, j;
            scanf("%d%d", &i, &j);
            Unite(i, j);
        }

        // 已有的路径先把字段并成连通块; 再新加的每条路径都只能连接两个连通块.
        for (int v = 1; v <= N; ++v) rootOf[v] = Find(v);
        int srcBlock = rootOf[1];
        int dstBlock = rootOf[N];
        if (srcBlock == dstBlock) {
            printf("0\n");
            continue;
        }
        FillNearDistances(N, srcBlock, dstBlock);

        // 每个块到源块 / 目标块的最近距离, 取块内所有字段的最小值.
        for (int v = 1; v <= N; ++v) {
            int block = rootOf[v];
            if (nearSrc[v] < bestSrc[block]) bestSrc[block] = nearSrc[v];
            if (nearDst[v] < bestDst[block]) bestDst[block] = nearDst[v];
        }

        // 新加的路径要么直接连源块与目标块, 要么经过一个中间块 c 连两条边,
        // 两种情况都能写成"源块 -> c -> 目标块"; c 取遍所有连通块,
        // 取到源块(或目标块)时就是只加一条边的情形. 费用是距离的平方.
        long long answer = INF * INF;  // 先放一个比任何真实费用都大的数
        for (int v = 1; v <= N; ++v) {
            if (rootOf[v] != v) continue;  // 每个连通块只统计一次
            long long cost = bestSrc[v] * bestSrc[v] + bestDst[v] * bestDst[v];
            if (cost < answer) answer = cost;
        }
        printf("%lld\n", answer);
    }
    return 0;
}
