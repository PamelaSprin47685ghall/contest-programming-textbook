#include <cstdio>
#include <algorithm>

using namespace std;

typedef long long ll;

const int MAXN = 100005;
const ll MOD = 1000000007;
const ll INF = (ll)4e18;    // 表示"该事件永远不会发生"

int n;
ll d, a, b;
ll c[MAXN];     // 排序后的初始牛奶量数组

// 最小的 t 使 t * y >= x (x >= 0, y > 0)
ll ceilDiv(ll x, ll y) {
    if (x <= 0) return 0;
    return (x + y - 1) / y;
}

int main() {
    scanf("%d %lld", &n, &d);
    scanf("%lld %lld", &a, &b);
    for (int i = 0; i < n; i++) scanf("%lld", &c[i]);
    sort(c, c + n);

    // 双方最优策略: FJ 给最大的 A 瓶加 1, FN 从最大的 B 瓶里各拿走 1。
    // 因为给较大的数加 1 使平方和增加得更多, 而拿走 1 使平方和减少得更多(2m+1 单调)。
    // 于是每天净效果是: 排序后下标在 [l, r] 的瓶子各 +1, 其余瓶子不变。
    ll l = n - a;
    ll r = n - b - 1;

    // border: 跨越区间右端 r、值相同的整段瓶子。其中在区间内的部分(下标 ql..r)每天 +1,
    // 其余部分不变; 数值上始终保持"较小的若干个为 num, 较大的 p 个为 num+1"。
    ll ql = r, qr = r;
    while (ql > l && c[ql - 1] == c[r]) ql--;
    while (qr + 1 < n && c[qr + 1] == c[r]) qr++;
    ll num = c[r];      // border 中较小时的值
    ll p = 0;           // border 中值为 num + 1 的个数
    ll k = 0;           // 已经完整过去的天数

    while (true) {
        ll inside = r - ql + 1;     // border 中每天 +1 的元素个数
        ll len = qr - ql + 1;       // border 长度
        // border 两侧的值都是固定的(左边是 c[ql-1]+k 每天 +1, 右边 c[qr+1] 永远不变),
        // 一旦相等就说明该元素应当并入 border。先算出两个方向上"下一次并入"的天数, 直接跳过去。
        // 右扩: 最小的 t 使 num(t) >= c[qr+1], 即 p + t*inside >= (c[qr+1] - num) * len
        ll tR = (qr + 1 < n) ? ceilDiv((c[qr + 1] - num) * len - p, inside) : INF;
        // 左扩: 最小的 t 使 num(t) = c[ql-1] + k + t (左边每天涨 1, num 涨得不会更快, 必然正好相等)
        ll tL = (ql > l && qr != r) ? (len * (num - (c[ql - 1] + k)) + p) / (qr - r) : INF;
        ll t = tR < tL ? tR : tL;
        if (k + t > d) break;       // 剩下的天数不足以再发生并入, 循环外统一结算

        ll grow = t * inside + p;   // border 内这段时间共新增的牛奶总量
        num += grow / len;
        p = grow % len;
        k += t;
        while (qr + 1 < n && c[qr + 1] == num) qr++;
        while (ql > l && c[ql - 1] + k == num) ql--;
    }

    // 剩余天数里 border 不再变化, 直接算出最终形态
    ll grow = (d - k) * (r - ql + 1) + p;
    num += grow / (qr - ql + 1);
    p = grow % (qr - ql + 1);
    for (ll i = l; i < ql; i++) c[i] += d;                          // 区间内 border 左边的瓶子每天都 +1
    for (ll i = ql; i <= qr; i++) c[i] = (qr - i < p) ? num + 1 : num;  // border 内的最终值

    ll ans = 0;
    for (int i = 0; i < n; i++) {
        ll v = c[i] % MOD;
        ans = (ans + v * v) % MOD;
    }
    printf("%lld\n", ans);
    return 0;
}
