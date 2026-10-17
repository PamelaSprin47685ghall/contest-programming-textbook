#include <cstdio>

using namespace std;

const long long MOD = 1000000007;
const int MAXF = 64;    // 10^12 的不同质因数远少于 64 个
const int MAXDIV = 8192; // 10^12 以内的数最多有 6720 个约数

long long primeFactor[MAXF]; // N 的质因数
int primePower[MAXF];        // 每个质因数的指数
int factorCount = 0;

long long divisorList[MAXDIV]; // N 的全部约数
int divisorCount = 0;

// 快速幂：计算 base^exponent 对 MOD 取余
long long powerMod(long long base, long long exponent) {
    long long result = 1;
    long long current = base % MOD;
    while (exponent > 0) {
        if (exponent % 2 == 1) result = result * current % MOD;
        current = current * current % MOD;
        exponent /= 2;
    }
    return result;
}

int main() {
    long long n;
    scanf("%lld", &n);

    // 分解 N：试除到平方根即可，剩下的若非 1 必是质数
    long long remaining = n;
    for (long long p = 2; p * p <= remaining; ++p) {
        if (remaining % p == 0) {
            int exponent = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++exponent;
            }
            primeFactor[factorCount] = p;
            primePower[factorCount] = exponent;
            ++factorCount;
        }
    }
    if (remaining > 1) {
        primeFactor[factorCount] = remaining;
        primePower[factorCount] = 1;
        ++factorCount;
    }

    // 用质因数枚举 N 的所有约数 g
    divisorList[0] = 1;
    divisorCount = 1;
    for (int i = 0; i < factorCount; ++i) {
        int current = divisorCount;
        long long power = 1;
        for (int e = 0; e < primePower[i]; ++e) {
            power *= primeFactor[i];
            for (int j = 0; j < current; ++j) {
                divisorList[divisorCount++] = divisorList[j] * power;
            }
        }
    }

    // 累加 sum = Σ_{g|N} 2^g * φ(N/g)：先把 phi 按公式算出来，再配 2^g
    long long sum = 0;
    for (int i = 0; i < divisorCount; ++i) {
        long long g = divisorList[i];
        long long m = n / g;
        long long phi = m; // φ(m) = m * Π (1 - 1/p)，用质因数逐个修正
        for (int j = 0; j < factorCount; ++j) {
            if (m % primeFactor[j] == 0) phi = phi / primeFactor[j] * (primeFactor[j] - 1);
        }
        sum = (sum + powerMod(2, g) * (phi % MOD)) % MOD;
    }

    long long answer = (2 - n % MOD - powerMod(2, n) + sum) % MOD;
    answer = (answer % MOD + MOD) % MOD;
    printf("%lld\n", answer);
    return 0;
}
