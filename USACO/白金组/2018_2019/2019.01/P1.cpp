#include <cstdio>
#include <set>

using namespace std;

const int MAXN = 300000;

char breeds[MAXN + 2]; // 每头牛的品种：'H' 或 'G'
int balance[MAXN + 1]; // balance[i]：前 i 头牛中 Holstein 比 Guernsey 多多少
int dp[MAXN + 1];      // dp[j]：前 j 头牛划分后，G 占多数或平局的选区最少有几个

int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    scanf("%s", breeds);

    // 用前缀差判断一个选区里谁占多数：
    // 选区 [i, j) 中 G 的个数不少于 H 等价于 balance[j] <= balance[i]
    balance[0] = 0;
    for (int i = 0; i < n; ++i) {
        balance[i + 1] = balance[i] + (breeds[i] == 'H' ? 1 : -1);
    }

    // dp[j] = min{ dp[i] + cost(i, j) }，i 取遍 [max(0, j - k), j - 1]，
    // 而 cost(i, j) 只会是 0 或 1，所以 dp[j] 只可能是"窗口内 dp 的最小值 m"或 m + 1。
    // 想要取到 m，就必须存在一个 i 满足 dp[i] == m 且 cost(i, j) == 0，
    // 也就是 dp[i] == m 且 balance[i] < balance[j]。
    // 因此把窗口内所有 (dp[i], balance[i]) 按字典序放进 multiset：
    // 首元素给出最小的 dp 值，以及这个 dp 值下最小的 balance，正好够做判断。
    multiset<pair<int, int> > window;
    dp[0] = 0;
    window.insert(make_pair(0, 0));

    for (int j = 1; j <= n; ++j) {
        int minDp = window.begin()->first;
        int minBalance = window.begin()->second;
        if (minBalance < balance[j]) {
            dp[j] = minDp;
        } else {
            dp[j] = minDp + 1;
        }
        window.insert(make_pair(dp[j], balance[j]));
        if (j >= k) {
            // 选区长度不超过 k，所以 j 对应的窗口里 i 最小只能到 j - k，
            // 滑出窗口的正是下标 j - k（必须先插入 dp[j] 再删，避免窗口为空）
            window.erase(window.find(make_pair(dp[j - k], balance[j - k])));
        }
    }

    printf("%d\n", dp[n]);
    return 0;
}
