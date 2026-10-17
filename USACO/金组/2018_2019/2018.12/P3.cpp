#include <cstdio>

using namespace std;

const int MAXN = 10005;

int n, k;
int skill[MAXN];
long long dp[MAXN]; // dp[i]: 前 i 头牛分成若干队能得到的最大技能总和

int main() {
    scanf("%d %d", &n, &k);
    for (int i = 1; i <= n; i++) scanf("%d", &skill[i]);

    for (int i = 1; i <= n; i++) {
        int best = 0; // 最后一队从 j 到 i 的技能最大值
        // 枚举最后一队的起点 j: 队伍向左扩展时只需 O(1) 更新最大值,
        // 于是每个状态的转移从 O(K) 降到均摊 O(1)
        for (int j = i; j >= 1 && i - j + 1 <= k; j--) {
            if (skill[j] > best) best = skill[j];
            long long cand = dp[j - 1] + (long long)best * (i - j + 1);
            if (cand > dp[i]) dp[i] = cand;
        }
    }

    printf("%lld\n", dp[n]);
    return 0;
}
