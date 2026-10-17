#include <cstdio>

using namespace std;

const int MAXM = 100005;
const int LOG = 30; // 最多倒退 2^30 步, 足够覆盖 N <= 1e9

int n, m, q;
int posOf[MAXM]; // P[i]: 洗牌前第 i 张牌洗牌后到第 P[i] 个位置
int invP[MAXM];  // invP[j]: 洗牌后第 j 个位置的牌洗牌前在第几个位置
int preOf[MAXM]; // 窗口内唯一的 x 使得 x 倒退一步落在 v, 不存在则为 0
int escape[MAXM]; // 从窗口内位置 x 倒退多少步会落到 m (滑出窗口); 0 表示永远在环里
int up[LOG][MAXM]; // 倍增表: 窗口内倒退 2^k 步后的位置
int bfsQueue[MAXM];

// 窗口内倒退一步: 洗牌后位置 x (1 <= x <= m-1) 的牌, 来自洗牌前的 invP[x+1].
int Step(int x) { return invP[x + 1]; }

int Jump(int x, long long steps) {
    for (int k = 0; steps > 0; k++, steps >>= 1) {
        if (steps & 1) x = up[k][x];
    }
    return x;
}

int main() {
    scanf("%d %d %d", &n, &m, &q);
    for (int i = 1; i <= m; i++) {
        scanf("%d", &posOf[i]);
        invP[posOf[i]] = i;
    }

    // 第 u 步牌堆里还够 m 张当且仅当 n-u+1 >= m, 即 u <= n-m+1;
    // 之后牌堆不足 m 张, 只会简单地把最上面一张拿走.
    long long shuffleSteps = (long long)n - m + 1;

    // 在窗口内倒退时, 位置 x 会变成 invP[x+1]; 只有落在 m 上才算离开窗口,
    // 之后的位置只会不断 +1. 先沿反方向的唯一前驱链求出每个位置滑出窗口
    // 需要的步数 (到不了 m 的位置一定在环里, 步数记为 0).
    for (int v = 1; v <= m; v++) {
        int x = posOf[v] - 1; // invP[x+1] = v 等价于 x+1 = P[v]
        preOf[v] = (x >= 1) ? x : 0;
    }
    int qh = 0, qt = 0;
    if (preOf[m] != 0) {
        int x = preOf[m];
        escape[x] = 1;
        bfsQueue[qt++] = x;
    }
    while (qh < qt) {
        int x = bfsQueue[qh++];
        int y = preOf[x];
        if (y != 0 && escape[y] == 0) {
            escape[y] = escape[x] + 1;
            bfsQueue[qt++] = y;
        }
    }

    for (int x = 1; x < m; x++) up[0][x] = Step(x);
    up[0][m] = m; // 只在越界时兜底, 正常路径不会用到
    for (int k = 1; k < LOG; k++) {
        for (int x = 1; x <= m; x++) up[k][x] = up[k - 1][up[k - 1][x]];
    }

    for (int i = 0; i < q; i++) {
        long long qi;
        scanf("%lld", &qi);
        // 最终牌堆从上到下的第 qi 个位置, 就是第 n-qi+1 个被拿走的牌子.
        long long t = (long long)n - qi + 1;
        long long steps = t - 1; // 还要沿着时间往前倒退 steps 步
        long long p;
        if (t <= shuffleSteps) {
            // 这一步洗了牌, 被拿走的是洗牌后最上面那张, 它在洗牌前位于 invP[1]
            p = invP[1];
        } else {
            // 牌不够 m 张了, 直接拿走当前最上面一张
            p = 1;
        }
        // 倒数 m-1 步没有洗牌, 牌堆整体上移一格, 也就是倒退时位置 +1
        if (steps > shuffleSteps) {
            p += steps - shuffleSteps;
            steps = shuffleSteps;
        }
        long long ans;
        if (p >= m) {
            // 牌在窗口之外(或恰好是窗口最底), 之后每一步都只是位置 +1
            ans = p + steps;
        } else if (escape[p] != 0 && steps >= escape[p]) {
            // 这张牌最终会滑出窗口: 先变成位置 m, 然后一路 +1
            ans = m + (steps - escape[p]);
        } else {
            // 一直留在窗口内 (或在环上打转)
            ans = Jump((int)p, steps);
        }
        printf("%lld\n", ans);
    }
    return 0;
}
