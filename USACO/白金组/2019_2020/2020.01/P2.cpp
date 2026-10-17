#include <cstdio>

using namespace std;

const int MOD = 1000000007;
const int MAXN = 50005;
const int MAXK = 20;

int N, K;
int A[MAXN];

// ist[i][v] = ipref[i] 第 0 行第 v 列; st[i][v] = pref[i] 第 v 行所有元素之和
int ist[MAXN][MAXK];
int st[MAXN][MAXK];

// 前缀矩阵 pref 与逆前缀矩阵 ipref, 乘法只在 O(K^2) 内完成 (每步只改动一列)
int pre[MAXK][MAXK];
int ipre[MAXK][MAXK];

int Add(int a, int b) {
    int s = a + b;
    if (s >= MOD) s -= MOD;
    return s;
}

int Sub(int a, int b) {
    int s = a - b;
    if (s < 0) s += MOD;
    return s;
}

int Mul(int a, int b) {
    return (long long)a * b % MOD;
}

int main() {
    scanf("%d%d", &N, &K);
    for (int i = 0; i < N; i++) scanf("%d", &A[i]);

    for (int i = 0; i < K; i++) {
        for (int j = 0; j < K; j++) {
            pre[i][j] = 0;
            ipre[i][j] = 0;
        }
        pre[i][i] = 1;    // 空前缀的乘积是单位矩阵
        ipre[i][i] = 1;
    }

    const int inv2 = (MOD + 1) / 2;   // 2 在模意义下的逆元

    for (int i = 0; i <= N; i++) {
        // 记录前缀长度为 i 时的信息: st 用行和, ist 用第 0 行
        for (int j = 0; j < K; j++) {
            int s = 0;
            for (int k = j; k < K; k++) s = Add(s, pre[j][k]);
            st[i][j] = s;
        }
        for (int k = 0; k < K; k++) ist[i][k] = ipre[0][k];
        if (i == N) break;

        int x = A[i] - 1;   // 加入元素 x 时只有第 x 列发生变化

        // pre = pre * M_x: 第 x 列变为 原来该列的两倍加上左侧各行之和
        for (int j = 0; j <= x; j++) {
            for (int k = x; k >= j; k--) pre[j][x] = Add(pre[j][x], pre[j][k]);
        }
        // ipre = M_x^{-1} * ipre: 第 x 列除以 2, 其余行减去新第 x 行的一半
        for (int j = 0; j < x; j++) {
            for (int k = x; k < K; k++) ipre[j][k] = Sub(ipre[j][k], Mul(inv2, ipre[x][k]));
        }
        for (int k = x; k < K; k++) ipre[x][k] = Mul(inv2, ipre[x][k]);
    }

    int Q;
    scanf("%d", &Q);
    for (int t = 0; t < Q; t++) {
        int L, R;
        scanf("%d%d", &L, &R);
        // 答案 = ipref[L-1] * pref[R] 的所有元素之和 (含空子序列)
        int ans = 0;
        for (int j = 0; j < K; j++) ans = Add(ans, Mul(ist[L - 1][j], st[R][j]));
        printf("%d\n", ans);
    }
    return 0;
}
