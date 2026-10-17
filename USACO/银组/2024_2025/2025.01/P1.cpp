#include <cstdio>
#include <algorithm>

using namespace std;

// 思路：枚举所有满足 a[i] == b[j] 的位置对 (i, j)，统计"有多少次操作会把第 i 头牛送到
// 第 j 个位置"，答案就是这些次数的总和（每个位置对独立计数，最后按行把贡献相加）。
// 对 i < j 的一对，贡献是 min(i, N-1-j) + 1 = min(i+1, N-j)：
//   * 反转区间完全包含 i、j 且以 (i+j)/2 为中心时贡献 1（共 min(i, N-1-j) 种），
//   * [i, j] 本身（即把 i 直接换到 j）额外贡献 1。
// i == j 时反过来要对所有让第 i 个位置不动的区间计数（见下面主循环的公式）。
// 因为 N 可达 5e5，不可能枚举位置对，所以按数值分组：把 a 的出现记成"查询"、
// b 的出现记成"候选"，对每个数值从右往左扫描，用单调栈把候选分成
// "贡献 N-j"（小）和"贡献 i+1"（大）两部分，均摊 O(N)。
const int MAXN = 500005;

int a[MAXN], b[MAXN];

struct Event {
    int value; // 事件对应的数值
    int index; // 事件所在下标
    int isB;   // 0 表示来自数组 a，1 表示来自数组 b
};

Event events[2 * MAXN];
int stackValues[MAXN]; // 每个数值独立使用的单调栈，存 N-j

// 同一数值内按下标从大到小处理；下标相同时先处理 a 的事件，
// 这样 (i, i) 这种同下标的对不会被误算成 i < j 的对。
bool eventLess(const Event &p, const Event &q) {
    if (p.value != q.value) {
        return p.value < q.value;
    }
    if (p.index != q.index) {
        return p.index > q.index;
    }
    return p.isB < q.isB;
}

// 1 + 2 + ... + x
long long triangular(long long x) {
    return x * (x + 1) / 2;
}

int main() {
    int N;
    scanf("%d", &N);
    for (int i = 0; i < N; ++i) {
        scanf("%d", &a[i]);
    }
    for (int i = 0; i < N; ++i) {
        scanf("%d", &b[i]);
    }
    long long answer = 0;
    // i == j 的部分：让第 i 个位置原地不动的反转区间分为三类，
    // 完全落在 i 左侧（triangular(i) 个）、完全落在 i 右侧（triangular(N-1-i) 个）、
    // 以 i 为中心左右对称（min(i, N-1-i) 个加上区间 [i, i] 本身）。
    for (int i = 0; i < N; ++i) {
        if (a[i] == b[i]) {
            answer += triangular(i) + triangular(N - 1 - i) + min(i, N - 1 - i) + 1;
        }
    }
    for (int pass = 0; pass < 2; ++pass) {
        int total = 0;
        for (int i = 0; i < N; ++i) {
            events[total].value = a[i];
            events[total].index = i;
            events[total].isB = 0;
            ++total;
            events[total].value = b[i];
            events[total].index = i;
            events[total].isB = 1;
            ++total;
        }
        sort(events, events + total, eventLess);
        int top = 0;
        long long numLarge = 0, sumSmall = 0;
        for (int k = 0; k < total; ++k) {
            if (k > 0 && events[k].value != events[k - 1].value) {
                // 换了数值，单调栈与统计量必须清空，保证不同数值互不干扰
                top = 0;
                numLarge = 0;
                sumSmall = 0;
            }
            if (events[k].isB) {
                // 候选 j：N-j 先按"小贡献"（即贡献 N-j）记账并压栈
                int small = N - events[k].index;
                stackValues[top++] = small;
                sumSmall += small;
            } else {
                // 查询 i：贡献是 min(i+1, N-j)，把栈顶中 N-j > i+1 的改成大贡献 i+1
                int x = events[k].index + 1;
                while (top > 0 && stackValues[top - 1] > x) {
                    --top;
                    sumSmall -= stackValues[top];
                    ++numLarge;
                }
                answer += sumSmall + numLarge * x;
            }
        }
        // 第二遍把两个数组倒过来，等价于统计 i > j 的位置对
        reverse(a, a + N);
        reverse(b, b + N);
    }
    printf("%lld\n", answer);
    return 0;
}
