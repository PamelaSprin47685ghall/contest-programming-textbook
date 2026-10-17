// USACO 2016 US Open Silver P2 - Diamond Collector
// 思路：排序后一定存在最优方案，使得一个盒子里的钻石全部比另一个盒子里的都小（把最小的那颗能带上的
// 都放进同一个盒子即可证明）。于是枚举「分界点」i：左盒只取前缀里的钻石，右盒只取后缀里的钻石。
// 预处理 lft[i] = 大小都不超过 a[i] 的前提下一个盒子最多装几颗（后缀同理 rgt[i]），
// 则答案为 max(lft[i] + rgt[i+1])。
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 50005;
int n;
long long k;
long long a[MAXN];
int lft[MAXN], rgt[MAXN];

int main() {
    scanf("%d%lld", &n, &k);
    for (int i = 0; i < n; i++) scanf("%lld", &a[i]);
    sort(a, a + n);

    // lft[i]：只允许使用大小 <= a[i] 的钻石时，单个盒子的最大颗数（滑动窗口 + 前缀最大值）
    int j = 0;
    for (int i = 0; i < n; i++) {
        while (a[i] - a[j] > k) j++;
        int cnt = i - j + 1;
        lft[i] = cnt;
        if (i > 0 && lft[i - 1] > lft[i]) lft[i] = lft[i - 1];
    }
    // rgt[i]：只允许使用大小 >= a[i] 的钻石时，单个盒子的最大颗数（滑动窗口 + 后缀最大值）
    j = n - 1;
    for (int i = n - 1; i >= 0; i--) {
        while (a[j] - a[i] > k) j--;
        int cnt = j - i + 1;
        rgt[i] = cnt;
        if (i < n - 1 && rgt[i + 1] > rgt[i]) rgt[i] = rgt[i + 1];
    }

    int ans = 0;
    for (int i = 0; i + 1 < n; i++) {
        int sum = lft[i] + rgt[i + 1];
        if (sum > ans) ans = sum;
    }
    if (ans == 0) ans = n;  // n == 1 时上面的循环不会执行，一颗钻石装进一个盒子即可
    printf("%d\n", ans);
    return 0;
}
