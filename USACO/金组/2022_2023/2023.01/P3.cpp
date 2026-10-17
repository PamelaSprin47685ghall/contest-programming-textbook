#include <cstdio>

using namespace std;

const int MAXN = 100005;
const int MAXB = 500005;   // B_i = A_i / 2 <= 5e5
const long long MOD = 1000000007;

int a[MAXN];
int b[MAXN];               // b[i] = A_i / 2，即向左、向右各穿过 i.5 的次数
long long fact[MAXB];
long long invFact[MAXB];

long long power(long long base, long long exp) {
    long long res = 1;
    while (exp > 0) {
        if (exp & 1) {
            res = res * base % MOD;
        }
        base = base * base % MOD;
        exp >>= 1;
    }
    return res;
}

long long binom(int n, int k) {
    return fact[n] * invFact[k] % MOD * invFact[n - k] % MOD;
}

int main() {
    int n;
    scanf("%d", &n);
    int maxB = 0;
    for (int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
        b[i] = a[i] / 2;
        if (b[i] > maxB) {
            maxB = b[i];
        }
    }

    fact[0] = 1;
    for (int i = 1; i <= maxB; ++i) {
        fact[i] = fact[i - 1] * i % MOD;
    }
    // 先求 maxB! 的逆元，再倒着推出所有逆阶乘，省掉每项一次快速幂
    invFact[maxB] = power(fact[maxB], MOD - 2);
    for (int i = maxB; i >= 1; --i) {
        invFact[i - 1] = invFact[i] * i % MOD;
    }

    long long ans = 1;
    for (int i = 0; i + 1 < n; ++i) {
        if (b[i] >= b[i + 1]) {
            ans = ans * binom(b[i], b[i + 1]) % MOD;
        } else {
            // 最后一个 L_{i+1} 必须紧跟 L_i，因此先扣掉它再选
            ans = ans * binom(b[i + 1] - 1, b[i] - 1) % MOD;
        }
    }
    printf("%lld\n", ans);
    return 0;
}
