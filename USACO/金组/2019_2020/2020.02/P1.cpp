#include <cstdio>

using namespace std;

const int MAXN = 100005;
const int MAXC = 100005;

int N, M, C;
int S[MAXN];                                     // 每次挤奶的最早日期，会不断被松弛抬高
int inDeg[MAXN];                                 // 入度，用于 Kahn 拓扑排序
int head[MAXN], nxt[MAXC], to[MAXC], weight[MAXC], edgeCnt;
int queueArr[MAXN];                              // 手写队列（拓扑序）

void addEdge(int a, int b, int x) {
    to[edgeCnt] = b;
    weight[edgeCnt] = x;
    nxt[edgeCnt] = head[a];
    head[a] = edgeCnt++;
}

int main() {
    scanf("%d %d %d", &N, &M, &C);
    // M 只用来限定取值范围，答案保证不超过 M，所以读入后不需要再用
    for (int i = 1; i <= N; i++) {
        scanf("%d", &S[i]);
        head[i] = -1;
    }
    for (int i = 0; i < C; i++) {
        int a, b, x;
        scanf("%d %d %d", &a, &b, &x);
        addEdge(a, b, x);
        inDeg[b]++;
    }

    // 约束 S[b] >= S[a] + x 构成一张 DAG（有环则无解，题面保证有解）。
    // 按拓扑序松弛：处理到 a 时，所有能抬高 S[a] 的前驱都已经处理完，
    // 所以此时 S[a] 就是最终的最小值。
    int qh = 0, qt = 0;
    for (int i = 1; i <= N; i++) {
        if (inDeg[i] == 0) queueArr[qt++] = i;
    }
    while (qh < qt) {
        int v = queueArr[qh++];
        for (int e = head[v]; e != -1; e = nxt[e]) {
            int u = to[e];
            if (S[v] + weight[e] > S[u]) S[u] = S[v] + weight[e];
            if (--inDeg[u] == 0) queueArr[qt++] = u;
        }
    }

    for (int i = 1; i <= N; i++) printf("%d\n", S[i]);
    return 0;
}
