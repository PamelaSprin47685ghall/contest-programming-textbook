#include <cstdio>

using namespace std;

const int MAXN = 1005;
const int MAXM = 1005;
const int INF = 1000000000;

// 链式前向星存双向管道
int head[MAXN], nxt[2 * MAXM], to[2 * MAXM], costE[2 * MAXM], flowE[2 * MAXM], edgeCnt;
int N, M;
int flowList[MAXM];
int dist[MAXN];

// 手写小根堆（按入堆时的距离比较，允许同一个点多次入堆）
int heapNode[4 * MAXM], heapKey[4 * MAXM], heapSize;

void addEdge(int a, int b, int c, int f) {
    to[edgeCnt] = b;
    costE[edgeCnt] = c;
    flowE[edgeCnt] = f;
    nxt[edgeCnt] = head[a];
    head[a] = edgeCnt++;
}

void heapPush(int v, int key) {
    int i = heapSize++;
    heapNode[i] = v;
    heapKey[i] = key;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (heapKey[p] <= heapKey[i]) break;
        int tn = heapNode[p], tk = heapKey[p];
        heapNode[p] = heapNode[i];
        heapKey[p] = heapKey[i];
        heapNode[i] = tn;
        heapKey[i] = tk;
        i = p;
    }
}

int heapPopNode() {
    int top = heapNode[0];
    heapSize--;
    heapNode[0] = heapNode[heapSize];
    heapKey[0] = heapKey[heapSize];
    int i = 0;
    while (true) {
        int l = 2 * i + 1, r = 2 * i + 2, s = i;
        if (l < heapSize && heapKey[l] < heapKey[s]) s = l;
        if (r < heapSize && heapKey[r] < heapKey[s]) s = r;
        if (s == i) break;
        int tn = heapNode[s], tk = heapKey[s];
        heapNode[s] = heapNode[i];
        heapKey[s] = heapKey[i];
        heapNode[i] = tn;
        heapKey[i] = tk;
        i = s;
    }
    return top;
}

// 只允许流量不小于 flowMin 的管道参与，求 1 到 N 的最小花费和；不可达返回 INF
int dijkstra(int flowMin) {
    for (int i = 1; i <= N; i++) dist[i] = INF;
    heapSize = 0;
    dist[1] = 0;
    heapPush(1, 0);
    while (heapSize > 0) {
        int key = heapKey[0];
        int v = heapPopNode();
        if (key > dist[v]) continue;    // 过期的堆条目直接扔掉
        if (v == N) return dist[N];     // 出堆时该点的距离已经最终确定
        for (int e = head[v]; e != -1; e = nxt[e]) {
            if (flowE[e] < flowMin) continue;
            int u = to[e];
            if (dist[v] + costE[e] < dist[u]) {
                dist[u] = dist[v] + costE[e];
                heapPush(u, dist[u]);
            }
        }
    }
    return INF;
}

int main() {
    scanf("%d %d", &N, &M);
    for (int i = 1; i <= N; i++) head[i] = -1;
    for (int i = 0; i < M; i++) {
        int a, b, c, f;
        scanf("%d %d %d %d", &a, &b, &c, &f);
        addEdge(a, b, c, f);
        addEdge(b, a, c, f);
        flowList[i] = f;
    }

    // 最优路径的瓶颈流量一定等于某条边的流量，所以只需枚举这些流量作为下限：
    // 对每种下限取“下限 / 此时的最短路花费”的最大值，用交叉相乘比较避免浮点误差。
    long long bestNum = 0, bestDen = 1;
    for (int i = 0; i < M; i++) {
        long long num = flowList[i];
        long long den = dijkstra(flowList[i]);
        if (den == INF) continue;
        if (num * bestDen > bestNum * den) {
            bestNum = num;
            bestDen = den;
        }
    }
    // 此时 bestNum / bestDen 恰好等于最优比值，整数除法即下取整
    printf("%lld\n", bestNum * 1000000LL / bestDen);
    return 0;
}
