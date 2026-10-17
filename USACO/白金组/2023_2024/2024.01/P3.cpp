#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 200005;              // 奶牛数上限
const int SEG = 1 << 18;              // 线段树叶数，覆盖下标 0..SEG-1（2^18 > 2*10^5）
const long long MOD = 1000000007;
const long long INV2 = (MOD + 1) / 2; // 2 在模 MOD 下的逆元，"区间除以 2" 用乘它代替

int N;
int cowX[MAXN], cowY[MAXN]; // 每头牛的坐标（已减 1，变成 0-based，方便直接当下标）
int order[MAXN];            // 按 (x 升序, y 降序) 排好序的奶牛编号
int cntX[MAXN], cntY[MAXN]; // 每个 x / y 值上分别有多少头牛
long long pow2[MAXN];       // pow2[k] = 2^k mod MOD
long long answer;

// 三棵支持"区间乘常数、区间求和"的懒标记线段树：
//   tree 0 维护 G(y) = 2^{a(y)+b(y)}
//   tree 1 维护 H(y) = 2^{a(y)}
//   tree 2 维护 I(y) = 2^{a(y+1)+b(y)}
// 其中 a(y) = "排在当前牛之前且 y_j >= y 的牛数"，
//       b(y) = "x_j 比当前牛大且 y_j < y 的牛数"。
long long treeSum[3][2 * SEG];
long long treeLazy[3][2 * SEG];
long long leafValue[SEG]; // 建树时叶子上的初值
int queryL, queryR;       // 当前操作的区间
long long queryMul;       // 当前区间要整体乘上的常数

// 下推节点 i 的乘法标记：只有先把标记传给孩子，才允许继续往子树递归
void pushDown(int t, int i) {
    if (treeLazy[t][i] == 1) return;
    for (int c = 2 * i; c <= 2 * i + 1; ++c) {
        treeSum[t][c] = treeSum[t][c] * treeLazy[t][i] % MOD;
        treeLazy[t][c] = treeLazy[t][c] * treeLazy[t][i] % MOD;
    }
    treeLazy[t][i] = 1;
}

// 用 leafValue 作为叶子初值建树（区间和 + 懒标记清零）
void build(int t, int i, int lo, int hi) {
    treeLazy[t][i] = 1;
    if (lo == hi) {
        treeSum[t][i] = leafValue[lo];
        return;
    }
    int mid = (lo + hi) / 2;
    build(t, 2 * i, lo, mid);
    build(t, 2 * i + 1, mid + 1, hi);
    treeSum[t][i] = (treeSum[t][2 * i] + treeSum[t][2 * i + 1]) % MOD;
}

// 把区间 [queryL, queryR] 整体乘上 queryMul
void rangeMul(int t, int i, int lo, int hi) {
    if (queryR < lo || hi < queryL) return;
    if (queryL <= lo && hi <= queryR) {
        treeSum[t][i] = treeSum[t][i] * queryMul % MOD;
        treeLazy[t][i] = treeLazy[t][i] * queryMul % MOD;
        return;
    }
    pushDown(t, i);
    int mid = (lo + hi) / 2;
    rangeMul(t, 2 * i, lo, mid);
    rangeMul(t, 2 * i + 1, mid + 1, hi);
    treeSum[t][i] = (treeSum[t][2 * i] + treeSum[t][2 * i + 1]) % MOD;
}

// 区间 [queryL, queryR] 求和
long long rangeSum(int t, int i, int lo, int hi) {
    if (queryR < lo || hi < queryL) return 0;
    if (queryL <= lo && hi <= queryR) return treeSum[t][i];
    pushDown(t, i);
    int mid = (lo + hi) / 2;
    return (rangeSum(t, 2 * i, lo, mid) + rangeSum(t, 2 * i + 1, mid + 1, hi)) % MOD;
}

// 前缀 [0, r] 乘 c（r < 0 表示空区间，直接跳过）
void mulPrefix(int t, int r, long long c) {
    if (r < 0) return;
    queryL = 0;
    queryR = r;
    queryMul = c;
    rangeMul(t, 1, 0, SEG - 1);
}

// 后缀 [l, SEG-1] 乘 c（l 超过下标范围表示空区间）
void mulSuffix(int t, int l, long long c) {
    if (l > SEG - 1) return;
    queryL = l;
    queryR = SEG - 1;
    queryMul = c;
    rangeMul(t, 1, 0, SEG - 1);
}

// 前缀 [0, r] 求和（r < 0 时为空）
long long sumPrefix(int t, int r) {
    if (r < 0) return 0;
    queryL = 0;
    queryR = r;
    return rangeSum(t, 1, 0, SEG - 1);
}

// 排序：x 升序；x 相同的按 y 降序。
// 这样"字典序最大的牛"在 S 中唯一，枚举最后一头牛就不会重复计数。
bool cmpByX(int a, int b) {
    if (cowX[a] != cowX[b]) return cowX[a] < cowX[b];
    return cowY[a] > cowY[b];
}

// 垂直方向的对称排序：y 升序，y 相同的按 x 降序
bool cmpByY(int a, int b) {
    if (cowY[a] != cowY[b]) return cowY[a] < cowY[b];
    return cowX[a] > cowX[b];
}

