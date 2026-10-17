#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 1005;

struct Target {
    int x;  // 位置
    int p;  // 分数
};

Target t[MAXN];
int n;

// dp[i][j] (i < j)：从 i 跳到 j、落在 j 之后（含 j）能拿到的最大分数
int dp[MAXN][MAXN];

// 按位置从小到大排序，方向统一为“往位置更大的目标跳”
bool cmpByPos(const Target &a, const Target &b) {
    return a.x < b.x;
}

// 在坐标递增的数轴上，求只能向右跳时的最大得分
int bestForward() {
    for (int j = n - 1; j >= 0; j--) {
        int ptr = n;    // 合法后继 k 构成区间 [ptr, n-1]，初值为空
        int best = 0;   // max dp[j][k]，取 0 表示落到 j 后不再起跳
        // i 递增 -> 这一次的跳跃距离 x[j]-x[i] 递减 -> 合法后继只增不减，
        // 所以 ptr 从右往左单向移动，同时维护后缀最大值，把 O(N) 转移压成 O(1)
        for (int i = 0; i < j; i++) {
            while (ptr - 1 > j && t[ptr - 1].x - t[j].x >= t[j].x - t[i].x) {
                ptr--;
                if (dp[j][ptr] > best) {
                    best = dp[j][ptr];
                }
            }
            dp[i][j] = t[j].p + best;
        }
    }
    int res = 0;
    for (int i = 0; i < n; i++) {
        if (t[i].p > res) {
            res = t[i].p;   // 只落在 i 这一个目标上
        }
        for (int j = i + 1; j < n; j++) {
            int tot = t[i].p + dp[i][j];    // 起点是 i，第一跳到 j
            if (tot > res) {
                res = tot;
            }
        }
    }
    return res;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &t[i].x, &t[i].p);
    }
    sort(t, t + n, cmpByPos);

    int ans = bestForward();
    // 反方向：把坐标镜像（取负）后整段反转，就又变成“只能向右跳”的同一个问题
    for (int i = 0; i < n; i++) {
        t[i].x = -t[i].x;
    }
    for (int i = 0, j = n - 1; i < j; i++, j--) {
        swap(t[i], t[j]);
    }
    int back = bestForward();
    if (back > ans) {
        ans = back;
    }

    printf("%d\n", ans);
    return 0;
}
