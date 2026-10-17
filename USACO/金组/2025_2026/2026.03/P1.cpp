// P1 Good Cyclic Shifts
// 结论：排列"好" <=> 它不含 321 子序列（不存在 i<j<k 且 p_i>p_j>p_k）。
// 对每个元素 j 求出环上"向左最近的更大元素 g"和"向右最近的更小元素 s"：
// 若某个循环移位里存在 321，取其中间那个元素 j，则 g、j、s 一定也按顺序落在同一个窗口里；
// 反之只要 g、j、s 落进同一个窗口就构成 321。于是问题转化为：
// 对每个 j，标记出那些"能把 g、j、s 按顺序装下"的窗口起点，它们是一段环上的连续区间。
// 用差分数组做区间加，最后没被任何元素标记的起点对应的移位就是答案。
#include <cstdio>

using namespace std;

const int MAXN = 200005;

int n;
int val[2 * MAXN];        // 断环成链后的值：val[i] = val[i + n]
int dL[MAXN];             // dL[j]：环上向左最近的大于 p[j] 的元素与 j 的距离
int dR[MAXN];             // dR[j]：环上向右最近的小于 p[j] 的元素与 j 的距离
int stk[2 * MAXN];        // 单调栈，存 2n 长链上的下标
int diff[2 * MAXN + 2];   // 坏起点的差分标记
char bad[MAXN];           // bad[st]：从环上位置 st 开始的窗口是否含 321

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        for (int i = 0; i < n; i++) {
            scanf("%d", &val[i]);
            val[i + n] = val[i];
        }

        // 向左最近的更大元素：在 2n 长的链上单调栈，只在下标 >= n 时记录答案。
        // 栈里保留的"下标 % n"就是环上位置，减出环上距离即可。
        int top = 0;
        for (int i = 0; i < 2 * n; i++) {
            while (top > 0 && val[stk[top - 1]] <= val[i]) top--;
            if (i >= n) {
                int j = i - n;
                // 栈顶下标与 j 同余不可能发生：自己那份值相等，早就被弹掉了
                dL[j] = (top == 0) ? -1 : (j - stk[top - 1] % n + n) % n;
            }
            stk[top++] = i;
        }

        // 向右最近的更小元素：从右往左扫，只在下标 < n 时记录答案
        top = 0;
        for (int i = 2 * n - 1; i >= 0; i--) {
            while (top > 0 && val[stk[top - 1]] >= val[i]) top--;
            if (i < n) {
                dR[i] = (top == 0) ? -1 : (stk[top - 1] % n - i + n) % n;
            }
            stk[top++] = i;
        }

        for (int i = 0; i <= 2 * n; i++) diff[i] = 0;
        for (int j = 0; j < n; j++) {
            if (dL[j] < 0 || dR[j] < 0) continue;   // 自己是最大/最小值，当不了 321 里的"2"
            if (dL[j] + dR[j] > n - 1) continue;    // 三点跨度超过窗口长度，任何移位都装不下
            // 窗口起点为 st 时，j 在窗口中的位置是 (j - st) mod n；
            // 要装下 g、j、s 就要求该位置落在 [dL, n-1-dR] 内，对应 len 个起点。
            int len = n - dL[j] - dR[j];
            int end = ((j - dL[j]) % n + n) % n + n;   // 抬到第二圈，避免区间跨过 0
            diff[end - len + 1]++;
            diff[end + 1]--;
        }

        // 前缀和得到每个起点被标记的次数；起点 st 与 st+n 其实是同一个循环移位
        int cur = 0;
        for (int c = 0; c < 2 * n; c++) {
            cur += diff[c];
            if (c < n) bad[c] = (cur > 0);
            else if (cur > 0) bad[c - n] = 1;
        }

        int cnt = 0;
        for (int st = 0; st < n; st++)
            if (!bad[st]) cnt++;
        printf("%d\n", cnt);
        // 输出的是"向右循环移位 s 次"，它对应的窗口起点是 (n - s) mod n
        for (int s = 0; s < n; s++) {
            int st = (n - s) % n;
            if (!bad[st]) printf("%d ", s);
        }
        printf("\n");
    }
    return 0;
}
