#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 50000; // 牛棚数上限
const int MAXLOG = 17;  // 2^17 > 50000，向上跳 2^16 步足以从任意牛棚走到根

vector<int> edges[MAXN + 1];  // 邻接表
int parent[MAXN + 1];         // 每个牛棚的父牛棚，根的父牛棚记为 0 号虚拟点
int depth[MAXN + 1];          // 到根的边数
int jumpUp[MAXN + 1][MAXLOG]; // jumpUp[v][j]：v 向上跳 2^j 步到达的祖先
int amount[MAXN + 1];         // 树上差分：某牛棚子树内所有差分值之和 = 经过它的路径数
int visitOrder[MAXN + 1];     // BFS 访问顺序，逆序恰好保证先处理儿子再处理父亲
int bfsQueue[MAXN + 1];

// 求 a、b 的最近公共祖先
int lowestCommonAncestor(int a, int b) {
    if (depth[a] < depth[b]) swap(a, b); // 让 a 更深
    // 从大步到小步把 a 提到和 b 同深度
    for (int j = MAXLOG - 1; j >= 0; --j) {
        if (depth[a] - (1 << j) >= depth[b]) a = jumpUp[a][j];
    }
    if (a == b) return a;
    // 只要跳 2^j 步后两人还不重合，就一起跳过去；
    // 跳完这轮后 a、b 停在 LCA 的两个不同儿子上，所以 LCA 是它们的父亲
    for (int j = MAXLOG - 1; j >= 0; --j) {
        if (jumpUp[a][j] != jumpUp[b][j]) {
            a = jumpUp[a][j];
            b = jumpUp[b][j];
        }
    }
    return parent[a];
}

int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    for (int i = 1; i < n; ++i) {
        int x, y;
        scanf("%d%d", &x, &y);
        edges[x].push_back(y);
        edges[y].push_back(x);
    }

    // 以 1 号牛棚为根做 BFS，同时得到父牛棚、深度和"父先子后"的访问顺序
    int head = 0, tail = 0, orderCount = 0;
    parent[1] = 0;
    depth[1] = 0;
    bfsQueue[tail++] = 1;
    while (head < tail) {
        int cur = bfsQueue[head++];
        visitOrder[orderCount++] = cur;
        for (int i = 0; i < (int)edges[cur].size(); ++i) {
            int nxt = edges[cur][i];
            if (nxt == parent[cur]) continue; // 树里唯一要绕开的就是父亲
            parent[nxt] = cur;
            depth[nxt] = depth[cur] + 1;
            jumpUp[nxt][0] = cur;
            bfsQueue[tail++] = nxt;
        }
    }

    // 倍增表：向上跳 2^j 步 = 先跳 2^(j-1) 步，再跳 2^(j-1) 步
    for (int j = 1; j < MAXLOG; ++j) {
        for (int v = 1; v <= n; ++v) {
            jumpUp[v][j] = jumpUp[jumpUp[v][j - 1]][j - 1];
        }
    }

    for (int i = 0; i < k; ++i) {
        int s, t;
        scanf("%d%d", &s, &t);
        int meeting = lowestCommonAncestor(s, t);
        // 一条路径的差分标记：两端各 +1，LCA 和 LCA 的父亲各 -1。
        // 这样"某个牛棚的子树之和"里，这条路径只在子树真的包含路径上的点时被计入一次。
        amount[s] += 1;
        amount[t] += 1;
        amount[meeting] -= 1;
        amount[parent[meeting]] -= 1;
    }

    // 逆序把儿子的差分值累加到父亲，累加完每个牛棚的 amount 就是经过它的路径数
    for (int i = orderCount - 1; i >= 1; --i) {
        int v = visitOrder[i];
        amount[parent[v]] += amount[v];
    }

    int best = 0;
    for (int v = 1; v <= n; ++v) {
        if (amount[v] > best) best = amount[v];
    }
    printf("%d\n", best);
    return 0;
}
