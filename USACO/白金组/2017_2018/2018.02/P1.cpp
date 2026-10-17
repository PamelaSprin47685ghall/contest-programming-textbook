#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 100000;
const int MAXC = 2 * MAXN; // 离散化后最多有 N 个弹弓 y 加 M 个询问 b 这么多种取值
const long long NO_SHOT = -(1LL << 60); // 前缀最大值里的"空"标记，比任何真实代价都小

int n, m;
long long shotX[MAXN], shotY[MAXN], shotT[MAXN]; // 每台弹弓的发射点、落点与发射耗时
long long askA[MAXN], askB[MAXN];                 // 每个询问的起点与终点
int shotOrder[MAXN], askOrder[MAXN];              // 按 x、按 a 排好序的下标
long long answer[MAXN];

long long sortedY[MAXC]; // 弹弓的 y 与询问的 b 全部收集起来做离散化
int yCount = 0;              // 去重后有多少种不同的 y
int rankY[MAXN];             // 弹弓 y 的离散化名次（从 1 开始）
int askLow[MAXN];            // 询问 b 对应"名次不超过它的最大名次"
int askHigh[MAXN];           // 询问 b 对应"名次不小于它的最小名次"

long long prefixTree[MAXC + 2]; // 记录 y 名次 <= 询问 b 的弹弓里最好用的那个值
long long suffixTree[MAXC + 2]; // 记录 y 名次 >= 询问 b 的弹弓：下标倒过来存，用前缀最大值实现

void clearTrees() {
    for (int i = 1; i <= yCount; ++i) {
        prefixTree[i] = NO_SHOT;
        suffixTree[i] = NO_SHOT;
    }
}

// 树状数组：让下标 pos 存储的值变成"原值与 value 中较大的那个"
void updateMax(long long tree[], int pos, long long value) {
    for (int i = pos; i <= yCount; i += i & -i) {
        if (value > tree[i]) tree[i] = value;
    }
}

// 树状数组：问下标 1 到 pos 里的最大值
long long queryMax(long long tree[], int pos) {
    long long best = NO_SHOT;
    for (int i = pos; i > 0; i -= i & -i) {
        if (tree[i] > best) best = tree[i];
    }
    return best;
}

bool byShotX(int a, int b) { return shotX[a] < shotX[b]; }
bool byAskA(int a, int b) { return askA[a] < askA[b]; }

int main() {
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n; ++i) {
        scanf("%lld%lld%lld", &shotX[i], &shotY[i], &shotT[i]);
        sortedY[yCount++] = shotY[i];
    }
    for (int i = 0; i < m; ++i) {
        scanf("%lld%lld", &askA[i], &askB[i]);
        sortedY[yCount++] = askB[i];
    }

    // 弹弓的 y 与询问的 b 一起排序去重，之后"y 与 b 的大小关系"就能用名次比较
    sort(sortedY, sortedY + yCount);
    int keep = 0;
    for (int i = 0; i < yCount; ++i) {
        if (i == 0 || sortedY[i] != sortedY[i - 1]) sortedY[keep++] = sortedY[i];
    }
    yCount = keep;

    for (int i = 0; i < n; ++i) {
        rankY[i] = (int)(lower_bound(sortedY, sortedY + yCount, shotY[i]) - sortedY) + 1;
    }
    for (int i = 0; i < m; ++i) {
        // upper_bound 给出"名次不超过 b"的个数，lower_bound 给出"第一个不小于 b 的名次"
        askLow[i] = (int)(upper_bound(sortedY, sortedY + yCount, askB[i]) - sortedY);
        askHigh[i] = (int)(lower_bound(sortedY, sortedY + yCount, askB[i]) - sortedY) + 1;
        long long direct = askA[i] - askB[i];
        if (direct < 0) direct = -direct;
        answer[i] = direct; // 不用弹弓时的代价
    }

    for (int i = 0; i < n; ++i) shotOrder[i] = i;
    for (int i = 0; i < m; ++i) askOrder[i] = i;
    sort(shotOrder, shotOrder + n, byShotX);
    sort(askOrder, askOrder + m, byAskA);

    // 第一遍只考虑 x <= a 的弹弓，此时 |a-x| = a-x。按 y <= b、y >= b 分别化简：
    //   y <= b：代价 = (a+b) - (x+y-t)，要记住最大的 x+y-t
    //   y >= b：代价 = (a-b) - (x-y-t)，要记住最大的 x-y-t
    // 询问按 a 从小到大处理，处理到询问 q 时树状数组里恰好是 x <= a 的弹弓
    clearTrees();
    int inserted = 0;
    for (int i = 0; i < m; ++i) {
        int q = askOrder[i];
        while (inserted < n && shotX[shotOrder[inserted]] <= askA[q]) {
            int s = shotOrder[inserted];
            updateMax(prefixTree, rankY[s], shotX[s] + shotY[s] - shotT[s]);
            updateMax(suffixTree, yCount + 1 - rankY[s], shotX[s] - shotY[s] - shotT[s]);
            ++inserted;
        }
        long long byLow = askA[q] + askB[q] - queryMax(prefixTree, askLow[q]);
        long long byHigh = askA[q] - askB[q] - queryMax(suffixTree, yCount + 1 - askHigh[q]);
        if (byLow < answer[q]) answer[q] = byLow;
        if (byHigh < answer[q]) answer[q] = byHigh;
    }

    // 第二遍只考虑 x >= a 的弹弓，此时 |a-x| = x-a，询问按 a 从大到小处理：
    //   y <= b：代价 = (b-a) - (-x+y-t)，要记住最大的 -x+y-t
    //   y >= b：代价 = (-a-b) - (-x-y-t)，要记住最大的 -x-y-t
    clearTrees();
    inserted = n - 1;
    for (int i = m - 1; i >= 0; --i) {
        int q = askOrder[i];
        while (inserted >= 0 && shotX[shotOrder[inserted]] >= askA[q]) {
            int s = shotOrder[inserted];
            updateMax(prefixTree, rankY[s], -shotX[s] + shotY[s] - shotT[s]);
            updateMax(suffixTree, yCount + 1 - rankY[s], -shotX[s] - shotY[s] - shotT[s]);
            --inserted;
        }
        long long byLow = askB[q] - askA[q] - queryMax(prefixTree, askLow[q]);
        long long byHigh = -askA[q] - askB[q] - queryMax(suffixTree, yCount + 1 - askHigh[q]);
        if (byLow < answer[q]) answer[q] = byLow;
        if (byHigh < answer[q]) answer[q] = byHigh;
    }

    for (int i = 0; i < m; ++i) printf("%lld\n", answer[i]);
    return 0;
}
