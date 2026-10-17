#include <cstdio>

using namespace std;

const int MAXA = 1000000;   // 石堆大小上限，同时是枚举首手拿走石子数 x 的上界

int cum[MAXA + 2];   // cum[v]：大小不超过 v 的石堆个数（前缀和）
int cnt[MAXA + 2];   // 当前 x 下，cnt[t]：满足 floor(ai / x) == t 的石堆个数

int main() {
    int n;
    scanf("%d", &n);
    int maxA = 0;
    for (int i = 0; i < n; ++i) {
        int a;
        scanf("%d", &a);
        ++cum[a];
        if (a > maxA) maxA = a;
    }
    for (int v = 1; v <= maxA; ++v) cum[v] += cum[v - 1];

    long long answer = 0;
    // 枚举贝茜首手拿走的石子数 x：只有 x <= maxA 才可能从某一堆里拿得出来
    for (int x = 1; x <= maxA; ++x) {
        int lim = maxA / x;   // floor(ai / x) 的最大可能取值，超过它的 t 不必考虑
        // cnt[t] = 大小落在 [x*t, x*(t+1)-1] 内的石堆数，用前缀和在 O(1) 内求出
        for (int t = 1; t <= lim; ++t) {
            int hi = x * (t + 1) - 1;
            if (hi > maxA) hi = maxA;
            cnt[t] = cum[hi] - cum[x * t - 1];
        }
        // 局面必败当且仅当每种大小的石堆数都是偶数（后手镜像即可获胜）。
        // 首手从商为 t 的堆里拿走 x 颗后，商 t 的个数减一、商 t-1 的个数加一，
        // 所以必胜条件是"奇数个数的位置恰好是 {1} 或相邻的 {o, o+1}"，
        // 其中出现平移的那一处就是 t；这里只需统计奇数位置的个数与最后两个位置
        int oddCount = 0, lastOdd = 0, prevOdd = 0;
        for (int t = 1; t <= lim; ++t) {
            if (cnt[t] & 1) {
                ++oddCount;
                prevOdd = lastOdd;
                lastOdd = t;
            }
        }
        if ((oddCount == 1 && lastOdd == 1) || (oddCount == 2 && prevOdd + 1 == lastOdd)) {
            // 商为 lastOdd 的每一堆都可以作为首手目标，逐堆计数
            answer += cnt[lastOdd];
        }
    }
    printf("%lld\n", answer);
    return 0;
}
