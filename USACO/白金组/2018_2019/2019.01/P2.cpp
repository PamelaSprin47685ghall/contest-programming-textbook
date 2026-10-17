#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 200000;
const int MAXLOG = 18; // 2^18 > 200000，跳 2^17 步足以从任意点走到根

vector<int> standardEdges[MAXN + 1]; // 前 n-1 条标准道路构成的树
int parent[MAXN + 1];                // 根为 1 号点，根的 parent 记为 0
int depth[MAXN + 1];                 // 到根的边数
int jumpUp[MAXN + 1][MAXLOG];        // 倍增表：向上跳 2^j 步到达的点
int visitOrder[MAXN + 1];            // BFS 顺序，保证父亲先于儿子
int topCount[MAXN + 1];              // topCount[u]：有多少条"竖半段"的顶端边是 (parent[u], u)
long long topPrefix[MAXN + 1];       // 根到 v 的路径上 topCount 之和

int nonStandardX[MAXN + 1];
int nonStandardY[MAXN + 1];
int nonStandardLca[MAXN + 1];

// 把 v 向上提 steps 步
int liftUp(int v, int steps) {
    for (int j = 0; j < MAXLOG; ++j) {
        if ((steps >> j) & 1) v = jumpUp[v][j];
    }
    return v;
}

int lowestCommonAncestor(int a, int b) {
    if (depth[a] < depth[b]) swap(a, b);
    a = liftUp(a, depth[a] - depth[b]);
    if (a == b) return a;
    // 跳到"再跳一步就重合"的位置，此时 a、b 停在最近公共祖先的两个不同儿子上
    for (int j = MAXLOG - 1; j >= 0; --j) {
        if (jumpUp[a][j] != jumpUp[b][j]) {
            a = jumpUp[a][j];
            b = jumpUp[b][j];
        }
    }
    return parent[a];
}

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n - 1; ++i) {
        int a, b;
        scanf("%d%d", &a, &b);
        standardEdges[a].push_back(b);
        standardEdges[b].push_back(a);
    }

    // 以 1 号点为根做 BFS，同时得到父亲、深度、倍增表第一列和访问顺序
    parent[1] = 0;
    depth[1] = 0;
    jumpUp[1][0] = 0;
    int head = 0, tail = 0, orderCount = 0;
    visitOrder[tail++] = 1;
    while (head < tail) {
        int cur = visitOrder[head++];
        ++orderCount;
        for (int i = 0; i < (int)standardEdges[cur].size(); ++i) {
            int nxt = standardEdges[cur][i];
            if (nxt == parent[cur]) continue; // 树里唯一要绕开的就是父亲
            parent[nxt] = cur;
            depth[nxt] = depth[cur] + 1;
            jumpUp[nxt][0] = cur;
            visitOrder[tail++] = nxt;
        }
    }
    for (int j = 1; j < MAXLOG; ++j) {
        for (int v = 1; v <= n; ++v) {
            jumpUp[v][j] = jumpUp[jumpUp[v][j - 1]][j - 1];
        }
    }

    // 两条非标准道路能围成简单环，当且仅当它们在树上的路径至少共一条边。
    // 把每条非标准道路按最近公共祖先 w 切成两条"竖半段"（x 到 w、y 到 w），
    // 之后只数竖半段两两重叠的对数，再把多数的部分扣掉。
    int q = m - (n - 1);
    long long halfCount = 0; // 非空的竖半段总数
    vector<pair<int, int> > topEdgeKeys;
    for (int i = 0; i < q; ++i) {
        int x, y;
        scanf("%d%d", &x, &y);
        int w = lowestCommonAncestor(x, y);
        // 竖半段的"顶端边"是紧挨着 w 下方的那条边，用它的下端编号标记
        int topX = (x == w) ? 0 : liftUp(x, depth[x] - depth[w] - 1);
        int topY = (y == w) ? 0 : liftUp(y, depth[y] - depth[w] - 1);
        nonStandardX[i] = x;
        nonStandardY[i] = y;
        nonStandardLca[i] = w;
        if (topX != 0) {
            ++topCount[topX];
            ++halfCount;
        }
        if (topY != 0) {
            ++topCount[topY];
            ++halfCount;
        }
        // 两条非标准道路可能共用同一条顶端边（连向 w 的同一棵子树），
        // 这种对子会被多数一次，先记下 (topX, topY) 的重复次数
        if (topX != 0 && topY != 0) {
            topEdgeKeys.push_back(make_pair(min(topX, topY), max(topX, topY)));
        }
    }

    // 根到每个点的 topCount 之和：父先子后累加即可
    for (int i = 0; i < orderCount; ++i) {
        int v = visitOrder[i];
        topPrefix[v] = topCount[v] + (v == 1 ? 0 : topPrefix[parent[v]]);
    }

    // 对每条非标准道路，统计"顶端边落在它的两条竖半段内部"的竖半段条数；
    // 再逐条竖半段把每条竖半段自身及其同顶端边的前驱各扣一次，正好抵消重复计数
    long long answer = 0;
    for (int i = 0; i < q; ++i) {
        answer += topPrefix[nonStandardX[i]] + topPrefix[nonStandardY[i]] -
                  2 * topPrefix[nonStandardLca[i]];
    }
    answer -= halfCount;
    for (int v = 1; v <= n; ++v) {
        answer -= (long long)topCount[v] * (topCount[v] - 1) / 2;
    }
    sort(topEdgeKeys.begin(), topEdgeKeys.end());
    for (int i = 0; i < (int)topEdgeKeys.size();) {
        int j = i;
        while (j < (int)topEdgeKeys.size() && topEdgeKeys[j] == topEdgeKeys[i]) ++j;
        long long same = j - i;
        answer -= same * (same - 1) / 2;
        i = j;
    }

    printf("%lld\n", answer);
    return 0;
}
