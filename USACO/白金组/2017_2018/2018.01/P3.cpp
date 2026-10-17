#include <cstdio>

using namespace std;

const int MAXN = 100005;         // 边长上限
const long long MOD = 1000000007;  // 取模的模数

int N;
int col[MAXN];        // col[x]：第 x 列上那个喷头的纵坐标（每列每行各一个，构成一个排列）
int prefMin[MAXN];    // R(x) = min_{i <= x} col[i]，单调不增
int sufMax[MAXN];     // Q(x) = max_{i >= x} col[i]，单调不增
long long sumR[MAXN];   // R 的前缀和
long long sumR2[MAXN];  // R^2 的前缀和

int main() {
    scanf("%d", &N);
    for (int i = 0; i < N; ++i) {
        int x, y;
        scanf("%d %d", &x, &y);
        col[x] = y;
    }

    // 一个点 (x, y) 被浇水 ⟺ 存在喷头 (i, j) 满足 i ≤ x 且 j ≤ y；
    // 被施肥 ⟺ 存在喷头 (i, j) 满足 i ≥ x 且 j ≥ y。
    // 这两个条件分别对 x、y 单调，所以矩形 [a,b] × [c,d] 全部被浇水只需检查最“难”的
    // 西南角 (a, c)，全部被施肥只需检查东北角 (b, d)：
    //     浇水 ⟺ R(a) ≤ c，    施肥 ⟺ Q(b) ≥ d。
    prefMin[0] = col[0];
    for (int i = 1; i < N; ++i) prefMin[i] = col[i] < prefMin[i - 1] ? col[i] : prefMin[i - 1];
    sufMax[N - 1] = col[N - 1];
    for (int i = N - 2; i >= 0; --i) sufMax[i] = col[i] > sufMax[i + 1] ? col[i] : sufMax[i + 1];

    for (int i = 0; i < N; ++i) {
        sumR[i] = (i > 0 ? sumR[i - 1] : 0) + prefMin[i];
        sumR2[i] = (i > 0 ? sumR2[i - 1] : 0) + (long long)prefMin[i] * prefMin[i];
    }

    // 答案 = Σ_{a < b} #{(c,d) : c ≥ R(a), c < d ≤ Q(b)}。
    // 固定 b，记 k = Q(b)：a 必须满足 R(a) ≤ k-1（否则 c 无解），
    // 此时 (c,d) 的个数是 m(m+1)/2（m = k - R(a)，即 c ∈ [R(a), k-1] 每个 c 配 k-c 个 d）。
    // R 单调不增，所以满足 R(a) ≤ k-1 的 a 恰好是后缀 [t, b-1]，t 随 k 减小只会右移。
    long long ans = 0;
    int t = 0;
    for (int b = 1; b < N; ++b) {
        int k = sufMax[b];
        while (t < N && prefMin[t] > k - 1) ++t;  // 找到第一个 R(a) ≤ k-1 的 a
        int cnt = b - t;
        if (cnt > 0) {
            long long s1 = sumR[b - 1] - (t > 0 ? sumR[t - 1] : 0);
            long long s2 = sumR2[b - 1] - (t > 0 ? sumR2[t - 1] : 0);
            // Σ (k-R)^2 + Σ (k-R)，再除以 2，即为 Σ m(m+1)/2
            long long total = ((long long)cnt * k * k - 2LL * k * s1 + s2 + (long long)cnt * k - s1) / 2;
            ans = (ans + total) % MOD;
        }
    }
    printf("%lld\n", ans);
    return 0;
}
