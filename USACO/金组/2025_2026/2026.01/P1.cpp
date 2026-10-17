// P1 COW Traversals
// 正序视角下，"新开一场派对"会把一棵树劈成两棵，难以维护；
// 于是把询问离线倒序处理：派对只会被撤掉，树只会合并，用并查集即可 O(M α(N))。
#include <cstdio>

using namespace std;

const int MAXN = 200005;

int n, m;
int nx[MAXN];                 // nx[i]：奶牛 i 最好的朋友
int topQ[MAXN];               // topQ[i]：奶牛 i 在时间轴上最后一次开派对对应的询问编号（0 表示末尾时刻没有派对）
int curQ[MAXN];               // curQ[i]：倒序撤销到当前时刻时，奶牛 i 最近一次"还没被撤销"的询问编号
int prevQ[MAXN];              // prevQ[k]：同一头奶牛在询问 k 之前的上一次询问编号
int qCow[MAXN], qType[MAXN];  // 询问 k 开派对的奶牛、派对类型（0=C，1=O，2=W）

// 并查集：每个连通块是一棵以派对为根的树（若该块内没有派对，则是一环加挂着的树）
int par[MAXN], sizNum[MAXN], party[MAXN];
long long cnt[3];             // 当前时刻 C/O/W 三种类型各自吸引到的奶牛数

int findRoot(int x) {
    // 路径减半：迭代实现，避免递归深度过大
    while (par[x] != x) {
        par[x] = par[par[x]];
        x = par[x];
    }
    return x;
}

void unite(int a, int b) {
    a = findRoot(a);
    b = findRoot(b);
    if (a == b) return;
    // 沿 i -> nx[i] 合并，派对点只可能在父亲那一侧（也就是参数 b 所在的块）里出现，
    // 所以必须先把 party[b] 存下来：下面的按秩合并会交换 a、b。
    int pb = party[b];
    if (sizNum[a] < sizNum[b]) {
        int t = a;
        a = b;
        b = t;
    }
    par[b] = a;
    sizNum[a] += sizNum[b];
    if (pb) party[a] = pb;
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &nx[i]);
    scanf("%d", &m);
    for (int k = 1; k <= m; k++) {
        int c;
        char ch;
        scanf("%d %c", &c, &ch);  // 格式串里的空格会吃掉行内空白，%c 才不会读到换行
        int t = (ch == 'C' ? 0 : (ch == 'O' ? 1 : 2));
        qCow[k] = c;
        qType[k] = t;
        prevQ[k] = topQ[c];
        topQ[c] = k;
    }

    // 从最后一晚的状态出发：每头开过派对的奶牛此刻就开着最新那种类型的派对
    for (int i = 1; i <= n; i++) {
        par[i] = i;
        sizNum[i] = 1;
        curQ[i] = topQ[i];
        party[i] = (curQ[i] ? i : 0);
    }
    // 没开派对的奶牛沿着 nx 走，等价于把它并进"最好的朋友"所在的那棵树
    for (int i = 1; i <= n; i++)
        if (curQ[i] == 0) unite(i, nx[i]);
    for (int i = 1; i <= n; i++)
        if (curQ[i]) cnt[qType[curQ[i]]] += sizNum[findRoot(i)];

    long long res[MAXN][3];
    res[m][0] = cnt[0];
    res[m][1] = cnt[1];
    res[m][2] = cnt[2];

    for (int k = m; k >= 1; k--) {
        int c = qCow[k];
        // 撤销询问 k：奶牛 c 的派对退回到上一次的状态
        // 此时 c 仍开着类型 qType[k] 的派对，投奔它的奶牛恰好是它所在的整棵树
        long long sz = sizNum[findRoot(c)];
        cnt[qType[k]] -= sz;
        curQ[c] = prevQ[k];
        if (curQ[c] != 0) {
            // 派对还在，只是类型变回上一次的
            cnt[qType[curQ[c]]] += sz;
        } else {
            // 派对彻底消失：c 变成普通奶牛并指向 nx[c]，整棵树与被指向的那棵树合并
            party[findRoot(c)] = 0;  // 本块唯一的派对点没了
            unite(c, nx[c]);
            int np = party[findRoot(c)];
            // 合并后的块里若还有派对点 np，则这 sz 头奶牛改投奔 np 的派对
            if (np != 0 && curQ[np] != 0) cnt[qType[curQ[np]]] += sz;
        }
        res[k - 1][0] = cnt[0];
        res[k - 1][1] = cnt[1];
        res[k - 1][2] = cnt[2];
    }

    for (int i = 1; i <= m; i++)
        printf("%lld %lld %lld\n", res[i][0], res[i][1], res[i][2]);
    return 0;
}
