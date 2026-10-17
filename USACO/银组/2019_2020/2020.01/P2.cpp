// USACO 2020 January Contest, Silver P2: Loan Repayment
#include <cstdio>

using namespace std;

typedef long long ll;

ll N, K, M;

// 判断 X 是否可行：把 Y 不变的一段一次性跳过，段数只有 O(sqrt(N))
bool ok(ll X) {
    ll given = 0, days = 0;
    while (days < K && given < N) {
        ll y = (N - given) / X;
        if (y < M) {
            // 之后每天只能还 M，看剩余天数够不够还完
            ll need = (N - given + M - 1) / M;
            return need <= K - days;
        }
        // 只要 g <= N - X*y，就有 (N-g)/X == y，这一段可以整体结算
        ll maxG = N - X * y;
        ll cnt = (maxG - given) / y + 1;
        if (cnt > K - days) cnt = K - days;
        given += y * cnt;
        days += cnt;
    }
    return given >= N;
}

int main() {
    scanf("%lld %lld %lld", &N, &K, &M);
    // X > N 时每天 Y 都被抬到 M，总计 K*M < N 一定失败，故答案不超过 N
    ll lo = 1, hi = N;
    while (lo < hi) {
        ll mid = lo + (hi - lo + 1) / 2;
        if (ok(mid)) lo = mid;
        else hi = mid - 1;
    }
    printf("%lld\n", lo);
    return 0;
}
