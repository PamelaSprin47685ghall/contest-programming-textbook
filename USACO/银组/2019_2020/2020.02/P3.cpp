// USACO 2020 February Contest, Silver P3: Clock Tree
#include <cstdio>

using namespace std;

const int MAXN = 2500;

int n;
int clockValue[MAXN + 1];
int head[MAXN + 1], nextEdge[2 * MAXN], toNode[2 * MAXN], edgeCount;
int depth[MAXN + 1]; // 以 1 号房间为根时每个房间的深度，-1 表示还没访问过
int bfsQueue[MAXN + 1];

void addEdge(int a, int b) {
    toNode[edgeCount] = b;
    nextEdge[edgeCount] = head[a];
    head[a] = edgeCount++;
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &clockValue[i]);
    }
    for (int i = 1; i <= n; ++i) {
        head[i] = -1;
    }
    for (int i = 0; i < n - 1; ++i) {
        int a, b;
        scanf("%d %d", &a, &b);
        addEdge(a, b);
        addEdge(b, a);
    }

    // 房间之间的通路是一棵树，用一次 BFS 就能定出所有房间到 1 号房间的距离
    for (int i = 1; i <= n; ++i) {
        depth[i] = -1;
    }
    int qHead = 0, qTail = 0;
    depth[1] = 0;
    bfsQueue[qTail++] = 1;
    while (qHead < qTail) {
        int x = bfsQueue[qHead++];
        for (int e = head[x]; e != -1; e = nextEdge[e]) {
            int y = toNode[e];
            if (depth[y] != -1) continue;
            depth[y] = depth[x] + 1;
            bfsQueue[qTail++] = y;
        }
    }

    long long evenSum = 0, oddSum = 0;
    int evenCount = 0, oddCount = 0;
    for (int i = 1; i <= n; ++i) {
        if (depth[i] % 2 == 0) {
            evenSum += clockValue[i];
            ++evenCount;
        } else {
            oddSum += clockValue[i];
            ++oddCount;
        }
    }

    // 从某间房出发时，量 q = 各房间按"到起点的距离奇偶"取正负号后的带符号时钟和（模 12）。
    // 沿一条边走进另一间房只会让时钟加一，而这一项的符号由它到起点的距离奇偶决定，
    // 所以每次移动 q 改变 ±1；要让最终所有时钟都指向 12（即 q ≡ 0），出发时 q 必须是 0 或 1。
    // 反过来 q ≡ 0 或 1 时总能构造：不断找一个叶子 r，反复走"起点→r→起点"把 r 的时钟拨到 12，
    // 之后不再进 r（等价于把 r 从树上删掉），归纳下去即可。
    // 换成别的房间 v 出发，等价于把每个符号都乘上 (-1)^{depth[v]}，
    // 即 q_0(v) = ±(evenSum - oddSum)：同奇偶层的房间结论相同，异奇偶层互为相反数。
    int r0 = (int)(evenSum % 12), r1 = (int)(oddSum % 12);
    int answer;
    if (r0 == r1) {
        answer = n;                                   // 差 ≡ 0：两类房间都可行
    } else if ((r0 + 1) % 12 == r1) {
        answer = oddCount;                            // 差 ≡ -1：只有奇数层房间可行
    } else if (r0 == (r1 + 1) % 12) {
        answer = evenCount;                           // 差 ≡ 1：只有偶数层房间可行
    } else {
        answer = 0;
    }
    printf("%d\n", answer);
    return 0;
}
