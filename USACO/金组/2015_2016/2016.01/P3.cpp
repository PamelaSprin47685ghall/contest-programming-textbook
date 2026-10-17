#include <cstdio>

using namespace std;

const int MAXN = 210;

int n;
long long ax[MAXN], ay[MAXN];   // 顶点坐标, 数字可能很大, 叉积要用 long long
// 把多边形编码成序列: 偶数下标是"顶点信息", 奇数下标是边长。
// 出口(顶点 0)用 0 表示, 右转/左转的直角分别用 -1、-2 表示。
// 于是 S[0] 与 S[2n] 是出口标记, S[2k](1<=k<=n-1) 是顶点 k 的转角, S[2k+1] 是边 k->k+1 的长度。
int slen;
int S[2 * MAXN + 5];
int opt[MAXN];                   // opt[k]: 灯亮时从顶点 k 沿周长走到出口的最短距离
// lcp[i][j]: 从下标 i、j 出发的最长公共前缀(两个串都到序列末尾为止)
int lcp[2 * MAXN + 5][2 * MAXN + 5];

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%lld %lld", &ax[i], &ay[i]);

    S[0] = 0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        int k = (i + 2) % n;
        long long len = ax[i] - ax[j];
        if (len < 0) len = -len;
        long long len2 = ay[i] - ay[j];
        if (len2 < 0) len2 = -len2;
        S[2 * i + 1] = (int)(len + len2);
        // 用叉积判断拐弯方向: 顺时针多边形里, 叉积为正说明是右转(O(n) 不关心具体方向, 只要能区分两类角)
        long long cross = (ax[i] - ax[j]) * (ay[k] - ay[j]) - (ax[k] - ax[j]) * (ay[i] - ay[j]);
        S[2 * i + 2] = (cross > 0) ? -1 : -2;
    }
    S[2 * n] = 0;   // 顶点 0 就是出口, 用 0 覆盖它的转角
    slen = 2 * n + 1;

    // 出口到各顶点的最短周长距离: 先取顺时针一条, 再用另一条(经过顶点 n, 与 0 重合)松弛
    opt[0] = 0;
    for (int i = 0; i < n; i++) opt[i + 1] = opt[i] + S[2 * i + 1];
    opt[n] = 0;
    for (int i = n - 1; i >= 0; i--) {
        int alt = opt[i + 1] + S[2 * i + 1];
        if (alt < opt[i]) opt[i] = alt;
    }

    // 最长公共前缀, 用来快速判断某个"观察串"是否只可能来自一个出发点
    for (int i = slen - 1; i >= 0; i--) {
        for (int j = slen - 1; j >= 0; j--) {
            if (S[i] == S[j]) lcp[i][j] = 1 + lcp[i + 1][j + 1];
            else lcp[i][j] = 0;
        }
    }

    // 对每个出发点 i: 摸到的序列是 S[i..i+ln-1] (ln 为奇数, 以顶点信息结尾)。
    // 若这段序列在整个序列里只在某一个出发点处出现, Bessie 就知道了自己是谁。
    // 注意出发点也可能"走到序列末尾"(即走到出口)而提前结束, 这种被截断的序列更短,
    // 用 min(lcp, 剩余长度) 就能自然把它排除在等长比较之外。
    int result = 0;
    for (int i = 2; i + 2 < slen; i += 2) {
        int cost = 0;   // 已经走过的周长
        int ln = 1;
        while (true) {
            int cnt = 0;
            for (int j = 0; j < slen; j += 2) {
                int lim = slen - j;
                if (lcp[i][j] < lim) lim = lcp[i][j];
                if (lim >= ln) cnt++;
            }
            if (cnt == 1) break;
            cost += S[i + ln];   // 还没认出来, 就得再走一条边
            ln += 2;
        }
        int endVertex = (i + ln - 1) / 2;   // 认出来时所在的顶点
        int extra = cost + opt[endVertex] - opt[i / 2];
        if (extra > result) result = extra;
    }

    printf("%d\n", result);
    return 0;
}
