#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 150005;

int n, q;
long long val[MAXN];       // 每头牛原始的产奶量
long long a[MAXN];         // 把所有产奶量排序后的数组
long long pref[MAXN + 1];  // a 的前缀和
int ord[MAXN];             // 按产奶量从小到大排出来的牛的下标
int pos[MAXN];             // 每头牛在 a 中的位置
long long base;            // 不做修改时的最优总产量

// 按产奶量升序比较两头牛；产奶量相同则按下标，保证排列唯一
bool cmpCow(int x, int y) {
    if (val[x] != val[y]) return val[x] < val[y];
    return x < y;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) scanf("%lld", &val[i]);

    for (int i = 0; i < n; ++i) ord[i] = i;
    sort(ord, ord + n, cmpCow);
    for (int k = 0; k < n; ++k) {
        pos[ord[k]] = k;
        a[k] = val[ord[k]];
    }

    for (int k = 0; k < n; ++k) pref[k + 1] = pref[k] + a[k];
    base = 0;
    for (int k = 0; k < n; ++k) base += (long long)(k + 1) * a[k];

    scanf("%d", &q);
    while (q--) {
        int i;
        long long j;
        scanf("%d %lld", &i, &j);
        int p = pos[i - 1];                                // 被改的牛在 a 中的位置
        long long old = a[p];

        // 先把 a[p] 拿走：它自己贡献 (p+1)*old，排名在它后面的元素全部降一位，
        // 于是总产量少掉 sum_{k>p} a[k]；剩下的数组记为 b（仍然有序）。
        long long ans = base - (long long)(p + 1) * old - (pref[n] - pref[p + 1]);

        // 二分 j 在 b 中的插入位置：即 b 里严格小于 j 的元素个数。
        // 直接在下标上二分，比写比较器更省事（数组已经有序）。
        int lo = 0, hi = n;
        while (lo < hi) {                                  // 第一个 a[k] >= j 的下标
            int mid = (lo + hi) / 2;
            if (a[mid] >= j) hi = mid;
            else lo = mid + 1;
        }
        // j > old 时 old 也在 "小于 j" 的计数里，但它已经被拿走，所以要减一；
        // j <= old 时所有小于 j 的元素下标都小于 p，无需修正。
        int np = (j > old) ? lo - 1 : lo;

        // 插入 j 后，b 中下标 >= np 的元素都右移一位，各自多贡献一份自身，
        // 而 j 本身贡献 (np+1)*j。
        ans += (long long)(np + 1) * j;
        if (np <= p - 1) ans += pref[p] - pref[np];         // b 中前段 a[np..p-1]
        int m = (np > p) ? np : p;                          // b 中后段 a[m+1..n-1] 的起点
        ans += pref[n] - pref[m + 1];

        printf("%lld\n", ans);
    }
    return 0;
}
