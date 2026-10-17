#include <cstdio>

using namespace std;

const int MAXM = 300005;

int m;
// change[i][p]：第 i 回合 Elsie 猜奇偶性 p（0 为 Even）时，Bessie 最优应对下她石子数的变化量
long long change[MAXM][2];
// minPsum[i]：从第 i 回合开始，Elsie 每回合都挑对她最有利的奇偶性时，后缀前缀和的最小值
long long minPsum[MAXM + 1];

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        long long n;
        int k;
        scanf("%lld%d%d", &n, &m, &k);
        for (int i = 0; i < m; ++i) {
            change[i][0] = 1000000000;
            change[i][1] = 1000000000;
            for (int j = 0; j < k; ++j) {
                long long a;
                scanf("%lld", &a);
                int parity = (int)(a % 2);
                // 猜对就 +a，猜错就 -a；Bessie 会在每种猜法里挑让 Elsie 变化最小的那个
                if (a < change[i][parity]) {
                    change[i][parity] = a;
                }
                if (-a < change[i][parity ^ 1]) {
                    change[i][parity ^ 1] = -a;
                }
            }
        }
        // 后缀里 Elsie 总能选到较好的一边，所以 suffix 的最坏情况是三段取最小：
        // 「这一步就撑不住」或「这一步拿到较好结果后，后面仍然撑不住」
        minPsum[m] = 0;
        for (int i = m - 1; i >= 0; --i) {
            long long better = change[i][0] > change[i][1] ? change[i][0] : change[i][1];
            long long worst = better + minPsum[i + 1];
            minPsum[i] = worst < 0 ? worst : 0;
        }
        // 连每一步都挑最好的走法也活不下去，说明无论怎么走都会输
        if (n + minPsum[0] <= 0) {
            printf("-1\n");
            continue;
        }
        for (int i = 0; i < m; ++i) {
            // 贪心：能猜 Even 就猜 Even（字典序更小），只有猜 Even 之后必输才改猜 Odd
            int parity = (n + change[i][0] + minPsum[i + 1] <= 0) ? 1 : 0;
            if (i > 0) {
                printf(" ");
            }
            printf(parity == 0 ? "Even" : "Odd");
            n += change[i][parity];
        }
        printf("\n");
    }
    return 0;
}
