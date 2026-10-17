#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100005;

int n;
int a[2 * MAXN];
long long answer;
int onesTotal;                        // 整个数组中 1 的个数

// 统计区间 [lo, hi] 内的逆序对数: 形如 A[i] = 1, A[j] = 0 (i < j) 的数对
long long countInv(int lo, int hi) {
    long long res = 0;
    int onesSoFar = 0;
    for (int i = lo; i <= hi; i++) {
        if (a[i] == 1) onesSoFar++;
        else res += onesSoFar;
    }
    return res;
}

long long absLL(long long x) {
    return x < 0 ? -x : x;
}

// 方向一: 用若干次"中央交换"把前半段最右边的 1 逐个搬到后半段。
// 第 j 次搬运: 前半段里它是第 j 靠右的 1 (位置 q), 后半段里被顺带带回前半段的是
// 第 j 靠左的 0 (位置 z)。搬运用最少步数: 前段内部 q 走到边界处的 0 各换一次,
// 后段内部把 0 挪到边界, 再加一次跨中线的交换。
void solvePushOnes(long long base) {
    static int q[MAXN], z[MAXN];
    int m = 0;
    for (int i = n - 1; i >= 0; i--)               // 前半段的 1, 从右到左
        if (a[i] == 1) q[m++] = i;
    int zc = 0;
    for (int i = n; i < 2 * n; i++)                // 后半段的 0, 从左到右
        if (a[i] == 0) z[zc++] = i;

    int limit = min(m, zc);
    long long cost = 0, delta = base;
    for (int j = 0; j < limit; j++) {
        long long cross = n - 1 - q[j];            // 前半段内部交换次数
        long long pull = z[j] - n;                 // 后半段内部交换次数
        cost += cross + pull + 1;
        // 每挪动一个 1: 前段内部交换各消掉一个逆序, 中央交换把 delta 改动 ones-N,
        // 后段内部交换各消掉一个后半段的逆序, 于是 delta 共变化 -cross + pull + ones-N
        delta += -cross + pull + (onesTotal - n);
        answer = min(answer, cost + absLL(delta));
    }
}

// 方向二: 把前半段最右边的 0 逐个搬到后半段(等价于把后半段的 1 拉进前半段)。
// 第 k 次搬运用前半段第 k 靠右的 0 (位置 p) 和后半段第 k 靠左的 1 (位置 y)。
void solvePushZeros(long long base) {
    static int p[MAXN], y[MAXN];
    int zc = 0;
    for (int i = n - 1; i >= 0; i--)               // 前半段的 0, 从右到左
        if (a[i] == 0) p[zc++] = i;
    int m2 = 0;
    for (int i = n; i < 2 * n; i++)                // 后半段的 1, 从左到右
        if (a[i] == 1) y[m2++] = i;

    int limit = min(zc, m2);
    long long cost = 0, delta = base;
    for (int k = 0; k < limit; k++) {
        long long cross = n - 1 - p[k];
        long long pull = y[k] - n;
        cost += cross + pull + 1;
        // 0 往右挪时每越过一个 1 就多出一对逆序, 后段里 1 往左挪每越过一个 0
        // 同样多出一对逆序, 中央交换把 delta 改变 n - ones
        delta += cross - pull + (n - onesTotal);
        answer = min(answer, cost + absLL(delta));
    }
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < 2 * n; i++) {
        scanf("%d", &a[i]);
        onesTotal += a[i];
    }
    long long base = countInv(0, n - 1) - countInv(n, 2 * n - 1);
    answer = absLL(base);      // 一次中央交换都不用, 在逆序多的一半里做 |base| 次交换

    solvePushOnes(base);
    solvePushZeros(base);

    printf("%lld\n", answer);
    return 0;
}
