#include <cstdio>
#include <algorithm>
using namespace std;

const int MOD = 1000000007;
const int MAXN = 100005;  // 线段条数上限
const int MAXK = 10;      // 幂次上限

int N, K;
int Lv[MAXN], Rv[MAXN];         // 每条线段的左右端点
int ord[MAXN];                  // 按左端点升序排序后的线段编号
int yv[MAXN + 1];               // 离散化坐标：0 以及所有右端点，升序排列
int pos[2 * MAXN + 1];          // 端点值 -> 在 yv 中的下标
long long C[MAXK + 1][MAXK + 1]; // 组合数

// 线段树每个结点存 K+1 个数：该区间内所有 dp[i][0..K] 的和（i 是右端点下标）
int tree[4 * (MAXN + 1)][MAXK + 1];
int tag[4 * (MAXN + 1)];
int m;  // 叶子个数 = N+1

bool cmpL(int a, int b) {
    return Lv[a] < Lv[b];
}

// 把整个区间的 dp 向量整体乘 f：因为每个 a_c 不变，只是把解的数量翻倍
void apply_mul(int node, int f) {
    for (int i = 0; i <= K; ++i) tree[node][i] = (long long)tree[node][i] * f % MOD;
    tag[node] = (long long)tag[node] * f % MOD;
}

// 下传标记；只在内部结点上调用，所以可以直接访问左右儿子
void push(int node) {
    if (tag[node] != 1) {
        apply_mul(2 * node, tag[node]);
        apply_mul(2 * node + 1, tag[node]);
        tag[node] = 1;
    }
}

void range_mul(int node, int L, int R, int ql, int qr, int f) {
    if (qr < L || R < ql) return;
    if (ql <= L && R <= qr) {
        apply_mul(node, f);
        return;
    }
    push(node);
    int mid = (L + R) >> 1;
    range_mul(2 * node, L, mid, ql, qr, f);
    range_mul(2 * node + 1, mid + 1, R, ql, qr, f);
    for (int i = 0; i <= K; ++i)
        tree[node][i] = (tree[2 * node][i] + tree[2 * node + 1][i]) % MOD;
}

void point_add(int node, int L, int R, int p, const long long *v) {
    if (L == R) {
        for (int i = 0; i <= K; ++i) tree[node][i] = (tree[node][i] + v[i]) % MOD;
        return;
    }
    push(node);
    int mid = (L + R) >> 1;
    if (p <= mid) point_add(2 * node, L, mid, p, v);
    else point_add(2 * node + 1, mid + 1, R, p, v);
    for (int i = 0; i <= K; ++i)
        tree[node][i] = (tree[2 * node][i] + tree[2 * node + 1][i]) % MOD;
}

void query(int node, int L, int R, int ql, int qr, long long *acc) {
    if (qr < L || R < ql) return;
    if (ql <= L && R <= qr) {
        for (int i = 0; i <= K; ++i) acc[i] = (acc[i] + tree[node][i]) % MOD;
        return;
    }
    push(node);
    int mid = (L + R) >> 1;
    query(2 * node, L, mid, ql, qr, acc);
    query(2 * node + 1, mid + 1, R, ql, qr, acc);
}

int main() {
    scanf("%d %d", &N, &K);
    for (int i = 0; i < N; ++i) scanf("%d %d", &Lv[i], &Rv[i]);

    for (int i = 0; i <= K; ++i) {
        C[i][0] = 1;
        for (int j = 1; j <= i; ++j) C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
    }

    yv[0] = 0;  // 虚拟右端点 0，表示"还没选任何线段"的集合
    for (int i = 0; i < N; ++i) yv[i + 1] = Rv[i];
    sort(yv, yv + N + 1);
    for (int i = 0; i <= N; ++i) pos[yv[i]] = i;

    for (int i = 0; i < N; ++i) ord[i] = i;
    // 按左端点升序处理，是后面"三种情况"成立的关键前提
    sort(ord, ord + N, cmpL);

    m = N + 1;
    for (int i = 0; i < 4 * m; ++i) tag[i] = 1;

    long long start[MAXK + 1];
    for (int i = 0; i <= K; ++i) start[i] = 0;
    start[0] = 1;  // 空集合：数量 1，各次幂和都是 0（除 0 次幂）
    point_add(1, 0, m - 1, 0, start);

    for (int t = 0; t < N; ++t) {
        int l = Lv[ord[t]], r = Rv[ord[t]];
        int i1 = (int)(lower_bound(yv, yv + m, l) - yv) - 1;  // 最大的 y < l
        int i2 = pos[r];

        long long A[MAXK + 1], B[MAXK + 1];
        for (int i = 0; i <= K; ++i) { A[i] = 0; B[i] = 0; }

        if (i1 >= 0) {
            // 情况 1：右端点 i < l，新线段单独成一块，a 加一后再放到 r 上
            query(1, 0, m - 1, 0, i1, A);
            long long tmp[MAXK + 1];
            for (int j = 0; j <= K; ++j) {
                long long s = 0;  // 二项式定理：sum_c (a_c+1)^j = sum_k C(j,k) dp[i][k]
                for (int k = 0; k <= j; ++k) s += C[j][k] * A[k] % MOD;
                tmp[j] = s % MOD;
            }
            for (int j = 0; j <= K; ++j) A[j] = tmp[j];
        }
        if (i1 + 1 <= i2) {
            // 情况 2：l <= i <= r，线段与新集合相接，块数不变，右端点变成 r
            query(1, 0, m - 1, i1 + 1, i2, B);
        }
        for (int j = 0; j <= K; ++j) A[j] = (A[j] + B[j]) % MOD;
        // 把 dp[r] 自身也加进来，等价于"选/不选该线段都停在 r"时 dp[r] 翻倍
        point_add(1, 0, m - 1, i2, A);
        // 情况 3：右端点 i > r，该线段被之前某条线段完全覆盖，选不选都无影响
        if (i2 + 1 <= m - 1) range_mul(1, 0, m - 1, i2 + 1, m - 1, 2);
    }

    long long res[MAXK + 1];
    for (int i = 0; i <= K; ++i) res[i] = 0;
    query(1, 0, m - 1, 0, m - 1, res);
    printf("%lld\n", res[K]);
    return 0;
}
