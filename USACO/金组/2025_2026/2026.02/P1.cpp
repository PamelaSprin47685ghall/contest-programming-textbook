// P1 Balancing the Barns
// 二分答案 Y，判定能否做到 max(a) - min(b) <= Y。
// 令 x = min(b)，则每个位置 i 需要的转移次数是 f_i(x) = max(0, a_i - x - Y, x - b_i)：
// 三个约束分别是 a_i <= x+Y、次数非负、b_i >= x，取最大即可。
// 每个 f_i 都是若干线性函数的 max（凸），所以 f(x) = Σ f_i(x) 是凸函数，
// 差分 f(x+1) - f(x) 单调不降：只要在"斜率由负转零"的折点处取值即可判定。
// 多余的转移总能安全地"浪费"（挑 max(a) 所在的谷仓转移），所以只需判 min_x f(x) <= K。
#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 50005;

int n;
long long K;
long long a[MAXN], b[MAXN];
long long na[MAXN], nb[MAXN];
long long xs[2 * MAXN]; // 凸函数的所有折点位置

bool can(long long thresh) {
    // 预处理：把 a_i - b_i > thresh 的位置用最少的转移压进约束。
    // 记 d = a_i - b_i、use = ceil((d - thresh) / 2)，则恒有
    // max(0, a_i - x - thresh, x - b_i) = use + max(0, (a_i - use) - x - thresh, x - (b_i + use))，
    // 这样剩下的部分折点必然是 a_i - thresh <= b_i 的规整形状。
    long long have = K;
    for (int i = 0; i < n; i++) {
        na[i] = a[i];
        nb[i] = b[i];
        if (na[i] - nb[i] > thresh) {
            long long use = (na[i] - nb[i] - thresh + 1) / 2;
            if (use > have) return false; // 单个位置就花光预算，不可能
            na[i] -= use;
            nb[i] += use;
            have -= use;
        }
    }
    long long lowb = nb[0];
    for (int i = 1; i < n; i++)
        if (nb[i] < lowb) lowb = nb[i];
    long long abound = lowb + thresh; // max(a) 允许的上界（此时 x = min(b) = lowb）
    __int128 need = 0;                // 当前 x 处的总代价，可能超过 long long
    int m = 0;
    long long inc = 0;                // 当前 x 处的斜率
    for (int i = 0; i < n; i++) {
        xs[m++] = nb[i];              // x 越过 b_i 后斜率 +1
        if (na[i] > abound) {
            need += (__int128)(na[i] - abound);
            inc--;                    // 这一段斜率是 -1
            xs[m++] = na[i] - thresh; // x 越过 a_i - thresh 后斜率 +1
        }
    }
    if (need <= (__int128)have) return true;
    sort(xs, xs + m);
    long long lastx = lowb;
    // 只走到斜率变非负为止：之后的 x 只会让代价更大（凸性）
    for (int i = 0; i < m && inc < 0; i++) {
        need += (__int128)(xs[i] - lastx) * inc;
        if (need <= (__int128)have) return true;
        inc++;
        lastx = xs[i];
    }
    return false;
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d %lld", &n, &K);
        for (int i = 0; i < n; i++) scanf("%lld", &a[i]);
        for (int i = 0; i < n; i++) scanf("%lld", &b[i]);
        // 答案的下界：max(a) - min(b) >= -K - (1e9 + K) > -2.1e18
        long long lo = -2100000000000000000LL, hi = 2100000000000000000LL;
        while (lo < hi) {
            long long mid = lo + (hi - lo) / 2;
            if (can(mid)) hi = mid;
            else lo = mid + 1;
        }
        printf("%lld\n", lo);
    }
    return 0;
}
