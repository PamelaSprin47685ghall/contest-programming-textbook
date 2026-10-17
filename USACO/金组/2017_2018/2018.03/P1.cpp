#include <cstdio>
#include <algorithm>
#include <utility>

using namespace std;

const int MAXN = 100005;

int n;
pair<int, int> valueAt[MAXN]; // (数值, 原始下标)，按数值排序后即可还原每个元素的原位置
int bit[MAXN];                // 树状数组，标记"已经统计过的原始下标"

// 在树状数组中给位置 j 打上标记
void Add(int j) {
    for (; j <= n; j += j & -j) bit[j]++;
}

// 查询位置 1..j 中被标记的个数
int Sum(int j) {
    int s = 0;
    for (; j > 0; j -= j & -j) s += bit[j];
    return s;
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        int x;
        scanf("%d", &x);
        valueAt[i] = make_pair(x, i);
    }
    sort(valueAt + 1, valueAt + n + 1);

    // 即使数组一开始就有序，主循环也要跑一轮才会把 sorted 重新置为 true，所以至少打印一次 moo
    int answer = 1;
    // 枚举分界线：第 i 小的元素必须排在分界线左边，才算"前 i 个位置由前 i 小的元素占据"
    for (int i = 1; i <= n - 1; i++) {
        Add(valueAt[i].second);
        // 前 i 小的元素里，原本就位于分界线左侧的有 Sum(i) 个，
        // 剩下的 i - Sum(i) 个必须跨过分界线，这就是该分界线的流量下界
        int flow = i - Sum(i);
        if (flow > answer) answer = flow;
    }
    printf("%d\n", answer);
    return 0;
}
