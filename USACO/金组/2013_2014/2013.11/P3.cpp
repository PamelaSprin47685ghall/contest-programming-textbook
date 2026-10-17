#include <cstdio>

using namespace std;

const int MAXK = 16;
const int MAXN = 100000;

int k, n;
long long coin[MAXK];    // 每枚硬币的面值
long long pay[MAXN + 1]; // pay[i] = 前 i 笔购物一共花掉的钱
int dp[1 << MAXK];       // dp[mask] = 只用 mask 里的硬币最多能付清前多少笔

int main() {
    scanf("%d %d", &k, &n);
    long long total = 0;
    for (int i = 0; i < k; i++) {
        scanf("%lld", &coin[i]);
        total += coin[i];
    }
    for (int i = 1; i <= n; i++) {
        int c;
        scanf("%d", &c);
        pay[i] = pay[i - 1] + c;
    }

    long long answer = -1;
    for (int mask = 1; mask < (1 << k); mask++) {
        int best = 0;
        long long spent = 0; // mask 里所有硬币的面值之和
        for (int i = 0; i < k; i++) {
            if (!(mask & (1 << i))) continue;
            spent += coin[i];
            // 枚举"最后一枚硬币"是第 i 枚: 去掉它之后最多付到
            // dp[mask ^ (1 << i)] 笔, 剩下第 i 枚硬币接着往后付。
            // 付的笔数随金额单调, 二分出这枚硬币最多付到第几笔。
            long long last = pay[dp[mask ^ (1 << i)]]; // 前面已经付掉的金额
            int lo = 0, hi = n;
            while (lo < hi) {
                int mid = (lo + hi + 1) / 2;
                if (pay[mid] - last <= coin[i]) lo = mid;
                else hi = mid - 1;
            }
            if (lo > best) best = lo;
        }
        dp[mask] = best;
        // 这个硬币集合能付清全部 N 笔, 没花掉的硬币就是最后剩下的钱。
        // 硬币被浪费的情形由它的子集覆盖, 取最大值时不必额外处理。
        if (best == n && total - spent > answer) answer = total - spent;
    }
    printf("%lld\n", answer);
    return 0;
}
