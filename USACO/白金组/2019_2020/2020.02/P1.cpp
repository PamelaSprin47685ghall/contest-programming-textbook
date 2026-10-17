// USACO 2020 February Contest, Platinum Problem 1: Delegation
// 题意：把一棵树的全部边划分成若干条路径（不同路径可以共用顶点），使得最短路径的长度尽量大，
// 求这个最大长度 K。做法：二分 K + 自底向上的树形 DP 判定可行性。
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 100005;

int N, K;
int head[MAXN], nxt[2 * MAXN], to[2 * MAXN], ecnt = 1; // 链式前向星，省去 vector 的开销
int par[MAXN], order[MAXN];                            // BFS 定序，倒序遍历即自底向上
int g[MAXN];      // g[x]：子树 x 中除悬空路径外全部合成长度 >= K 的路径时，悬空路径从 x 往下的
                  // 最大长度（0 表示悬空路径止于 x，即边 (x,par) 自己向上延伸），-1 表示无解
int buf[MAXN];    // 暂存某个结点所有孩子带来的路径长度

void addEdge(int u, int v) {
    to[ecnt] = v;
    nxt[ecnt] = head[u];
    head[u] = ecnt++;
}

// 排序后的 a[0..m-1] 中跳过下标 s1、s2（-1 表示不跳过）后剩下的元素个数为偶数，
// 判断它们能否两两配对且每对之和都不小于 K。两头配对（最小配最大）即最优：
// 若存在合法配对，把最小与次小的配对伙伴交换仍合法，归纳即得最小与最大配在一起也合法
bool canPair(int a[], int m, int s1, int s2) {
    int l = 0, r = m - 1;
    while (l < r) {
        if (l == s1 || l == s2) { l++; continue; }
        if (r == s1 || r == s2) { r--; continue; }
        if (a[l] + a[r] < K) return false;
        l++; r--;
    }
    return true;
}

// 判断孩子们能否全部"收尾"：每条孩子边要么单独成为一条长度 >= K 的完整路径，
// 要么与另一条孩子边在 x 处对接（两条长度之和 >= K）。
// 单独收尾的数与 m 同奇偶；而两条长度都 >= K 的路径总能并为一条合法路径，
// 所以至多需要一个孤点。孤点必须 >= K，取满足条件的最小者最划算——把大的元素留给配对，
// 留下的多重集在逐项比较下最大，配对最容易成功
bool canFinish(int a[], int m) {
    if (m % 2 == 0) return canPair(a, m, -1, -1); // 偶数个：全部两两对接
    int j = -1;
    for (int i = 0; i < m; ++i)
        if (a[i] >= K) { j = i; break; }
    if (j < 0) return false;
    return canPair(a, m, j, -1);
}

// 判断"让悬空路径往下钻进孩子 r"是否可行：此时 r 不再需要收尾，其余孩子必须能收尾
bool canFinishExcept(int a[], int m, int r) {
    if ((m - 1) % 2 == 0) return canPair(a, m, r, -1); // 其余孩子个数为偶数：全部对接
    int j = -1;                                        // 否则其余孩子中还要挑一个孤点
    for (int i = 0; i < m; ++i)
        if (i != r && a[i] >= K) { j = i; break; }
    if (j < 0) return false;
    return canPair(a, m, r, j);
}

bool feasible(int k) {
    K = k;
    for (int idx = N - 1; idx >= 0; --idx) {
        int x = order[idx];
        int m = 0;
        bool bad = false;
        for (int e = head[x]; e; e = nxt[e]) {
            int y = to[e];
            if (y == par[x]) continue;
            if (g[y] < 0) { bad = true; break; } // 子树已无解，整棵树必然无解
            int w = g[y] + 1;                    // 从 x 出发到那个端点的路径长度
            if (w > K) w = K;                    // 超过 K 的部分对判定不再有影响
            buf[m++] = w;
        }
        if (bad) { g[x] = -1; continue; }
        sort(buf, buf + m);
        if (x == 1) {
            // 根没有父边，不存在悬空路径，所有孩子边必须全部收尾
            g[x] = canFinish(buf, m) ? 0 : -1;
            continue;
        }
        int best = canFinish(buf, m) ? 0 : -1; // 方案一：悬空路径止于 x
        // 方案二：悬空路径钻进某个孩子 r，取其中能得到的最长悬空路径。
        // 被去掉的下标越靠前，剩下的多重集逐项越大、越容易合法，因此可行下标构成前缀，可二分
        int lo = -1, hi = m - 1;
        while (lo < hi) {
            int mid = (lo + hi + 1) / 2;
            if (canFinishExcept(buf, m, mid)) lo = mid; else hi = mid - 1;
        }
        if (lo >= 0 && buf[lo] > best) best = buf[lo];
        g[x] = best;
    }
    return g[1] >= 0; // 根是结点 1
}

int main() {
    if (scanf("%d", &N) != 1) return 0;
    for (int i = 1; i < N; ++i) {
        int a, b;
        scanf("%d %d", &a, &b);
        addEdge(a, b);
        addEdge(b, a);
    }
    par[1] = 0;
    order[0] = 1;
    int cnt = 1;
    for (int i = 0; i < cnt; ++i) {
        int x = order[i];
        for (int e = head[x]; e; e = nxt[e]) {
            int y = to[e];
            if (y == par[x]) continue;
            par[y] = x;
            order[cnt++] = y;
        }
    }
    int lo = 1, hi = N - 1; // K = 1 总可行：每条边单独成路
    while (lo < hi) {
        int mid = (lo + hi + 1) / 2;
        if (feasible(mid)) lo = mid; else hi = mid - 1;
    }
    printf("%d\n", lo);
    return 0;
}
