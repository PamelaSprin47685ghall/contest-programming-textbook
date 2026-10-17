#include <cstdio>

using namespace std;

// 一次合并操作会从前缀和集合中去掉一个数，一次拆分操作会插入一个数。
// 若最终每段都是 d，则前缀和集合恰为 d, 2d, ..., S（S 为总和），
// 与初始前缀和集合的对称差大小为 N + S/d - 2 * x，
// 其中 x = #{i : d | pre[i]}，所以答案 = N + S/d - 2 * x（d 不整除 S 则无解）。
// 关键是把每个询问的 x 快速算出来。

int N;
long long pre[100005];   // 前缀和 pre[i] = a_1 + ... + a_i
int Q;
long long qry[100005];   // 询问

long long sp[20];        // S 中不超过 1e6 的质因子
int se[20];              // 它们在 S 中的指数
int sc = 0;

long long bv[8];         // 剩余部分的大质因子“取值”字典（剩余部分的约数）
int bc = 0;

// 按（小质因子指数编码, 大质因子取值编号）统计前缀和个数
// 上界：1e18 以内因数个数最多为 103680，大质因子取值最多 4 种
int cnt[103680 * 8];

long long gcdll(long long x, long long y) {
    while (y != 0) {
        long long t = x % y;
        x = y;
        y = t;
    }
    return x;
}

// 把 v 在各小质因子上的指数（截断到 S 中的指数）编码成混合进制整数
int encode(long long v) {
    int code = 0;
    for (int i = 0; i < sc; i++) {
        int e = 0;
        while (e < se[i] && v % sp[i] == 0) {
            v /= sp[i];
            e++;
        }
        code = code * (se[i] + 1) + e;
    }
    return code;
}

int findKey(long long g) {
    for (int i = 0; i < bc; i++) {
        if (bv[i] == g) return i;
    }
    return -1;
}

int main() {
    scanf("%d", &N);
    for (int i = 1; i <= N; i++) {
        long long x;
        scanf("%lld", &x);
        pre[i] = pre[i - 1] + x;
    }
    long long S = pre[N];

    // 试除掉 S 中所有不超过 1e6 的质因子；剩下的部分 R 不含小质因子，
    // 而 S <= 1e18 意味着 R 至多含两个质因子，因此无需再分解它，
    // 直接按 gcd(pre[i], R) 的取值分类即可（这样也避免了半素数无法拆分的麻烦）。
    long long R = S;
    for (long long p = 2; p <= 1000000 && p * p <= R; p++) {
        if (R % p == 0) {
            int e = 0;
            while (R % p == 0) {
                R /= p;
                e++;
            }
            sp[sc] = p;
            se[sc] = e;
            sc++;
        }
    }

    scanf("%d", &Q);
    for (int i = 0; i < Q; i++) {
        scanf("%lld", &qry[i]);
    }

    // 先收集所有可能出现的大质因子取值（前缀和的 gcd 和询问的 gcd）
    for (int i = 1; i <= N; i++) {
        long long g = gcdll(pre[i], R);
        if (g != 1 && findKey(g) < 0) {
            bv[bc] = g;
            bc++;
        }
    }
    for (int i = 0; i < Q; i++) {
        if (S % qry[i] != 0) continue;      // 不整除 S 的询问答案必为 -1
        long long g = gcdll(qry[i], R);
        if (g != 1 && findKey(g) < 0) {
            bv[bc] = g;
            bc++;
        }
    }
    int C = bc + 1;
    int numfac = 1;
    for (int i = 0; i < sc; i++) {
        numfac *= (se[i] + 1);
    }

    for (int i = 1; i <= N; i++) {
        long long g = gcdll(pre[i], R);
        int c = (g == 1) ? 0 : findKey(g) + 1;
        cnt[encode(pre[i]) * C + c]++;
    }

    // 对小质因子各维度做多维前缀和（即 SOS 的推广）：
    // 处理完后 cnt[code(m) * C + c] = 小质因子部分能被 m 整除、
    // 且大质因子取值编号恰为 c 的前缀和个数。
    int block = 1;
    for (int i = sc - 1; i >= 0; i--) {
        int stride = se[i] + 1;
        for (int code = numfac - 1; code >= 0; code--) {
            if (code / block % stride != 0) {
                int *dst = cnt + (code - block) * C;
                int *src = cnt + code * C;
                for (int c = 0; c < C; c++) {
                    dst[c] += src[c];
                }
            }
        }
        block *= stride;
    }

    for (int j = 0; j < Q; j++) {
        long long d = qry[j];
        if (S % d != 0) {
            printf("-1\n");
            continue;
        }
        long long r = gcdll(d, R);     // d 中属于大质因子的部分（r | R）
        long long m = d / r;           // d 中属于小质因子的部分（m 只含 sp 中的质因子）
        int code = encode(m);
        long long x = 0;
        for (int c = 0; c < C; c++) {
            long long g = (c == 0) ? 1 : bv[c - 1];
            if (g % r == 0) x += cnt[code * C + c];
        }
        printf("%lld\n", (long long)N + S / d - 2 * x);
    }
    return 0;
}
