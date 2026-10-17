#include <cstdio>

using namespace std;

const int MAXN = 100005;
// 表示"该状态不可达"的极小值，取足够小以免与合法答案（最小可能为 0）混淆
const long long NEG = -1000000000000000LL;

int T, N, K;
int xs[MAXN], ys[MAXN];               // 全部奶牛，位置严格递增
int cx[MAXN], cy[MAXN], m;            // 当前正在处理的"链"内的奶牛
int ub[MAXN];                         // ub[i]：链内第一头与第 i 头牛位置差大于 K 的牛下标
long long dpEven[MAXN], dpOdd[MAXN];  // 后缀 DP，下标 0 / 1 表示未配对牛头数为偶数 / 奇数

// 归并：只有相邻两头牛位置差不超过 K 才能配对，所以按位置切成若干"链"。
// 不同链中的任意两头牛位置差都大于 K，既不能配对也不会互相影响最大性，可以各链独立求解。

// T=1：长度为 m（奇数）的链，最少只需要留下 1 头牛不配对
long long minChain() {
    int best = 1000000001;  // 体重不超过 10^4，用大于它的哨兵值表示"还没有候选"
    for (int i = 0; i < m; i++) {
        // i 为偶数时，它左边 i 头、右边 m-1-i 头都是偶数，各自相邻两两配对即可（m 为奇数）
        // i 为奇数时两侧各剩奇数头，必须让 i-1 与 i+1 直接配成一对，才能刚好把别的牛配完
        bool ok = (i % 2 == 0) ||
                  (i - 1 >= 0 && i + 1 < m && cx[i + 1] - cx[i - 1] <= K);
        if (ok && cy[i] < best) best = cy[i];
    }
    // 体重都是正数，留 1 头一定比留更多头更小
    return best;
}

// T=2：求这条链上所有合法方案中未配对奶牛体重和的最大值
long long maxChain() {
    dpEven[m] = 0;       // 后缀为空：0 头未配对，属于偶数情况
    dpOdd[m] = NEG;      // 空后缀不可能有奇数头未配对
    for (int i = m - 1; i >= 0; i--) {
        // 情况一：第 i 头牛参加了配对，未配对信息完全来自后缀 i+1
        dpEven[i] = dpEven[i + 1];
        dpOdd[i] = dpOdd[i + 1];
        // 情况二：让第 i 头牛落单。它必须满足链端、邻牛可配对、或后缀奇偶合适之一，
        // 否则剩下的牛无法两两配完（详见题解思路）
        bool endOK = (i == 0 || i == m - 1);
        bool midOK = (i - 1 >= 0 && i + 1 < m && cx[i + 1] - cx[i - 1] <= K);
        int q = (m - i) % 2;  // 后缀 i..m-1 的长度奇偶
        if (endOK || midOK || q == 0) {
            long long cand = dpOdd[ub[i]] + cy[i];
            if (cand > dpEven[i]) dpEven[i] = cand;
        }
        if (endOK || midOK || q == 1) {
            long long cand = dpEven[ub[i]] + cy[i];
            if (cand > dpOdd[i]) dpOdd[i] = cand;
        }
        // 注意 ub[i] 之后的牛才可能与 i 同时落单：位置差不超过 K 的两头牛都落单会违反最大性
    }
    // 配对的牛总是两头一组，所以未配对头数必须与链长同奇偶
    return (m % 2 == 1) ? dpOdd[0] : dpEven[0];
}

long long solveChain() {
    if (T == 1) return (m % 2 == 1) ? minChain() : 0;  // 偶数长度的链可以全部配完
    // 双指针求 ub[]：随着 i 增大 x_i + K 也增大，ub[i] 单调不减，整体 O(m)
    int p = 0;
    for (int i = 0; i < m; i++) {
        if (p < i + 1) p = i + 1;
        while (p < m && cx[p] - cx[i] <= K) p++;
        ub[i] = p;
    }
    return maxChain();
}

int main() {
    scanf("%d %d %d", &T, &N, &K);
    for (int i = 0; i < N; i++) scanf("%d %d", &xs[i], &ys[i]);

    long long ans = 0;
    m = 0;
    for (int i = 0; i < N; i++) {
        if (m > 0 && xs[i] - xs[i - 1] > K) {  // 相邻两头牛配不上，链在此断开
            ans += solveChain();
            m = 0;
        }
        cx[m] = xs[i];
        cy[m] = ys[i];
        m++;
    }
    if (m > 0) ans += solveChain();

    printf("%lld\n", ans);
    return 0;
}
