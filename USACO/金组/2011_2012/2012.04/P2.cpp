#include <cstdio>
#include <set>

using namespace std;

const int MAXN = 100000;

int n;
long long limit;
long long h[MAXN + 1];  // 书本高度
long long w[MAXN + 1];  // 书本宽度
long long dp[MAXN + 1]; // dp[i]: 放好前 i 本书的最小总高度

// 单调栈把当前窗口内所有合法的"上一架书架的结尾位置" j 分成若干块:
// 同一块内 max(h[j+1..i]) 相同. 每块只记录块内 j 的个数.
int sz[MAXN + 2];
// 每块在 multiset 里的候选值 = 块高 + dp[块内最靠左的 j].
// 因为 dp 单调不减, 这个值就是块内的最小值, 全局最小值即 dp[i].
multiset<long long> cand;

int main() {
    scanf("%d %lld", &n, &limit);
    for (int i = 1; i <= n; i++) {
        scanf("%lld %lld", &h[i], &w[i]);
    }

    int base = 0, top = 0;  // 各块存放在 sz[base .. base + top - 1]
    int lo = 1;             // 窗口左端: 合法的 j 取值范围是 [lo - 1, i - 1]
    long long wsum = 0;     // 窗口内书的宽度和, 即 w[lo..i] 之和
    dp[0] = 0;
    for (int i = 1; i <= n; i++) {
        // 新书自己构成一块, 只含 j = i - 1, 候选值等合并完再算
        sz[base + top] = 1;
        top++;
        // 新书不矮于下面那块, 两块的 max 都会变成 h[i], 并成一块
        while (top > 1 && h[i - sz[base + top - 1]] <= h[i]) {
            long long peak = h[i - sz[base + top - 1]];              // 下面那块的高度(峰)
            int left = i - sz[base + top - 1] - sz[base + top - 2];  // 下面那块最左的 j
            cand.erase(cand.find(peak + dp[left]));
            sz[base + top - 2] += sz[base + top - 1];
            top--;
        }
        cand.insert(h[i] + dp[i - sz[base + top - 1]]);

        wsum += w[i];
        // 宽度超了, 从左边缩窗口: 最底下的块少掉一个 j
        while (wsum > limit) {
            wsum -= w[lo];
            cand.erase(cand.find(h[lo + sz[base] - 1] + dp[lo - 1]));
            sz[base]--;
            if (sz[base] == 0) {
                base++;  // 整块被移出窗口, 上面各块的最左端没有变
                top--;
            } else {
                cand.insert(h[lo + sz[base]] + dp[lo]);
            }
            lo++;
        }
        dp[i] = *cand.begin();
    }
    printf("%lld\n", dp[n]);
    return 0;
}
