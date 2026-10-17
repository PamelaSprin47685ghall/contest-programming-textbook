#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 262144; // 序列长度上限
const int MAXV = 60;     // 可能生成的最大值上限：初值不超过 40，每次合并值 +1，最多合并 18 次

int n;              // 序列长度
int a[MAXN + 2];    // 初始序列
int endA[MAXN + 2]; // 滚动数组 A
int endB[MAXN + 2]; // 滚动数组 B

// 记 f(v, i) 为「从下标 i 出发、恰好能合并出单个值 v 的区间右端点（左闭右开）」，
// 不存在则记 -1。因为一段区间能合并出 v 的必要条件是各元素按 2^a[i] 求和恰好等于 2^v，
// 而元素全为正数，所以这样的右端点唯一，f 的定义不会冲突。
// 于是合并出 v 的那段区间再由左右两半各自合并出 v-1 拼成，
// 且左半段必须正好是 [i, f(v-1, i))，故 f(v, i) = f(v-1, f(v-1, i))。
// 另外 a[i] 本身就已经是一段能合并出 a[i] 的区间特例，需单独作为边界条件。
int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);

    int* prev = endA; // 存放值 v-1 的 f
    int* cur = endB;  // 存放值 v 的 f
    for (int i = 0; i <= n; i++) {
        endA[i] = -1;
        endB[i] = -1; // 下标 n 作为越界哨兵，永远取 -1
    }

    int ans = 0;
    for (int v = 1; v <= MAXV; v++) {
        bool reachable = false;
        for (int i = 0; i < n; i++) {
            if (a[i] == v) {
                cur[i] = i + 1; // 单个元素就是长度为 1 的区间
            } else if (prev[i] != -1 && prev[prev[i]] != -1) {
                // 左半段 [i, prev[i]) 与右半段 [prev[i], prev[prev[i]]) 都能合并出 v-1
                cur[i] = prev[prev[i]];
            } else {
                cur[i] = -1;
            }
            if (cur[i] != -1) reachable = true;
        }
        // 存在能合并出 v 的区间，说明最终序列中可以留下数值 v
        if (reachable) ans = v;
        int* tmp = prev; // 交换滚动数组，下一轮讨论值 v+1
        prev = cur;
        cur = tmp;
    }

    printf("%d\n", ans);
    return 0;
}
