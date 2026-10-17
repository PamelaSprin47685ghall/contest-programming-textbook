#include <cstdio>

using namespace std;

const int MAXN = 200005;
const int INF = 1000000000;

int n, m;

// 左端点恰为 a 的所有照片中, 最大 / 最小的右端点
int lastB[MAXN];   // 没有照片时记 0, 表示"没有约束"
int firstB[MAXN];  // 没有照片时记 INF

// maxRight[j] = 所有 a <= j 的照片中最大的 b.
// 若上一头斑点牛在 j, 那么下一头斑点牛 i 必须满足 i > maxRight[j];
// 否则那张 a <= j 且 b >= i 的照片会同时包含 j 与 i 两头斑点牛.
int maxRight[MAXN];

// minRight[j] = 所有 a > j 的照片中最小的 b.
// 若上一头斑点牛在 j, 那么下一头斑点牛 i 必须满足 i <= minRight[j];
// 否则那张 a > j 且 b < i 的照片整个落在 (j, i) 内部, 一头斑点牛也没有.
// 两条约束合起来说明: 对固定的 i, 合法的 j 是连续的一段区间, 且两端随 i 单调右移.
int minRight[MAXN];

// dp[i] = 以 i 作为"当前最后一头斑点牛"时, 前 i 头牛里斑点牛的最大数量; -1 表示不可达
int dp[MAXN];

// 单调队列, 存放下标, 使对应 dp 值单调递减
int que[MAXN];

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; i++) {
        lastB[i] = 0;
        firstB[i] = INF;
    }

    int maxLeft = 0;  // 所有照片左端点的最大值: 最后一头斑点牛必须不早于它
    for (int k = 0; k < m; k++) {
        int a, b;
        scanf("%d %d", &a, &b);
        if (b > lastB[a]) lastB[a] = b;
        if (b < firstB[a]) firstB[a] = b;
        if (a > maxLeft) maxLeft = a;
    }

    // 前缀最大值 (单调不降)
    maxRight[0] = 0;
    for (int j = 1; j <= n; j++) {
        maxRight[j] = maxRight[j - 1];
        if (lastB[j] > maxRight[j]) maxRight[j] = lastB[j];
    }
    // 后缀最小值 (单调不降)
    minRight[n] = INF;
    for (int j = n - 1; j >= 0; j--) {
        minRight[j] = minRight[j + 1];
        if (firstB[j + 1] < minRight[j]) minRight[j] = firstB[j + 1];
    }

    for (int i = 0; i <= n; i++) dp[i] = -1;
    dp[0] = 0;  // 哨兵: 还没放任何斑点牛

    int hi = -1;             // 已经放进单调队列的最大下标
    int lo = 0;              // 窗口左端, 比它更小的 j 永远不会再合法
    int head = 0, tail = 0;  // 队列占据 [head, tail)

    for (int i = 1; i <= n; i++) {
        // 淘汰右端点过小的 j; minRight 单调不降, 所以从左往右删即可
        // 注意不能写成 lo <= hi: 此时窗口内部可能还残留尚未处理的非法下标,
        // 必须让 lo 一步到位地越过所有 minRight[j] < i 的 j
        while (minRight[lo] < i) lo++;
        while (tail > head && que[head] < lo) head++;
        // 加入新下标: 只有满足 maxRight[j] <= i - 1 的 j 才可能与 i 配对;
        // maxRight 单调不降, 所以这一步也是单调的
        while (hi + 1 <= i - 1 && maxRight[hi + 1] <= i - 1) {
            hi++;
            if (dp[hi] >= 0 && hi >= lo) {
                while (tail > head && dp[que[tail - 1]] <= dp[hi]) tail--;
                que[tail] = hi;
                tail++;
            }
        }
        if (tail > head) dp[i] = dp[que[head]] + 1;
    }

    int answer = -1;
    for (int i = maxLeft; i <= n; i++) {
        if (dp[i] > answer) answer = dp[i];
    }
    printf("%d\n", answer);
    return 0;
}
