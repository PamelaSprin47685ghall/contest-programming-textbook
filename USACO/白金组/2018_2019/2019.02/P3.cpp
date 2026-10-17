#include <cstdio>
#include <algorithm>
#include <vector>

using namespace std;

const int MAXN = 200005; // 花的数量上限（另外再加上终点 (T,T)）
const long long INF = 1000000000000000000LL;

int N;           // 点数：读入的花数加 1（终点 (T,T) 也算一个点）
int T;           // 草地边长
int px[MAXN];    // 点的 x 坐标
int py[MAXN];    // 点的 y 坐标
int byX[MAXN];   // 按 x 递增排好序的点编号
long long dp[MAXN]; // 走到该点时，所选矩形面积和的最小值

// levels[k] 就是第 k 层；同一层内按 x 递增排列时 y 必然递减
// （否则 x、y 都更大的那个点层号会更大），这个性质下面反复用到。
vector<int> levels[MAXN + 2];

bool cmpByX(int a, int b) {
    return px[a] < px[b];
}

// 用类似 LIS 的耐心排序给每个点定层：
// 层号 = 以该点结尾的最长上升链长度（下标从 0 开始）。
// 因为每层按 x 递增读入，levels[k].back() 就是该层里 y 最小的点，
// 所以"第 k 层里有没有 y 更小的点"可以直接拿它判断。
void computeLIS() {
    levels[0].push_back(byX[0]);
    int maxLevel = 0;
    for (int i = 1; i < N; ++i) {
        int cur = byX[i];
        int low = -1, high = maxLevel;
        while (low != high) {
            int mid = (low + high + 1) / 2;
            if (py[levels[mid].back()] < py[cur]) low = mid;
            else high = mid - 1;
        }
        levels[low + 1].push_back(cur);
        if (low + 1 > maxLevel) maxLevel = low + 1;
    }
}

// 从 from 接着走到 to 所新增的矩形面积
long long transCost(int from, int to) {
    return dp[from] + (long long)px[from] * py[from]
           - (long long)px[from] * py[to] - (long long)py[from] * px[to]
           + (long long)px[to] * py[to];
}

// a、b 是第 l 层里的两个候选点且 x[a] < x[b]。
// 同层内 y 随 x 递减，两个候选的差 cost(a,p)-cost(b,p) 在按 x 递增扫过第 l 层时
// 单调不增，所以 a 一旦反超 b 就永远更优；二分出这个最早的下标。
int firstOvertake(int l, int a, int b) {
    int low = 0, high = (int)levels[l].size();
    while (low != high) {
        int mid = (low + high) / 2;
        if (transCost(a, levels[l][mid]) < transCost(b, levels[l][mid])) high = mid;
        else low = mid + 1;
    }
    return low;
}

// 第 l 层里能支配 q（x、y 都更大）的第一个 / 最后一个点，没有则返回 -1。
// 能支配 q 的点在这一层里是连续一段，所以只要二分出 x 或 y 的临界位置再验证。
int findFirstDom(int l, int q) {
    int low = 0, high = (int)levels[l].size() - 1;
    while (low < high) {
        int mid = (low + high) / 2;
        if (px[levels[l][mid]] > px[q]) high = mid;
        else low = mid + 1;
    }
    if (px[levels[l][low]] > px[q] && py[levels[l][low]] > py[q]) return low;
    return -1;
}

int findLastDom(int l, int q) {
    int low = 0, high = (int)levels[l].size() - 1;
    while (low < high) {
        int mid = (low + high + 1) / 2;
        if (py[levels[l][mid]] > py[q]) low = mid;
        else high = mid - 1;
    }
    if (px[levels[l][low]] > px[q] && py[levels[l][low]] > py[q]) return low;
    return -1;
}

int que[MAXN];      // 单调队列：按 x 递增（或递减）排列的候选点
int overtakeAt[MAXN]; // overtakeAt[k]：que[k+1] 反超 que[k] 的下一层下标
vector<int> useful;   // 前一层里"还能支配某些点"的点，按 x 递增
int firstDom[MAXN]; // useful[i] 能支配的下一层点的最小下标
int lastDom[MAXN];  // useful[i] 能支配的下一层点的最大下标

