#include <cstdio>
#include <set>

using namespace std;

const int MAXN = 200005;

int n, q;
int a[MAXN];            // a[i] = 奶牛 i 的投票对象
int cnt[MAXN];          // cnt[v] = 奶牛 v 当前的得票数
int freqNum[MAXN];      // freqNum[f] = 得票数恰好为 f 的奶牛个数
set<int> vals[MAXN];    // vals[f] = 得票数恰好为 f 的所有奶牛编号
set<int> freqSet;       // 当前出现过的非零得票数(去重、有序)

int freqList[MAXN];     // 把 freqSet 摊平成数组, 方便两指针
int loVal[MAXN], hiVal[MAXN];   // 每个票数上的最小编号 / 最大编号
int sufMax[MAXN];       // 票数 >= freqList[i] 的奶牛里最大的编号

// 把奶牛 v 的得票数加上 delta
void change(int v, int delta) {
    int f = cnt[v];
    vals[f].erase(v);
    if (--freqNum[f] == 0 && f > 0) freqSet.erase(f);
    f += delta;
    cnt[v] = f;
    vals[f].insert(v);
    if (freqNum[f]++ == 0 && f > 0) freqSet.insert(f);
}

int main() {
    scanf("%d %d", &n, &q);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);

    for (int i = 1; i <= n; i++) cnt[a[i]]++;
    for (int v = 1; v <= n; v++) {
        freqNum[cnt[v]]++;
        vals[cnt[v]].insert(v);
    }
    for (int f = 1; f <= n; f++)
        if (freqNum[f] > 0) freqSet.insert(f);

    while (q--) {
        int idx, x;
        scanf("%d %d", &idx, &x);
        change(a[idx], -1);
        a[idx] = x;
        change(x, 1);

        // 设 k 为最大得票数, 则奶牛 p、q 能同时当两个首领 <=> cnt[p] + cnt[q] >= k (且都非零)。
        // 于是只需找满足该条件的编号差最大的 (p, q)。
        // 出现过的不同票数 f 满足 sum f <= N, 个数不超过 sqrt(2N) ~ 633 个, 可以每轮全部枚举。
        int m = 0;
        for (set<int>::iterator it = freqSet.begin(); it != freqSet.end(); ++it) freqList[m++] = *it;
        int mode = freqList[m - 1];

        for (int i = 0; i < m; i++) {
            loVal[i] = *vals[freqList[i]].begin();
            hiVal[i] = *vals[freqList[i]].rbegin();
        }
        sufMax[m - 1] = hiVal[m - 1];
        for (int i = m - 2; i >= 0; i--) sufMax[i] = sufMax[i + 1] > hiVal[i] ? sufMax[i + 1] : hiVal[i];

        // 对每个票数 f = freqList[i], 找出最小的票数档位 r 使 freqList[r] + f >= mode,
        // 该档位及更大档位上的奶牛都能与 loVal[i] 配对。f 增大时 r 单调左移, 用两指针。
        int ans = 0;
        int r = m - 1;
        for (int i = 0; i < m; i++) {
            while (r - 1 >= 0 && freqList[r - 1] + freqList[i] >= mode) r--;
            int cand = sufMax[r] - loVal[i];
            if (cand < 0) cand = -cand;
            if (cand > ans) ans = cand;
        }
        printf("%d\n", ans);
    }
    return 0;
}
