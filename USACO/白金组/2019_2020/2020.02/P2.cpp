#include <cstdio>
#include <algorithm>
using namespace std;

int N;
bool G[300][300], GG[300][300];
long long ans;

// 把网格顺时针旋转 90 度：这样"左下角"这个特殊位置会轮流取遍包围矩形的四个角，
// 同一个三角形在四种朝向中被统计的次数正好等于它落在包围矩形角上的顶点数，
// 而曼哈顿等边三角形最多只有两个顶点落在角上，配合下面的统计方式不会重复计数。
void rot() {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            GG[N - 1 - j][i] = G[i][j];
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            G[i][j] = GG[i][j];
}

// 用对角线编号 sum = x + y 描述一条副对角线。
// 统计所有"三角形的一个顶点 a 位于第 a 条副对角线且是包围矩形左下角"的三角形：
// 另外两个顶点 b、c 必定同在第 b 条副对角线上，且坐标差为 (dif, -dif)，dif = (b - a) / 2。
// 于是 a 的副对角线每上一个点 (i, a - i)，答案就加上"当前能与它配对的 (b, c) 对数" cur。
// 关键优化：把 a 沿副对角线移动一格时，合法的 (b, c) 对只增删常数个，因此 cur 可以 O(1) 转移。
void solve() {
    for (int a = 0; a < 2 * N - 1; ++a)
        for (int b = a + 2; b < 2 * N - 1; b += 2) {
            int dif = (b - a) / 2;
            int st = max(0, a - (N - 1)), en = min(a, N - 1);
            int cur = 0; // 当前 (i, a - i) 处能配成的 (b, c) 对数
            for (int i = st; i <= en; ++i) {
                if (i == st) // 起点：暴力统计一次，之后只做 O(1) 转移
                    for (int j = max(i, b - (N - 1)); j < min(i + dif, N - dif); ++j)
                        cur += G[j][b - j] && G[j + dif][b - j - dif];
                if (G[i][a - i]) ans += cur;
                // 移到下一个点 (i + 1, a - i - 1)：不断有新点对进入/离开配对集合
                if (i + 2 * dif < N && b - (i + dif) < N)
                    cur += G[i + dif][b - i - dif] && G[i + 2 * dif][b - i - 2 * dif];
                if (i + dif < N && b - i < N)
                    cur -= G[i][b - i] && G[i + dif][b - i - dif];
            }
        }
}

int main() {
    if (scanf("%d", &N) != 1) return 1;
    char buf[512];
    for (int i = 0; i < N; ++i) {
        scanf("%s", buf);
        for (int j = 0; j < N; ++j)
            G[i][j] = (buf[j] == '*');
    }
    for (int t = 0; t < 4; ++t) { // 四个朝向各统计一遍
        solve();
        rot();
    }
    printf("%lld\n", ans);
    return 0;
}
