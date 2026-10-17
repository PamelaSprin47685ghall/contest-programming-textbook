#include <cstdio>

using namespace std;

const int MAXN = 100000;

int cards[MAXN + 2];  // 首尾各留一个 0: 点数 1..N 之外没有牌, 相当于 a[0] = a[N+1] = 0

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &cards[i]);
    }

    // 一副顺子 [i, j] 只拿走两个端点各一张, 所以只改变 a[i]-a[i-1] 和
    // a[j+1]-a[j] 这两个相邻差, 其余相邻差不变, 于是相邻差绝对值之和最多减少 2;
    // 牌全部打完时相邻差之和为 0, 故顺子数不少于 (相邻差绝对值之和) / 2.
    // 这个下界可达到: 每次取"最长顺子" [i, j] (即 a[i] > 0 而 a[i-1] = 0),
    // 它的左端把 a[i]-a[i-1] 从 a[i] 变成 a[i]-1, 右端同理, 正好各减 1.
    long long diffSum = 0;  // 每一项不超过 1e5, 但 N 项之和可达 2e10, 用 long long
    for (int i = 0; i <= n; ++i) {
        int diff = cards[i + 1] - cards[i];
        if (diff < 0) {
            diff = -diff;
        }
        diffSum += diff;
    }

    printf("%lld\n", diffSum / 2);
    return 0;
}
