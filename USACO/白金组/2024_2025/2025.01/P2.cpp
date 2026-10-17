#include <cstdio>
#include <algorithm>

using namespace std;

typedef long long ll;

const int MAXN = 100005;

int N;
ll p[MAXN];
int cover[MAXN + 2];

// 除数为正时的向下取整除法：C++ 的 / 向零取整，被除数为负且除不尽时要再减一
ll floorDiv(ll a, ll b) {
    ll q = a / b;
    if (a % b != 0 && a < 0) --q;
    return q;
}

// 除数为正时的向上取整除法
ll ceilDiv(ll a, ll b) {
    ll q = a / b;
    if (a % b != 0 && a > 0) ++q;
    return q;
}

// 判断恰好/至多 K 次出拳能否打破所有砖块（出拳数可行则更多次也可行，故可二分）
bool feasible(ll K) {
    // 最优方案中最多只有一个位置既不是 0 也不是 N-1（把内侧两击向外挪总能不劣）。
    // 只看两端共 K 次出拳时，位置 i 得到的力量是 K*(N-1-i) + A*(2i+1-N)，
    // 其中 A 是打在 0 的次数。N 为奇数时正中间的系数 2i+1-N 为 0：
    // 该位置的力量与 A 无关，若它不满则连同"补中间一击"的方案一起都不满
    // （补一击时该位置的力量同样只有 K*(N-1-mid)），可以直接判负。
    if (N % 2 == 1) {
        int mid = N / 2;
        if (p[mid] > K * (ll)(N - 1 - mid)) return false;
    }
    // 每个位置给出 A 的下界（系数为正）或上界（系数为负），区间内存在整数即合法
    ll lo = 0, hi = K;
    for (int i = 0; i < N; ++i) {
        ll num = p[i] - K * (ll)(N - 1 - i);
        int den = 2 * i + 1 - N;
        if (den > 0) lo = max(lo, ceilDiv(num, den));
        else if (den < 0) hi = min(hi, floorDiv(-num, -(ll)den));
    }
    if (lo <= hi) return true;
    // 界相差超过 1 说明实数区间本身就是空的，不存在任何非整数解
    if (lo > hi + 1) return false;
    // 剩下唯一可能：两端共出 K-1 次（A=hi 次打在 0，B=K-1-hi 次打在 N-1），
    // 再在某个位置 x 补一击。位置 i 还差 d 点力量时要求 |i-x| >= d，
    // 即 x 不能落进禁区 [i-d+1, i+d-1]，用差分统计被覆盖的次数即可
    if (hi < 0 || hi > K - 1) return false;
    ll A = hi, B = K - 1 - hi;
    for (int i = 0; i <= N; ++i) cover[i] = 0;
    for (int i = 0; i < N; ++i) {
        ll d = p[i] - (A * (ll)i + B * (ll)(N - 1 - i));
        if (d <= 0) continue;
        ll L = (ll)i - d + 1, R = (ll)i + d - 1;
        if (L < 0) L = 0;
        if (R > N - 1) R = N - 1;
        if (L <= R) { ++cover[L]; --cover[R + 1]; }
    }
    int cur = 0;
    for (int x = 0; x < N; ++x) {
        cur += cover[x];
        if (cur == 0) return true;  // 存在没有被任何禁区覆盖的落点
    }
    return false;
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &N);
        ll mx = 0;
        for (int i = 0; i < N; ++i) {
            scanf("%lld", &p[i]);
            if (p[i] > mx) mx = p[i];
        }
        // 上界：两端各出 ceil(mx/(N-1)) 次拳，每个位置至少得到 mx 点力量
        ll hi = 2 * ((mx + (ll)(N - 1) - 1) / (N - 1));
        ll lo = 0;
        while (lo < hi) {
            ll mid = lo + (hi - lo) / 2;
            if (feasible(mid)) hi = mid;
            else lo = mid + 1;
        }
        printf("%lld\n", lo);
    }
    return 0;
}
