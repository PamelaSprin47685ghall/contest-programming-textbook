#include <cstdio>

using namespace std;

const int MAXN = 10005;
const int MAXP = 1300;

int N;
long long M;
int primes[MAXP];        // N 以内的全部质数
int primeCount = 0;
long long prev[MAXN];    // 处理完前若干质数后的背包
long long cur[MAXN];

int main() {
    scanf("%d %lld", &N, &M);

    bool composite[MAXN] = {false};
    for (int i = 2; i <= N; i++) {
        if (!composite[i]) {
            primes[primeCount++] = i;
            for (int j = 2 * i; j <= N; j += i) {
                composite[j] = true;
            }
        }
    }

    // dp[i][j]：只考虑前 i 个质数时，取到的质数幂之和不超过 j 的
    // 所有方案（每个质数至多取一个幂）的乘积之和。空方案乘积为 1。
    for (int j = 0; j <= N; j++) prev[j] = 1;

    for (int i = 0; i < primeCount; i++) {
        for (int j = 0; j <= N; j++) {
            // 不取当前质数
            long long value = prev[j];
            // 取当前质数的一个幂 p^e，代价 p^e，乘积乘上 p^e
            long long primePower = primes[i];
            while (primePower <= j) {
                value = (value + primePower * prev[j - (int)primePower]) % M;
                primePower *= primes[i];
            }
            cur[j] = value;
        }
        for (int j = 0; j <= N; j++) prev[j] = cur[j];
    }

    printf("%lld\n", prev[N]);
    return 0;
}
