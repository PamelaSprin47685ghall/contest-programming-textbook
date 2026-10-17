// P2 Milk Buckets
// 第二阶段每次合并相邻两桶取平均，等价于最后那桶 = Σ a_i · 2^(-d_i)（d_i 为该桶参与合并的次数），
// 要让总量最大就要让大数尽量"晚合并"，于是把数组摆成"先不升后不降"的谷形，
// 谷底的两个最小值最先被合并。剩下的问题就是求把这个数组排成谷形的最少相邻交换次数。
#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 200005;

int n;
struct Item {
    int val, idx;
} items[MAXN];
int bit[MAXN];  // 只存"值严格更小"的元素的位置，用来数它们分列在左右两侧的个数

bool cmpVal(const Item &x, const Item &y) {
    return x.val < y.val;
}

void bitAdd(int pos) {
    for (int i = pos; i <= n; i += i & -i) bit[i]++;
}

int bitSum(int pos) {
    int s = 0;
    for (int i = pos; i > 0; i -= i & -i) s += bit[i];
    return s;
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        for (int i = 0; i < n; i++) {
            scanf("%d", &items[i].val);
            items[i].idx = i + 1;
        }
        sort(items, items + n, cmpVal);  // 按值从小到大处理：处理到某个值时，树里恰好只有比它小的元素
        for (int i = 1; i <= n; i++) bit[i] = 0;
        long long ans = 0;
        int i = 0;
        while (i < n) {
            int j = i;
            while (j < n && items[j].val == items[i].val) j++;
            // 值相同的元素之间不算"更小"，必须先把整组都统计完再一起插入，否则组内元素会互相干扰
            for (int k = i; k < j; k++) {
                int p = items[k].idx;
                int left = bitSum(p - 1);            // 左边严格更小的个数
                int right = bitSum(n) - bitSum(p);   // 右边严格更小的个数
                // 无论把它放左半边（不升段）还是右半边（不降段），都要与这一侧所有更小的元素换一次位，
                // 两边都可行，取较小的一边即可
                ans += (left < right ? left : right);
            }
            for (int k = i; k < j; k++) bitAdd(items[k].idx);
            i = j;
        }
        printf("%lld\n", ans);
    }
    return 0;
}
