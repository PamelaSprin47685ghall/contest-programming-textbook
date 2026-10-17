#include <cstdio>

using namespace std;

const int MAXN = 7505;

int n;
long long mod;     // 题目给定的模数 M（质数）
long long expMod;  // M-1：由费马小定理，底数的指数只需在模 M-1 下计算

char composite[MAXN];  // 埃氏筛：标记合数

// 快速幂
long long powMod(long long base, long long exponent) {
    long long result = 1;
    base %= mod;
    while (exponent > 0) {
        if (exponent & 1) result = result * base % mod;
        base = base * base % mod;
        exponent >>= 1;
    }
    return result;
}

int main() {
    scanf("%d%lld", &n, &mod);
    expMod = mod - 1;

    // 指数一律模 M-1 计算，先把 n! 在模 M-1 下算好
    long long factorialExp = 1;
    for (int i = 1; i <= n; ++i) factorialExp = factorialExp * i % expMod;

    long long answer = 1;
    for (int prime = 2; prime <= n; ++prime) {
        if (composite[prime]) continue;
        for (int multiple = 2 * prime; multiple <= n; multiple += prime) composite[multiple] = 1;

        // 枚举这个素数的每个幂 power = prime^k：周期能被 power 整除当且仅当存在长度被 power 整除的循环，
        // 这样的 power 每存在一个，答案里就多乘一个 prime
        for (long long power = prime; power <= n; power *= prime) {
            // 没有长度被 power 整除的循环的排列数：由生成函数 a_n = n!/(d!·power^d)·Π(power·i-1)（d=⌊n/power⌋）
            // 化简成"遇到 power 的倍数就把该因子换成 i-1"，结果只在模 M-1 下需要
            long long without = 1;
            for (int i = 1; i <= n; ++i) {
                if (i % power == 0) without = without * (i - 1) % expMod;
                else without = without * i % expMod;
            }
            // 存在长度被 power 整除的循环的排列数 = n! - without
            long long exponent = (factorialExp - without + expMod) % expMod;
            answer = answer * powMod(prime, exponent) % mod;
        }
    }

    printf("%lld\n", answer);
    return 0;
}
