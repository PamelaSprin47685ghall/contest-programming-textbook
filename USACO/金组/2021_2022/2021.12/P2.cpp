#include <cstdio>
using namespace std;

// N 最大 2e5，数组放全局区，避免栈空间不足；下标 1..N，用 0 作哨兵
static int a[200005];      // 排列本身
static int pos[200005];    // pos[v] = 值 v 在排列中的下标
static int prv[200005];    // prv[i] = i 之前、比 a[i] 大的最小元素所在的下标（不存在记 0）
static bool hilo[200005];  // 位置 i 是否是某个 HILO 对中的那个 LO
static int st[200005];     // 手写单调栈，存下标
static int top_;

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        pos[a[i]] = i;
    }

    // 结论一：位置 i 若是某个 HILO 对里的 LO，则与之配对的 HI 必是
    // i 之前比 a[i] 大的最小元素 a[prv[i]]。因为若比它更大的元素先被说 HI，
    // 那么 i 一定会被跳过（不会再被问）。
    // 按值从大到小扫描并维护"位置单调递增、值单调递减"的栈：位置在 pos[v] 右边
    // 的元素不可能成为任何更靠左位置的"之前最小更大元素"，直接弹出。
    top_ = 0;
    st[top_++] = 0;  // 哨兵：0 比所有下标都小，永不出栈，表示"不存在"
    for (int v = n; v >= 1; v--) {
        while (st[top_ - 1] > pos[v]) top_--;
        prv[pos[v]] = st[top_ - 1];
        st[top_++] = pos[v];
    }

    // 结论二：对每个 x，Bessie 会说 LO 的位置集合，正好是把值 1..x 依次插入
    // 上述"位置单调递增"栈后栈内的下标；x 变到 x+1 时只需删掉位置在 pos[x+1]
    // 右侧的元素（它们此后永远不会再说 LO），所以总量 O(N)。
    // 结论三：位置 i 是 HILO 对中的 LO 当且仅当 prv[i] 存在，且 i 之前最后一次
    // 说 LO 的位置 j 不存在或 prv[i] != prv[j]。
    top_ = 0;
    st[top_++] = 0;
    int cnt = 0;  // 当前 x 下 HILO 的出现次数
    printf("0\n");  // x = 0：全是 HI，答案为 0
    for (int v = 1; v <= n; v++) {
        int i = pos[v];
        while (st[top_ - 1] > i) {
            cnt -= hilo[st[top_ - 1]];  // 该位置不再说 LO，若它曾是 HILO 的 LO 则减掉
            top_--;
        }
        int j = st[top_ - 1];  // i 之前最后一次说 LO 的位置，0 表示没有
        hilo[i] = prv[i] != 0 && (j == 0 || prv[i] != prv[j]);
        st[top_++] = i;
        cnt += hilo[i];
        printf("%d\n", cnt);
    }
    return 0;
}
