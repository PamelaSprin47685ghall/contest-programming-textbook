#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 20;
const int MAXMASK = 1 << MAXN;
const long long INF = (long long)4e18;  // 空塔的安全系数：地面能撑住任意重量

int n;                     // 奶牛数量
long long markHeight;      // Mark 的身高 H
long long height[MAXN];    // 每头奶牛的身高
long long weight[MAXN];    // 每头奶牛的体重
long long strength[MAXN];  // 每头奶牛上方最多能承受的总重量

long long dp[MAXMASK];         // dp[S]：恰好用集合 S 里的奶牛堆成的塔的最大安全系数
long long sumWeight[MAXMASK];  // 集合 S 的总体重，也就是压在塔底下方东西上的重量
long long sumHeight[MAXMASK];  // 集合 S 的总身高，只需与 H 比较，故超过 H 就截断

int main() {
    scanf("%d%lld", &n, &markHeight);
    for (int i = 0; i < n; ++i) {
        scanf("%lld%lld%lld", &height[i], &weight[i], &strength[i]);
    }

    int total = 1 << n;
    dp[0] = INF;  // 塔顶加多重都不会压坏地面
    bool found = false;
    long long answer = 0;
    for (int mask = 1; mask < total; ++mask) {
        int low = mask & (-mask);
        int idx = __builtin_ctz(low);  // 取出一个元素即可递推出集合的和
        int rest = mask ^ low;
        sumWeight[mask] = sumWeight[rest] + weight[idx];
        // 身高和只用来判断"够不够 H"，截断到 H 既避免溢出也不影响判断结果
        sumHeight[mask] = min(markHeight, sumHeight[rest] + height[idx]);
        // 枚举站在最底下的奶牛 c：它上方恰好是 rest 这个集合
        long long best = -INF;
        for (int c = 0; c < n; ++c) {
            if (!(mask & (1 << c))) continue;
            // c 自身还能额外承受的重量，即整塔安全系数的上界
            long long spare = strength[c] - (sumWeight[mask] - weight[c]);
            // 上方那堆的最大安全系数是 dp[rest]，两者取较小值就是整塔的安全系数
            long long candidate = min(dp[mask ^ (1 << c)], spare);
            if (candidate > best) best = candidate;
        }
        dp[mask] = best;
        // 安全系数为负说明这个子集无论怎么排都有奶牛被压垮，塔根本立不起来，
        // 因此只有 best >= 0 的塔才算是"能叠出来"的，才可能成为答案。
        if (sumHeight[mask] >= markHeight && best >= 0 && (!found || best > answer)) {
            found = true;
            answer = best;
        }
    }
    if (found) printf("%lld\n", answer);
    else printf("Mark is too tall\n");
    return 0;
}
