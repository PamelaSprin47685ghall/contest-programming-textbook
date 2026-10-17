#include <cstdio>

// P2 Cow Circle
// 关键观察：两牛相撞等价于"两只牛直接穿过"，只是交换了标号；而牛的环形相对
// 顺序始终不变，所以相撞必然发生在标号相邻的两条路径之间，且每撞一次，顺时针
// 路径的标号 +1、逆时针路径的标号 -1。于是每条路径的最终标号只取决于它经历了
// 多少次碰撞，而顺时针路径 i 的碰撞次数 T = L*A + B（L = floor(2K/M) 圈整圈、
// A 为走逆时针的牛总数、B 为落在 [x_i, x_i + (2K mod M)] 内的逆时针牛数）。
// 想要求出标号 0 落在路径 i 上的概率，就是要求出"弧内 B 头、弧外 C 头走逆时针"
// 的概率和，于是用生成函数（多项式）维护弧内 / 弧外两个分布：两指针滑动时
// 弧内分布只需乘一个一次因式、弧外分布只需乘/除一个一次因式，总复杂度 O(N^2)。

using namespace std;

const int MOD = 1000000007;
const int MAXN = 5005;
const int MAXM = 1000005;

int N, M;
long long K, L, E;              // L = (2K/M) mod N；E = 2K mod M
int p[2 * MAXN];                // 每头牛走顺时针的概率（模 1e9+7 意义下）
int x[2 * MAXN];                // 位置，后面再复制一份，方便处理环绕区间
int ip[MAXN];                   // 1 - p_i 的模逆元，多项式除法要用
int cpoly[MAXN], opoly[MAXN];   // 弧内 / 弧外的概率生成函数
int clen, olen;                 // 两个多项式的项数
int resA[MAXM], resB[MAXM];     // 顺时针分支与镜像后的逆时针分支的答案

// 快速幂，用来求 (1 - p_i) 的逆元
int qpow(int base, long long e) {
    long long b = base % MOD, r = 1;
    while (e > 0) {
        if (e & 1) r = r * b % MOD;
        b = b * b % MOD;
        e >>= 1;
    }
    return (int)r;
}

// poly *= (1 - pr)x + pr：第 n 项表示"弧内恰好 n 头牛走逆时针"的概率
void mulp(int *poly, int &len, int pr) {
    if ((MOD + 1 - pr) % MOD == 0) return;   // 因子退化为常数 1
    int npr = MOD + 1 - pr;
    poly[len] = 0;
    for (int i = len - 1; i >= 0; i--) {
        poly[i + 1] = (int)((poly[i + 1] + (long long)npr * poly[i]) % MOD);
        poly[i] = (int)((long long)pr * poly[i] % MOD);
    }
    len++;
}

// poly /= (1 - pr)x + pr：有牛滑出弧内区间时只能靠整除把它"撤掉"
void divp(int *poly, int &len, int pr, int ipr) {
    if ((MOD + 1 - pr) % MOD == 0) return;   // 因子是常数 1，无需相除
    long long v = poly[len - 1];             // 当前最高次项系数，作为余数往下带
    for (int i = len - 1; i >= 1; i--) {
        long long nv = poly[i - 1];
        poly[i - 1] = (int)(v * ipr % MOD);
        v = (nv - (long long)poly[i - 1] * pr) % MOD;
        if (v < 0) v += MOD;
    }
    len--;
}

// 只处理"顺时针路径"这一半：第 i 头牛走顺时针（概率 p[i]）时它所在的路径最终
// 标号是 i + T，要落在标号 0 上就需要 T ≡ -i (mod N)。以 A = B + C 枚举
// (弧内逆时针数 B, 弧外逆时针数 C)，累加 cpoly[B] * opoly[C]。
void solveCW(int *res) {
    for (int i = 0; i < N; i++) ip[i] = qpow((MOD + 1 - p[i]) % MOD, MOD - 2);
    clen = 1; cpoly[0] = 1;
    olen = 1; opoly[0] = 1;
    for (int i = 0; i < N; i++) mulp(opoly, olen, p[i]);   // 先假装所有牛都在弧外
    int j = 0;                                             // 弧内区间右端（两指针）
    for (int i = 0; i < N; i++) {
        if (i > 0) mulp(opoly, olen, p[i - 1]);            // 第 i-1 头牛此刻已在弧外
        while (x[j] - x[i] <= E) {                         // 新滑进弧内的牛搬进 cpoly
            mulp(cpoly, clen, p[j % N]);
            divp(opoly, olen, p[j % N], ip[j % N]);
            j++;
        }
        // 路径主人自己单独乘 p[i]，它不进入"逆时针牛数"的统计
        divp(cpoly, clen, p[i], ip[i]);
        int cans = 0;
        int b = (N - i) % N;                               // 需要的 B ≡ -i - L*A (mod N)
        for (int a = 0; a < N; a++) {
            int c = a - b;                                 // 弧外逆时针牛数 C = A - B
            if (b < clen && c >= 0 && c < olen)
                cans = (int)((cans + (long long)cpoly[b] * opoly[c]) % MOD);
            b -= (int)L;
            if (b < 0) b += N;
        }
        int endp = (int)(((x[i] + K) % M + M) % M);
        res[endp] = (int)((res[endp] + (long long)p[i] * cans) % MOD);
    }
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d %d %lld", &N, &M, &K);
        L = 2 * K / M % N;
        E = 2 * K % M;
        for (int i = 0; i < N; i++) scanf("%d", &p[i]);
        for (int i = 0; i < N; i++) scanf("%d", &x[i]);
        for (int i = 0; i < M; i++) resA[i] = 0;
        for (int i = 0; i < N; i++) x[i + N] = x[i] + M;    // 复制位置，环上区间变普通区间
        solveCW(resA);
        // 逆时针分支：把整个系统沿位置 0 的直径镜像，镜像后逆时针就是顺时针；
        // 镜像里顺时针路径 i 的标号变化方向也随之翻号，所以仍用同一套 T ≡ -i 判定。
        for (int i = 0; i < N; i++) {
            x[i] = (M - x[i]) % M;
            p[i] = (MOD + 1 - p[i]) % MOD;
        }
        for (int i = 1, j = N - 1; i < j; i++, j--) {       // 镜像后要重新按位置递增排列
            int t = x[i]; x[i] = x[j]; x[j] = t;
            t = p[i]; p[i] = p[j]; p[j] = t;
        }
        for (int i = 0; i < M; i++) resB[i] = 0;
        for (int i = 0; i < N; i++) x[i + N] = x[i] + M;
        solveCW(resB);
        for (int i = 0; i < M; i++) {                       // 镜像里位置 q 对应原位置 M - q
            int v = (int)((resA[i] + resB[(M - i) % M]) % MOD);
            if (i > 0) printf(" ");
            printf("%d", v);
        }
        printf("\n");
    }
    return 0;
}
