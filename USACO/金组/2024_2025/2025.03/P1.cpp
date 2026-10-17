#include <cstdio>

using namespace std;

typedef long long ll;

const int MAXN = 2000005;   // 阶乘需要算到 (后缀可用 O 的个数) + K <= N + K <= 2N
const int MAXS = 1000005;   // T 的长度上界
const ll MOD = 1000000007;

int k, n;
ll l;
char s[MAXS];
int fact[MAXN];
int invFact[MAXN];

// 快速幂: 求 base^e mod MOD
ll power(ll base, ll e) {
    ll r = 1;
    while (e > 0) {
        if (e & 1) r = r * base % MOD;
        base = base * base % MOD;
        e >>= 1;
    }
    return r;
}

int main() {
    scanf("%d %d %lld", &k, &n, &l);
    scanf("%s", s);

    int lim = n + k;   // 组合数的上标最大不超过 总 O 数 + K
    fact[0] = 1;
    for (int i = 1; i <= lim; i++) fact[i] = (ll)fact[i - 1] * i % MOD;
    invFact[lim] = (int)power(fact[lim], MOD - 2);
    for (int i = lim; i >= 1; i--) invFact[i - 1] = (ll)invFact[i] * i % MOD;

    // 倒着做 DP 才不需要记录每个 M 已经分到几个 O: 从后往前扫时每个 O 只有"待用"这一种状态,
    // 而待用 O 的个数恒等于 (后缀 O 数 - K * 后缀 M 数), 与划分方式无关。
    // 正着扫时维护 req = "扫描到当前位置为止(含当前位置)的后缀"的 O 数 - K * M 数,
    // 于是处理到字符 i 时 req + K 恰好是它右边那部分的后缀 O 数 - K * 后缀 M 数,
    // 若 s[i] == 'M' 就把 K 个待用 O 分配给它的方案数 C(req + K, K) 乘进去。
    ll ans = 1;
    int req = 0;
    for (int i = 0; i < n; i++) {
        if (s[i] == 'M') {
            ans = ans * fact[req + k] % MOD * invFact[k] % MOD * invFact[req] % MOD;
            req += k;
        } else {
            req--;
        }
    }

    // 任何子序列都不可能跨过两个相邻的 T: 否则靠后的那个 T 里的 M 会缺 O。
    // 所以 S 的分解方案数等于 T 的方案数的 L 次方。
    printf("%lld\n", power(ans, l));
    return 0;
}