// 处理"支配区间在 [pBegin, pEnd] 内开始"的那批候选点：
// 扫描的下一层点全部落在 [pBegin, pEnd] 中，而每个候选点的支配区间都是从
// 这段区间内部开始的，所以在整段扫描里它们始终有效，不用考虑区间到期，
// 只要按 x 递增依次入单调队列，队尾一旦不可能再更优就弹掉。
void sweepFromStart(int l, int qBegin, int qEnd, int pBegin, int pEnd) {
    int len = 0;
    int i = qBegin;
    for (int j = pBegin; j <= pEnd; ++j) {
        int p = levels[l + 1][j];
        while (i <= qEnd && firstDom[i] <= j) {
            while (len >= 2 && overtakeAt[len - 2] <= firstOvertake(l + 1, que[len - 1], useful[i])) --len;
            que[len] = useful[i];
            if (len >= 1) overtakeAt[len - 1] = firstOvertake(l + 1, que[len - 1], useful[i]);
            ++len;
            ++i;
        }
        // 队尾的点在大下标处更优，一旦被前一个点反超就再也没用了
        while (len >= 2 && overtakeAt[len - 2] <= j) --len;
        long long cand = transCost(que[len - 1], p);
        if (cand < dp[p]) dp[p] = cand;
    }
}

// 处理"支配区间在 [pBegin, pEnd] 内结束"的那批候选点：从右往左扫。
// 这段区间左端的点全部小于等于所有候选点的支配区间起点，所以不会"迟到"，
// 只有支配区间结束造成的失效——于是按 x 递减入队，
// 队尾候选在大下标处更优，一旦它在当前下标还没反超前一个就直接弹掉。
void sweepToEnd(int l, int qBegin, int qEnd, int pBegin, int pEnd) {
    int len = 0;
    int i = qEnd;
    for (int j = pEnd; j >= pBegin; --j) {
        int p = levels[l + 1][j];
        while (i >= qBegin && lastDom[i] >= j) {
            while (len >= 2 && overtakeAt[len - 2] >= firstOvertake(l + 1, useful[i], que[len - 1])) --len;
            que[len] = useful[i];
            if (len >= 1) overtakeAt[len - 1] = firstOvertake(l + 1, useful[i], que[len - 1]);
            ++len;
            --i;
        }
        while (len >= 2 && overtakeAt[len - 2] > j) --len;
        long long cand = transCost(que[len - 1], p);
        if (cand < dp[p]) dp[p] = cand;
    }
}

int main() {
    int flowerCount;
    scanf("%d %d", &flowerCount, &T);
    for (int i = 0; i < flowerCount; ++i) {
        scanf("%d %d", &px[i], &py[i]);
    }
    // 终点 (T,T) 必须被经过，把它当成第 flowerCount 个点一起处理
    px[flowerCount] = py[flowerCount] = T;
    N = flowerCount + 1;
    for (int i = 0; i < N; ++i) {
        byX[i] = i;
        dp[i] = INF;
    }
    sort(byX, byX + N, cmpByX);
    computeLIS();

    // 第 0 层的点前面只有 (0,0)，代价就是 x*y
    for (int i = 0; i < (int)levels[0].size(); ++i) {
        int cur = levels[0][i];
        dp[cur] = (long long)px[cur] * py[cur];
    }

    for (int l = 0; (int)levels[l + 1].size() > 0; ++l) {
        // 先扔掉下一层根本没人能支配的点：它们不可能出现在最长链上
        useful.clear();
        for (int i = 0; i < (int)levels[l].size(); ++i) {
            int cur = levels[l][i];
            int fd = findFirstDom(l + 1, cur);
            if (fd != -1) {
                firstDom[useful.size()] = fd;
                lastDom[useful.size()] = findLastDom(l + 1, cur);
                useful.push_back(cur);
            }
        }
        // 把相邻点切成段：每段内"支配区间起点 ≤ 本段第一个点的区间终点"，
        // 这样每段的候选区间起点都不会落在段外，一个区间最多横跨两段。
        for (int i = 0; i < (int)useful.size(); ) {
            int iEnd = i;
            while (iEnd + 1 < (int)useful.size() && firstDom[iEnd + 1] <= lastDom[i]) ++iEnd;
            sweepFromStart(l, i, iEnd, firstDom[i], lastDom[i]);
            if (lastDom[iEnd] >= lastDom[i] + 1) {
                sweepToEnd(l, i + 1, iEnd, lastDom[i] + 1, lastDom[iEnd]);
            }
            i = iEnd + 1;
        }
    }

    printf("%lld\n", dp[N - 1]);
    return 0;
}
