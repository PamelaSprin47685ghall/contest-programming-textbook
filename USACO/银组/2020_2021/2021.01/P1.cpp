#include <cstdio>

using namespace std;

const int MAXN = 100000;
const int MAXK = 200000;
const int MAXTOT = MAXN + 2 * MAXK + 5;  // 每头牛初始位置 + 每次交换最多新增两个访问位置

int n, k;
int a[MAXK + 5], b[MAXK + 5];
int from[MAXN + 5];     // from[i]：第 i 个位置上现在站的奶牛编号
int succ[MAXN + 5];     // succ[c]：奶牛 c 走完 K 次交换后所在的最终位置
int ans[MAXN + 5];
bool vis[MAXN + 5];
int cover[MAXN + 5];    // cover[x]：当前统计的并集中位置 x 被几个集合覆盖

// 每头奶牛"第一轮 K 次交换中访问过的位置"用链表存，总长度 O(N+K)
int head[MAXN + 5];
int poolPos[MAXTOT], poolNext[MAXTOT], poolCnt;

int cycle[MAXN + 5];    // 当前环上的奶牛
int covered;            // 当前并集大小

void addPos(int cow, int position) {
    poolPos[poolCnt] = position;
    poolNext[poolCnt] = head[cow];
    head[cow] = poolCnt;
    ++poolCnt;
}

void addCow(int cow) {
    for (int e = head[cow]; e != -1; e = poolNext[e]) {
        if (cover[poolPos[e]] == 0) ++covered;
        ++cover[poolPos[e]];
    }
}

void removeCow(int cow) {
    for (int e = head[cow]; e != -1; e = poolNext[e]) {
        if (cover[poolPos[e]] == 1) --covered;
        --cover[poolPos[e]];
    }
}

int main() {
    scanf("%d%d", &n, &k);
    for (int i = 0; i < k; ++i) scanf("%d%d", &a[i], &b[i]);

    for (int i = 1; i <= n; ++i) {
        from[i] = i;        // 一开始位置 i 上是奶牛 i
        head[i] = -1;
        addPos(i, i);       // 奶牛 i 至少待过位置 i
    }

    // 模拟第一轮 K 次交换，同时记录每头奶牛到过的位置
    for (int i = 0; i < k; ++i) {
        int x = a[i], y = b[i];
        int cx = from[x], cy = from[y];
        addPos(cx, y);      // 奶牛 cx 从位置 x 换到了位置 y
        addPos(cy, x);
        from[x] = cy;
        from[y] = cx;
    }

    // from[i] 走完第一轮后停在位置 i，所以 succ[from[i]] = i
    for (int i = 1; i <= n; ++i) succ[from[i]] = i;

    // 之后每 K 次交换，c 的位置沿 succ 前进；同一环上的奶牛最终到过的位置集合相同
    for (int i = 1; i <= n; ++i) {
        if (vis[i]) continue;
        int m = 0;
        for (int c = i; !vis[c]; c = succ[c]) {
            vis[c] = true;
            addCow(c);
            cycle[m++] = c;
        }
        for (int j = 0; j < m; ++j) ans[cycle[j]] = covered;
        // 清空计数，避免不同环互相影响
        for (int j = 0; j < m; ++j) removeCow(cycle[j]);
    }

    for (int i = 1; i <= n; ++i) printf("%d\n", ans[i]);
    return 0;
}
