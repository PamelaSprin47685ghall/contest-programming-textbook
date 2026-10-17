#include <cstdio>

using namespace std;

const int MAXN = 22;
const int MAXC = 1005;
const int MAXMASK = 1 << 20;

int n, L;
int dur[MAXN];          // 每部电影的长度
int cnt[MAXN];          // 每部电影的场次数
int shows[MAXN][MAXC];  // 每部电影的所有放映开始时间（已升序）

int dp[MAXMASK];  // dp[mask]：只看 mask 中这些电影时，能连续看到的最晚时刻；-1 表示不可达

int popcount(int x) {
    int c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

int main() {
    scanf("%d %d", &n, &L);
    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &dur[i], &cnt[i]);
        for (int j = 0; j < cnt[i]; ++j) {
            scanf("%d", &shows[i][j]);
        }
    }
    int total = 1 << n;
    for (int mask = 1; mask < total; ++mask) {
        dp[mask] = -1;
    }
    dp[0] = 0;  // 一部还没看，当前时刻 0

    int answer = -1;
    for (int mask = 0; mask < total; ++mask) {
        int cur = dp[mask];
        if (cur < 0) continue;
        if (cur >= L) {
            // mask 本身已经看满 L 分钟；再加电影只会让电影数变多，不可能更优
            int bits = popcount(mask);
            if (answer < 0 || bits < answer) answer = bits;
            continue;
        }
        for (int i = 0; i < n; ++i) {
            if (mask & (1 << i)) continue;
            // 每部电影只能看一次，所以必须赶在 cur 之前或正好在 cur 开始的那一场；
            // 二分找 <= cur 的最大场次，从 dp[mask] 中途进场也能看到 dur[i] 分钟
            int lo = -1, hi = cnt[i] - 1;
            while (lo < hi) {
                int mid = (lo + hi + 1) / 2;
                if (shows[i][mid] <= cur) {
                    lo = mid;
                } else {
                    hi = mid - 1;
                }
            }
            if (lo < 0) continue;
            int reach = shows[i][lo] + dur[i];
            int nmask = mask | (1 << i);
            if (reach > dp[nmask]) dp[nmask] = reach;
        }
    }
    printf("%d\n", answer);
    return 0;
}
