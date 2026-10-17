#include <cstdio>

using namespace std;

const int MAXN = 100005;
const int MOD = 1000000007;

int N;
int leftEnd[MAXN], rightEnd[MAXN];
int cover[2 * MAXN];                             // 差分数组，前缀和后表示每个位置被多少条线段覆盖
int pow2[MAXN];                                  // 预处理 2 的幂，避免反复快速幂

int main() {
    scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        scanf("%d %d", &leftEnd[i], &rightEnd[i]);
        cover[leftEnd[i]]++;
        cover[rightEnd[i]]--;
    }
    for (int i = 1; i <= 2 * N; i++) cover[i] += cover[i - 1];

    pow2[0] = 1;
    for (int i = 1; i < N; i++) pow2[i] = (int)(2LL * pow2[i - 1] % MOD);

    // 用「起点」来数复杂度：每条线段的左端点若没被别的线段盖住左邻，就开出一个新连通块。
    // 左端点为 l_i 的线段贡献一个连通块时，覆盖 (l_i-1, l_i) 的线段都不能选，
    // 也就是 cover[l_i-1] 条线段必须排除，其余 N-1-cover[l_i-1] 条线段随意。
    long long ans = 0;
    for (int i = 0; i < N; i++) {
        int blocked = cover[leftEnd[i] - 1];
        ans += pow2[N - 1 - blocked];
        if (ans >= MOD) ans -= MOD;
    }
    printf("%lld\n", ans);
    return 0;
}
