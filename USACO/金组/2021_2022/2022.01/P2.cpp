#include <cstdio>

using namespace std;

const int MAXN = 100005;
const int MAXQ = 200005;

int N, Q;

// 离线倒着做: 正向的"删边/停用农场"在倒序里变成"加边/激活农场", 便于用并查集维护.
// 相关性随时间倒流只会变多(倒序中已活跃的农场不会再变回不活跃),
// 所以某个农场第一次变得相关的那一刻, 就是它在正向里"最后一次相关"的时刻, 即答案.

int par[MAXN], sz[MAXN];
bool relevant[MAXN];  // 只对并查集根有意义: 该连通块里是否有活跃农场
int head[MAXN], tail[MAXN], nxtNode[MAXN];  // 尚未确定答案的农场链表
int answer[MAXN];

char type[MAXQ];   // 'D' / 'A' / 'R'
int arg1[MAXQ], arg2[MAXQ];

int edgeX[MAXQ], edgeY[MAXQ];  // 按加入顺序编号的边
int edgeCount;
bool removedEdge[MAXQ];  // 该边是否在正向里被 R 删过
int deactivateTime[MAXN];  // 0 表示从没被停用

int Find(int x) {
    while (par[x] != x) {
        par[x] = par[par[x]];
        x = par[x];
    }
    return x;
}

// 把 root 这一整块标记为相关, 块内还没定答案的农场在时刻 t 起开始相关
void Activate(int root, int t) {
    if (relevant[root]) return;
    relevant[root] = true;
    for (int u = head[root]; u != -1; u = nxtNode[u]) answer[u] = t;
    head[root] = -1;
    tail[root] = -1;
}

void Unite(int a, int b, int t) {
    int ra = Find(a), rb = Find(b);
    if (ra == rb) return;
    if (sz[ra] < sz[rb]) {
        int tmp = ra;
        ra = rb;
        rb = tmp;
    }
    // 一边相关一边不相关时, 不相关的那一块从此刻起也变相关
    if (relevant[ra] && !relevant[rb]) Activate(rb, t);
    else if (relevant[rb] && !relevant[ra]) Activate(ra, t);
    par[rb] = ra;
    sz[ra] += sz[rb];
    if (head[ra] == -1) {
        head[ra] = head[rb];
        tail[ra] = tail[rb];
    } else if (head[rb] != -1) {
        nxtNode[tail[ra]] = head[rb];
        tail[ra] = tail[rb];
    }
    head[rb] = -1;
    tail[rb] = -1;
}

int main() {
    scanf("%d %d", &N, &Q);
    for (int i = 0; i < N; ++i) {
        par[i] = i;
        sz[i] = 1;
        relevant[i] = false;
        head[i] = i;
        tail[i] = i;
        nxtNode[i] = -1;
        answer[i] = -1;
        deactivateTime[i] = 0;
    }
    par[N] = N;
    relevant[N] = false;
    for (int i = 1; i <= Q; ++i) {
        char t;
        scanf(" %c", &t);
        type[i] = t;
        if (t == 'D') {
            int x;
            scanf("%d", &x);
            --x;
            arg1[i] = x;
            deactivateTime[x] = i;
        } else if (t == 'A') {
            int x, y;
            scanf("%d %d", &x, &y);
            --x;
            --y;
            arg1[i] = x;
            arg2[i] = y;
            edgeX[edgeCount] = x;
            edgeY[edgeCount] = y;
            ++edgeCount;
        } else {
            int e;
            scanf("%d", &e);
            --e;
            arg1[i] = e;
            removedEdge[e] = true;
        }
    }
    // 端点: 第 Q 次更新之后的局面. 一直没被删掉的边此刻都还在.
    for (int e = 0; e < edgeCount; ++e) {
        if (!removedEdge[e]) Unite(edgeX[e], edgeY[e], Q);
    }
    for (int x = 0; x < N; ++x) {
        if (deactivateTime[x] == 0) Activate(Find(x), Q);
    }
    // 倒序撤销每次更新, 转移到的时刻是 i-1
    for (int i = Q; i >= 1; --i) {
        if (type[i] == 'R') {
            int e = arg1[i];
            Unite(edgeX[e], edgeY[e], i - 1);
        } else if (type[i] == 'D') {
            Activate(Find(arg1[i]), i - 1);
        }
        // 'A': 该边在时刻 i-1 还不存在, 但它两端点此刻都是活跃农场,
        //      留着它对"相关"的判断没有影响, 故无需处理.
    }
    // 时刻 0 时所有农场都活跃, 于是全都相关
    for (int x = 0; x < N; ++x) Activate(Find(x), 0);
    for (int x = 0; x < N; ++x) printf("%d\n", answer[x]);
    return 0;
}
