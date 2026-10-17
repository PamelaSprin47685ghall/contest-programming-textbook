#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 40;
const int MAXT = MAXN * (MAXN - 1) * (MAXN - 2) / 6;  // C(40,3) = 9880
const long long MOD = 1000000007;

struct Tri {
    int a, b, c;
    long long area2;
};

int n;
long long xs[MAXN], ys[MAXN];
long long insideCnt[MAXN][MAXN][MAXN];  // 三角形内部（含边界）的点数
long long dp[MAXN][MAXN][MAXN];         // 倒序 DP：dp[T] = 当前大三角形为 T 时，画完 T 外所有点的方案数
long long perm[MAXN + 1][MAXN + 1];     // perm[a][b] = a!/(a-b)!：从 a 个元素中有序取 b 个
Tri tris[MAXT];

long long crossArea(int a, int b, int c) {
    long long v = (xs[b] - xs[a]) * (ys[c] - ys[a]) - (ys[b] - ys[a]) * (xs[c] - xs[a]);
    return v < 0 ? -v : v;
}

bool pointInTri(int a, int b, int c, int p) {
    // 用面积和判断：p 在三角形 abc 内（含边界）当且仅当三个子三角形面积之和等于总面积
    return crossArea(a, b, c) == crossArea(a, b, p) + crossArea(b, c, p) + crossArea(c, a, p);
}

bool cmpTri(const Tri& u, const Tri& v) {
    return u.area2 > v.area2;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) scanf("%lld %lld", &xs[i], &ys[i]);

    perm[0][0] = 1;
    for (int a = 1; a <= n; ++a) {
        perm[a][0] = 1;
        for (int b = 1; b <= a; ++b) perm[a][b] = perm[a - 1][b - 1] * a % MOD;
    }

    int m = 0;
    for (int a = 0; a < n; ++a)
        for (int b = a + 1; b < n; ++b)
            for (int c = b + 1; c < n; ++c) {
                int cnt = 0;
                for (int p = 0; p < n; ++p)
                    if (pointInTri(a, b, c, p)) ++cnt;
                insideCnt[a][b][c] = cnt;
                tris[m].a = a; tris[m].b = b; tris[m].c = c;
                tris[m].area2 = crossArea(a, b, c);
                ++m;
            }
    // 按面积从大到小处理：任何一步转移都是把大三角形换成更小的一个，天然满足拓扑序
    sort(tris, tris + m, cmpTri);

    // 初始三角形必须包住所有点，否则画到某个点时只能连出少于 3 条边，答案为 0
    if (insideCnt[tris[0].a][tris[0].b][tris[0].c] != n) {
        printf("0\n");
        return 0;
    }

    dp[tris[0].a][tris[0].b][tris[0].c] = 1;
    long long ans = 0;
    for (int idx = 0; idx < m; ++idx) {
        int a = tris[idx].a, b = tris[idx].b, c = tris[idx].c;
        long long cur = dp[a][b][c];
        if (cur == 0) continue;
        // 把 T 当成最初三点画出的三角形：T 内部点数 - 3 个内部点可以随意插在之后的 N-3 个位置里
        ans = (ans + perm[n - 3][insideCnt[a][b][c] - 3] * cur) % MOD;
        // 反向扩展：T 变大成 T'（把一个顶点换成 T 内的点 p），是原过程的逆操作
        for (int p = 0; p < n; ++p) {
            if (p == a || p == b || p == c) continue;
            if (!pointInTri(a, b, c, p)) continue;
            for (int j = 0; j < 3; ++j) {
                int t[3] = {a, b, c};
                t[j] = p;
                sort(t, t + 3);
                int d = t[0], e = t[1], f = t[2];
                int dIn = insideCnt[d][e][f];
                // 位于 T' 外(y 个)的点顺序已由 dp[T'] 定好，T' 的新顶点必须最先画，
                // 中间还夹着 insideCnt[T']-dIn-1 个在 T 外但在 T' 内的点，与 y 个点混排
                dp[d][e][f] = (dp[d][e][f] + perm[n - dIn - 1][insideCnt[a][b][c] - dIn - 1] * cur) % MOD;
            }
        }
    }
    printf("%lld\n", ans * 6 % MOD);  // 最初三点的 3! 种顺序
    return 0;
}
