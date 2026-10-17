#include <cstdio>

using namespace std;

const int MAXN = 100005;
const int MAXM = 100005;

int N, M;
int T[MAXN];                                    // 每头牛的牛奶类型
int head[MAXN], nxt[2 * MAXN], to[2 * MAXN], edgeCnt;

int qa[MAXM], qb[MAXM], qc[MAXM];               // 询问的端点与喜欢的类型
int qhp[MAXN], qnx[2 * MAXM], qid[2 * MAXM], qcnt;
bool okAns[MAXM];
char outBuf[MAXM + 1];

int par[MAXN], dep[MAXN], orderArr[MAXN], preOrder[MAXN], sz[MAXN];
int stackArr[MAXN];

// 第一遍 DFS（显式栈，避免递归过深）：求先序编号与子树大小，用于 O(1) 判断祖先关系
void buildOrder() {
    int top = 0;
    stackArr[top++] = 1;
    par[1] = 0;
    dep[1] = 0;
    int cnt = 0;
    while (top > 0) {
        int v = stackArr[--top];
        preOrder[v] = cnt;
        orderArr[cnt] = v;
        cnt++;
        for (int e = head[v]; e != -1; e = nxt[e]) {
            int u = to[e];
            if (u == par[v]) continue;
            par[u] = v;
            dep[u] = dep[v] + 1;
            stackArr[top++] = u;
        }
    }
    for (int v = 1; v <= N; v++) sz[v] = 1;
    // 逆着先序累加，保证先处理完子树
    for (int i = N - 1; i >= 1; i--) sz[par[orderArr[i]]] += sz[orderArr[i]];
}

// a 是否是 b 的祖先（含 a == b）
bool isAnc(int a, int b) {
    return preOrder[a] <= preOrder[b] && preOrder[b] < preOrder[a] + sz[a];
}

void addQuery(int v, int id) {
    qid[qcnt] = id;
    qnx[qcnt] = qhp[v];
    qhp[v] = qcnt++;
}

// 路径上从根到当前点方向的下一个点
int pathOrd[MAXN];
// 每种类型在“根到当前点”路径上最深点的记录：用链表式栈，进入时串起来、离开时还原
int storTop[MAXN], prevSame[MAXN];
int stNode[MAXN + 5], stState[MAXN + 5];

int main() {
    scanf("%d %d", &N, &M);
    for (int i = 1; i <= N; i++) scanf("%d", &T[i]);
    for (int i = 1; i <= N; i++) {
        head[i] = -1;
        qhp[i] = -1;
    }
    for (int i = 0; i < N - 1; i++) {
        int x, y;
        scanf("%d %d", &x, &y);
        to[edgeCnt] = y; nxt[edgeCnt] = head[x]; head[x] = edgeCnt++;
        to[edgeCnt] = x; nxt[edgeCnt] = head[y]; head[y] = edgeCnt++;
    }
    for (int i = 0; i < M; i++) {
        scanf("%d %d %d", &qa[i], &qb[i], &qc[i]);
        addQuery(qa[i], i);
        addQuery(qb[i], i);
    }

    buildOrder();

    // 第二遍 DFS：维护根到当前点的路径，以及在每个端点上把询问“问一遍”
    int pathLen = 0;
    int stTop = 0;
    stNode[stTop] = 1; stState[stTop] = 0; stTop++;
    while (stTop > 0) {
        int v = stNode[stTop - 1];
        if (stState[stTop - 1] == 0) {
            stState[stTop - 1] = 1;
            int t = T[v];
            prevSame[v] = storTop[t];
            storTop[t] = v;
            pathOrd[pathLen++] = v;

            // 本点作为询问端点：取该类型在根路径上最深的点 y
            for (int e = qhp[v]; e != -1; e = qnx[e]) {
                int id = qid[e];
                if (okAns[id]) continue;
                int y = storTop[qc[id]];
                if (y == 0) continue;               // 根路径上没有该类型，路径上更不会有
                if (y == v) {                       // 端点自己就是该类型
                    okAns[id] = true;
                    continue;
                }
                int other = qa[id] + qb[id] - v;
                int down = pathOrd[dep[y] + 1];     // y 往 v 方向的儿子
                // down 不是 other 的祖先说明两端点分居 y 的不同儿子子树，
                // 于是 y 落在 v 到 LCA 的这段路径上，答案为 1
                if (!isAnc(down, other)) okAns[id] = true;
            }

            for (int e = head[v]; e != -1; e = nxt[e]) {
                int u = to[e];
                if (u == par[v]) continue;
                stNode[stTop] = u; stState[stTop] = 0; stTop++;
            }
        } else {
            storTop[T[v]] = prevSame[v];
            pathLen--;
            stTop--;
        }
    }

    for (int i = 0; i < M; i++) outBuf[i] = okAns[i] ? '1' : '0';
    outBuf[M] = '\0';
    printf("%s\n", outBuf);
    return 0;
}