// 减去"同时能被水平线和垂直线分开"的重复计数。
// 这里统计所有满足"S 整体在 T 左上方"（x 更小、y 更大）的有序对 (S,T)：
// 固定 S 中字典序最后的一头牛 i，记 y = min_{j in S} y_j，则
//   F_i(y) = (2^{a_i(y)} - 2^{a_i(y+1)}) * (2^{b_i(y)} - 1)   (y < y_i)
//   F_i(y_i) = 2^{a_i(y_i)} * (2^{b_i(y_i)} - 1)
// 第一项是"i 之后的 S 选法中最小 y 恰为 y"的方案数，第二项是非空 T 的选法数。
// 展开后求和得 Σ_y F_i(y) = Σ_{y<=y_i} G_i(y) - H_i(0) - Σ_{y<y_i} I_i(y)，
// 于是只需三棵支持区间乘与区间和的线段树，随 i 移动做区间乘 2 / 乘 INV2。
void fixDoubleCounting() {
    // 线段树初值：i = 0 时 a 恒为 0，b_0(y) = #{牛 j: y_j < y}，所以
    // G、I 的叶子 p 取 2^{#{y_j < p}}，H 的叶子恒为 1。
    long long cur = 1;
    for (int p = 0; p < SEG; ++p) {
        leafValue[p] = cur;
        if (p < N) cur = cur * pow2[cntY[p]] % MOD; // 把 y_j = p 的牛计入下一个叶子
    }
    build(0, 1, 0, SEG - 1);
    build(2, 1, 0, SEG - 1);
    for (int p = 0; p < SEG; ++p) leafValue[p] = 1;
    build(1, 1, 0, SEG - 1);

    for (int i = 0; i < N; ++i) {
        int cow = order[i];
        // 进入第 i 头牛：把 x 恰好等于 cowX[cow] 的牛从 b 里剔除，
        // 它们的贡献是"对 y > y_j 的 b(y) 减 1"，即后缀乘 INV2
        if (i == 0 || cowX[order[i]] > cowX[order[i - 1]]) {
            for (int j = i; j < N; ++j) {
                if (cowX[order[j]] > cowX[order[i]]) break;
                int yj = cowY[order[j]];
                mulSuffix(0, yj + 1, INV2);
                mulSuffix(2, yj + 1, INV2);
            }
        }
        // 累加 Σ_y F_i(y)
        long long sumF = sumPrefix(0, cowY[cow]);
        sumF = (sumF + MOD - sumPrefix(2, cowY[cow] - 1)) % MOD;
        sumF = (sumF + MOD - sumPrefix(1, 0)) % MOD;
        answer = (answer + MOD - sumF) % MOD;
        // 推进到下一头牛：a(y) 对所有 y <= cowY[cow] 加 1，即前缀乘 2；
        // I 里是 a(y+1)，故乘 2 的范围是 [0, cowY[cow]-1]
        mulPrefix(0, cowY[cow], 2);
        mulPrefix(1, cowY[cow], 2);
        if (cowY[cow] > 0) mulPrefix(2, cowY[cow] - 1, 2);
    }
}

int main() {
    scanf("%d", &N);
    for (int i = 0; i < N; ++i) {
        scanf("%d %d", &cowX[i], &cowY[i]);
        cowX[i]--;
        cowY[i]--;
        cntX[cowX[i]]++;
        cntY[cowY[i]]++;
        order[i] = i;
    }
    pow2[0] = 1;
    for (int i = 1; i <= N; ++i) pow2[i] = pow2[i - 1] * 2 % MOD;

    // 水平可分离（S 全在 T 左侧）：枚举 S 中 x 最大的牛 i，
    // S 的其余牛从 x 更小的 i 头里任选，T 从 x 更大的牛里任选且非空
    sort(order, order + N, cmpByX);
    for (int i = 0; i < N; ++i) {
        int cow = order[i];
        int bigger = N - i - cntX[cowX[cow]];
        answer = (answer + pow2[i] * (pow2[bigger] + MOD - 1)) % MOD;
    }

    // 垂直可分离（S 全在 T 下方）：按 y 升序（对称地）再做一遍
    sort(order, order + N, cmpByY);
    for (int i = 0; i < N; ++i) {
        int cow = order[i];
        int bigger = N - i - cntY[cowY[cow]];
        answer = (answer + pow2[i] * (pow2[bigger] + MOD - 1)) % MOD;
    }

    // 容斥：把"两条线都能分开"的方案被算重的部分减掉。
    // 一次统计 S 在 T 左上方，一次把 x 翻转后统计 S 在 T 右上方，
    // 两种朝向合起来正好覆盖所有被重复计入一次的方向。
    sort(order, order + N, cmpByX);
    fixDoubleCounting();
    for (int i = 0; i < N; ++i) cowX[i] = N - 1 - cowX[i];
    sort(order, order + N, cmpByX);
    fixDoubleCounting();

    answer = answer * 2 % MOD; // 红蓝两队可以互换
    printf("%lld\n", answer);
    return 0;
}
