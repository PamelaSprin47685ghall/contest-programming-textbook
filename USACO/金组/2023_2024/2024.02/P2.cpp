#include <cstdio>

using namespace std;

const int MAXN = 500005;

int n;
int a[MAXN];

// 第 t 分钟结束后，桶 j 里的牛奶恰好是 a[j], a[j-1], ..., a[j-t] 的最小值
// （牛奶每分钟整桶交给下一头牛，接收方按自己的容量截断，容量限制沿传递方向累积取 min）。
// 所以第 t 分钟的答案是环上所有长度 t+1 的窗口的最小值之和。
// 直接枚举每个长度再滑动窗口是 O(N^2)，这里改成统计每个元素作为窗口最小值的次数。

// 把数组首尾相接重复两遍，环上长度不超过 n 的窗口就变成起点在 [0, n-1] 的直线窗口
int b[2 * MAXN];
int prevLE[2 * MAXN];   // 左侧最近的满足 b[j] <= b[i] 的下标，没有则为 -1
int nextL[2 * MAXN];    // 右侧最近的满足 b[j] < b[i] 的下标，没有则为 2n（哨兵）
int stk[2 * MAXN];

// 二阶差分数组：每个元素的贡献是分段线性的梯形函数，用二阶差分做 O(1) 区间加
long long diff2[4 * MAXN + 10];
long long answer[MAXN];

int main() {
    scanf("%d", &n);
    int minAll = 2000000000;
    for (int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
        if (a[i] < minAll) minAll = a[i];
        b[i] = a[i];
        b[i + n] = a[i];
    }

    int len = 2 * n;
    // 单调栈求左侧最近的 <= b[i]：弹出的都比 b[i] 大，栈顶就是答案
    int top = 0;
    for (int i = 0; i < len; ++i) {
        while (top > 0 && b[stk[top - 1]] > b[i]) --top;
        prevLE[i] = (top > 0) ? stk[top - 1] : -1;
        stk[top++] = i;
    }
    // 单调栈求右侧最近的 < b[i]：弹出的都 >= b[i]，剩下的栈顶就是答案
    top = 0;
    for (int i = len - 1; i >= 0; --i) {
        while (top > 0 && b[stk[top - 1]] >= b[i]) --top;
        nextL[i] = (top > 0) ? stk[top - 1] : len;
        stk[top++] = i;
    }

    // 规定窗口 [l, r] 的"代表最小值"是其中最靠左的最小值所在位置 i：
    // 它满足 l > prevLE[i] 且 r < nextL[i]，且 l <= i <= r，l ∈ [0, n-1]。
    // 记窗口大小 w = r - l + 1，t = w - 1 = (i - l) + (r - i) = x + y，
    // 其中 x = i - l ∈ [xlo, i - prevLE[i] - 1]，y = r - i ∈ [0, B - 1]。
    // 于是 i 作为最小值的长度为 t+1 的窗口数 = G(t) - G(t - B)，
    // 其中 G(t) = #{x ∈ [xlo, xhi] : x <= t} 是从 xlo 起斜率为 1、上限为 xhi-xlo+1 的斜坡。
    for (int i = 0; i < len - 1; ++i) {
        int xhi = i - prevLE[i] - 1;
        int xlo = i - n + 1;
        if (xlo < 0) xlo = 0;
        if (xlo > xhi) continue;
        int rlim = (nextL[i] < len - 1) ? nextL[i] : len - 1;
        int width = rlim - i;   // y 的可取个数
        if (width <= 0) continue;
        long long value = a[i % n];
        // 加上 G：t >= xlo 后每次 +1，直到达到上限 xhi - xlo + 1
        diff2[xlo] += value;
        diff2[xhi + 1] -= value;
        // 减去 G(t - B)：整体右移 width 个位置
        diff2[xlo + width] -= value;
        diff2[xhi + width + 1] += value;
    }

    // 两次前缀和还原出各 t 的窗口最小值总和
    long long slope = 0;
    long long cur = 0;
    for (int t = 0; t < n; ++t) {
        slope += diff2[t];
        cur += slope;
        answer[t] = cur;
    }

    // 第 m 分钟对应窗口大小 m+1；窗口大小达到 n 时窗口覆盖整圈，答案恒为 n * 最小值
    for (int m = 1; m <= n; ++m) {
        if (m >= n - 1) {
            printf("%lld\n", (long long)n * minAll);
        } else {
            printf("%lld\n", answer[m]);
        }
    }
    return 0;
}
