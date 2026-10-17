#include <cstdio>

using namespace std;

const int MAXN = 100005;

int N;
int head[MAXN], nxt[2 * MAXN], to[2 * MAXN], edgeCnt;

int parentArr[MAXN], subSize[MAXN], orderArr[MAXN], stackArr[MAXN];

// 对每个点，记录「删掉它之后各连通块的大小」，用链式链表存
int numHead[MAXN], numNxt[2 * MAXN], numVal[2 * MAXN], numCnt;

// 配对计数器：pairCnt[z] 是当前点尚未配对的「余数 z」个数，每次判 K 前整体清空
int pairCnt[MAXN];
char ansBuf[MAXN];

void addEdge(int a, int b) {
    to[edgeCnt] = b;
    nxt[edgeCnt] = head[a];
    head[a] = edgeCnt++;
}

void addNum(int v, int sz) {
    numVal[numCnt] = sz;
    numNxt[numCnt] = numHead[v];
    numHead[v] = numCnt++;
}

// 判断能否把全部道路划分成若干条长度恰为 K 的路径
bool canPartition(int K) {
    if ((N - 1) % K != 0) return false;          // 边数必须是 K 的倍数
    for (int i = 0; i < K; i++) pairCnt[i] = 0;
    for (int v = 1; v <= N; v++) {
        int leftOver = 0;
        for (int e = numHead[v]; e != -1; e = numNxt[e]) {
            int z = numVal[e] % K;
            if (z == 0) continue;                // 该方向恰好是整数条完整路径，无需配对
            // 余数 z 只可能和 K-z 配成一条穿过 v 的长度 K 的路径，
            // 所以贪心：能配就配，配不上就先留着。
            if (pairCnt[K - z] > 0) {
                pairCnt[K - z]--;
                leftOver--;
            } else {
                pairCnt[z]++;
                leftOver++;
            }
        }
        if (leftOver != 0) return false;         // 有配不掉的残余，说明这条路划分不可行
    }
    return true;
}

int main() {
    scanf("%d", &N);
    for (int i = 1; i <= N; i++) {
        head[i] = -1;
        numHead[i] = -1;
    }
    for (int i = 0; i < N - 1; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        addEdge(a, b);
        addEdge(b, a);
    }

    // 以 1 为根：用显式栈做先序遍历（N 可达 1e5，递归会爆栈），
    // 再逆着先序累加得到子树大小。
    int top = 0, len = 0;
    parentArr[1] = 0;
    stackArr[top++] = 1;
    while (top > 0) {
        int v = stackArr[--top];
        orderArr[len++] = v;
        for (int e = head[v]; e != -1; e = nxt[e]) {
            int u = to[e];
            if (u == parentArr[v]) continue;
            parentArr[u] = v;
            stackArr[top++] = u;
        }
    }
    for (int i = 1; i <= N; i++) subSize[i] = 1;
    for (int i = N - 1; i >= 1; i--) {
        subSize[parentArr[orderArr[i]]] += subSize[orderArr[i]];
    }

    // 每个点各方向连通块的大小：子节点方向是子树大小，父方向是 N - subSize[v]
    for (int v = 1; v <= N; v++) {
        for (int e = head[v]; e != -1; e = nxt[e]) {
            int u = to[e];
            if (u == parentArr[v]) continue;
            addNum(v, subSize[u]);
        }
        if (v != 1) addNum(v, N - subSize[v]);
    }

    for (int K = 1; K <= N - 1; K++) {
        ansBuf[K - 1] = canPartition(K) ? '1' : '0';
    }
    ansBuf[N - 1] = 0;
    printf("%s\n", ansBuf);
    return 0;
}
