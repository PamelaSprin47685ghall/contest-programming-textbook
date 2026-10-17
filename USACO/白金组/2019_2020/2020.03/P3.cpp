#include <cstdio>
#include <algorithm>

using namespace std;

const int MOD = 1000000007;
const int MAXN = 100005;

int n;

// 邻接表用三个一维数组存
int head[MAXN], nextEdge[2 * MAXN], toVertex[2 * MAXN], edgeCount;
int degree[MAXN];
int parent[MAXN];

// 一条"二度链"：两个端点的度数都不为 2，中间的点度数全为 2
struct Chain {
    int length;  // 链上的边数
    int top, bottom;
};
Chain chains[MAXN];
int chainCount;

bool chainShorter(const Chain &a, const Chain &b) { return a.length < b.length; }

// 并查集：负值表示根，绝对值是集合大小
int dsu[MAXN];

int findRoot(int x) {
    int root = x;
    while (dsu[root] >= 0) root = dsu[root];
    while (dsu[x] >= 0) {  // 路径压缩
        int next = dsu[x];
        dsu[x] = root;
        x = next;
    }
    return root;
}

// 每个连通分量维护两个量：相邻的"饱和链"条数 adjacent、内部并入的链长总和 inner
int adjacent[MAXN], inner[MAXN];

int liveRoot[MAXN], liveCount;  // 当前所有连通分量的代表
int livePos[MAXN];              // 代表在 liveRoot 中的下标，便于 O(1) 删除

long long fact[MAXN], invFact[MAXN];

long long powMod(long long base, long long exponent) {
    long long result = 1;
    while (exponent > 0) {
        if (exponent & 1) result = result * base % MOD;
        base = base * base % MOD;
        exponent >>= 1;
    }
    return result;
}

void addEdge(int u, int v) {
    toVertex[edgeCount] = v;
    nextEdge[edgeCount] = head[u];
    head[u] = edgeCount++;
}

// 把一条饱和链两侧的分量合并（这条链的两端一定分属不同分量：原图是树）
void uniteChain(const Chain &chain) {
    int ru = findRoot(chain.top), rv = findRoot(chain.bottom);
    if (dsu[ru] > dsu[rv]) {  // 让 ru 是较大的集合，保证均摊复杂度
        int tmp = ru;
        ru = rv;
        rv = tmp;
    }
    dsu[ru] += dsu[rv];
    dsu[rv] = ru;
    // rv 不再是代表：把它从活跃表里摘掉（把表尾搬到它的位置）
    int pos = livePos[rv];
    liveRoot[pos] = liveRoot[liveCount - 1];
    livePos[liveRoot[pos]] = pos;
    --liveCount;
    // 这条链由"相邻的饱和链"变成"分量内部"：相邻数各少一，内部链长把 length 加进来
    adjacent[ru] += adjacent[rv] - 2;
    inner[ru] += inner[rv] + chain.length;
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) head[i] = -1;
    for (int i = 1; i < n; ++i) {
        int a, b;
        scanf("%d%d", &a, &b);
        addEdge(a, b);
        addEdge(b, a);
        ++degree[a];
        ++degree[b];
    }

    // 答案形如 K! / Π s_c!，先备好阶乘与逆阶乘
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i % MOD;
    invFact[n] = powMod(fact[n], MOD - 2);
    for (int i = n; i >= 1; --i) invFact[i - 1] = invFact[i] * i % MOD;

    // 以度数不为 2 的点为根做 BFS（树中必存在这样的点，否则度数之和矛盾）
    int root = 1;
    while (degree[root] == 2) ++root;
    int order[MAXN], orderCount = 0;
    parent[root] = 0;
    order[orderCount++] = root;
    for (int idx = 0; idx < orderCount; ++idx) {
        int x = order[idx];
        for (int e = head[x]; e != -1; e = nextEdge[e]) {
            int y = toVertex[e];
            if (y == parent[x]) continue;
            parent[y] = x;
            order[orderCount++] = y;
        }
    }

    // 把树压缩成"度数不为 2 的点 + 二度链"：每条链只需从它最靠上的那个端点向下走一次
    for (int idx = 0; idx < orderCount; ++idx) {
        int x = order[idx];
        if (degree[x] == 2) continue;  // 二度点只会出现在链的内部，不会成为链的端点
        for (int e = head[x]; e != -1; e = nextEdge[e]) {
            int y = toVertex[e];
            if (parent[y] != x) continue;  // 只看孩子，保证每条链只被数一次
            int current = y, length = 1;
            while (degree[current] == 2) {  // 顺着链往下走：二度点只有唯一一个孩子
                for (int f = head[current]; f != -1; f = nextEdge[f]) {
                    int w = toVertex[f];
                    if (w != parent[current]) {
                        current = w;
                        break;
                    }
                }
                ++length;
            }
            chains[chainCount].length = length;
            chains[chainCount].top = x;
            chains[chainCount].bottom = current;
            ++chainCount;
        }
    }
    sort(chains, chains + chainCount, chainShorter);

    // 初始时每个度数不为 2 的点自成一个分量，挨着它的每条链都还是"饱和"的
    liveCount = 0;
    for (int i = 1; i <= n; ++i) dsu[i] = -1;
    for (int i = 1; i <= n; ++i) {
        if (degree[i] == 2) continue;
        adjacent[i] = degree[i];
        inner[i] = 0;
        livePos[i] = liveCount;
        liveRoot[liveCount++] = i;
    }

    long long answer[MAXN];
    answer[n] = fact[n];  // K=N 时没有空位，一步也走不了，N! 个状态各自成一个等价类

    int processed = 0;
    for (int k = n - 1; k >= 1; --k) {
        // 长度为 length 的链在 K ≤ n-2-length 时饱和：它把两侧隔开，两侧的牛再也无法互相穿过
        while (processed < chainCount && chains[processed].length <= n - 2 - k) {
            uniteChain(chains[processed]);
            ++processed;
        }
        // 分量 c 里能自由活动的牛数 s_c = (n-1-k)(y_c-1) + x_c，其中 y_c 是相邻饱和链条数、x_c 是内部链长总和
        long long classes = fact[k];
        for (int i = 0; i < liveCount; ++i) {
            int r = liveRoot[i];
            long long freeCows = (long long)(n - 1 - k) * (adjacent[r] - 1) + inner[r];
            classes = classes * invFact[freeCows] % MOD;
        }
        answer[k] = classes;
    }

    for (int i = 1; i <= n; ++i) printf("%lld\n", answer[i]);
    return 0;
}
