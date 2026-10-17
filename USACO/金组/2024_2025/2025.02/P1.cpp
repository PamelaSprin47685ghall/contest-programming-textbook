#include <cstdio>
#include <algorithm>

using namespace std;

typedef long long ll;

const int MAXN = 200005;
const ll INF = (ll)1e18;    // 表示"该状态不合法"

int n;
int nxt[MAXN];              // nxt[i] = a[i], 即 f(i) 指向的点
ll cost[MAXN];              // cost[i] = c[i]; 若 a[i] 本来就是 i, 则置 0
int indeg[MAXN];            // 每个点还没处理完的孩子个数(剩余入度)
int que[MAXN];              // 拓扑剥离用的队列
bool removed[MAXN];         // 是否已处理完(被剥掉, 或属于已处理的环)
ll dpFix[MAXN];             // dpFix[i]: i 变成自环(进入集合 S)时子树的最小代价
ll dpFree[MAXN];            // dpFree[i]: i 不进集合 S 时子树的最小代价
ll sumMin[MAXN];            // 已处理孩子 x 给父亲贡献的 min(dpFix[x], dpFree[x])
ll sumFix[MAXN];            // 已处理孩子 x 给父亲贡献的 dpFix[x]
ll cycFix[MAXN], cycFree[MAXN];  // 环上第 j 个点进/不进 S 时, 它挂着的树的代价

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &nxt[i]);
    for (int i = 1; i <= n; i++) scanf("%lld", &cost[i]);

    for (int i = 1; i <= n; i++) {
        // 本来就是自环的点直接满足 a[i] = i, 让它当"自环点"不花一分钱
        if (nxt[i] == i) cost[i] = 0;
        indeg[nxt[i]]++;
    }

    // 幂等等价于: 令 S 为修改后满足 a[i] = i 的点集, 则对每个 i 有 i 属于 S 或 a[i] 属于 S。
    // 非环部分是一棵棵内向树, 按孩子全部处理完的顺序(剩余入度为 0)自底向上 DP:
    //   孩子 x 的父亲 p = nxt[x], x 若不在 S, 则要求 p 在 S。
    int head = 0, tail = 0;
    for (int i = 1; i <= n; i++)
        if (indeg[i] == 0) que[tail++] = i;

    while (head < tail) {
        int x = que[head++];
        removed[x] = true;
        dpFix[x] = cost[x] + sumMin[x];     // x 在 S: 孩子们可以自由选择
        dpFree[x] = sumFix[x];              // x 不在 S: 所有孩子都必须在 S
        int p = nxt[x];
        sumMin[p] += min(dpFix[x], dpFree[x]);
        sumFix[p] += dpFix[x];
        if (--indeg[p] == 0) que[tail++] = p;
    }

    ll total = 0;
    for (int i = 1; i <= n; i++) {
        if (removed[i]) continue;

        // 剥离完之后剩下的点恰好都落在环上, 沿着 nxt 走一圈取出整个环
        int len = 0, v = i;
        do {
            cycFix[len] = cost[v] + sumMin[v];
            cycFree[len] = sumFix[v];
            len++;
            removed[v] = true;
            v = nxt[v];
        } while (v != i);

        if (len == 1) {
            // 自环点必须留在 S 里, 否则 a[v] = v 不在 S 中, 条件被破坏
            total += cycFix[0];
            continue;
        }

        // 环上的约束: 若第 j 个点不在 S, 则它的后继(第 j+1 个点, 模 len)必须在 S。
        // 枚举第 0 个点的两种状态, 线性扫一遍即可。
        ll best = INF;
        ll inS = cycFix[0], outS = INF;     // 情形一: 第 0 个点在 S 中
        for (int j = 1; j < len; j++) {
            ll nIn = min(inS, outS) + cycFix[j];
            ll nOut = inS + cycFree[j];
            inS = nIn;
            outS = nOut;
        }
        best = min(best, min(inS, outS));

        inS = INF, outS = cycFree[0];       // 情形二: 第 0 个点不在 S 中
        for (int j = 1; j < len; j++) {
            ll nIn = min(inS, outS) + cycFix[j];
            ll nOut = inS + cycFree[j];
            inS = nIn;
            outS = nOut;
        }
        // 情形二里第 0 个点不在 S, 因此最后一个点(它的前驱)必须在 S
        best = min(best, inS);

        total += best;
    }

    printf("%lld\n", total);
    return 0;
}
