#include <cstdio>

using namespace std;

const int MOD = 1000000007; // 概率对 1e9+7 取模
const int MAXN = 5000;      // 细胞数上限

int n;              // 细胞数
int cumS[MAXN + 1]; // 前缀和 cumS[i] = s_0 + ... + s_{i-1}，配合半开区间 [l, r) 求区间和

// winProb[l][r]：把区间 [l, r)（l < r，半开区间）看成一格细胞时，
// 它最终"拿下"整排标签的概率里、由 [l, r) 自己那一步合并贡献的部分。
// 与官方 subtask 2 的 DP 方向相反：从整段 [0, n) 出发，把标签一路传递到更小的区间，
// 于是所有细胞共用同一张表，不必对每个 c 重算。
// 关键观察：不管 [l, r) 里最后合并出的标签是谁，只要它接下去不被吃掉，
// 对最终标签的贡献都一样，所以可以这样递推：
//   winProb[l][r] = ( sum_{p: sum(p,l) <= sum(l,r)} winProb[p][r]
//                   + sum_{q: sum(l,r) > sum(r,q)} winProb[l][q] ) / (|[l,r)| - 1)
// 其中 |[l, r)| - 1 = r - l 是区间 [l, r) 里可能的最后一次合并数，除法即乘逆元。
//（递推式中的下标写法沿用官方代码的 0-based，与题面 1-based 编号只差一个平移。）
// 该表 (n+1)^2 个 int，n = 5000 时约 100MB，在 512MB 限制下安全。
int winProb[MAXN + 1][MAXN + 1];

// 下面四个一维数组把官方 subtask 4 的两张 pL/pR 表和两张区间和表压成一维：
// 固定右端点 r 时，左边能合并进来的最小左端 lPtr[r]（官方 pL）随枚举的左端点 l
// 单调不减；固定左端点 l 时，右边能吞进来的最大右端 rPtr[l]（官方 pR）随枚举的
// 右端点 r 单调不增，因此各用一个双指针即可，边算边用。
// lSum[r] 维护 sum_{p in [lPtr[r], 当前 l)} winProb[p][r]，
// rSum[l] 维护 sum_{q in (当前 r, rPtr[l]]} winProb[l][q]。
// 因为枚举顺序是"l 递增、r 递减"，winProb 的值是按 p 递增、q 递减的顺序加入的，
// 且双指针只会朝一个方向扫过已加入的元素，所以用两个普通前缀和（而非树状数组）就能 O(1) 查询。
int lSum[MAXN + 1], rSum[MAXN + 1];
int lPtr[MAXN + 1], rPtr[MAXN + 1];
int inv[MAXN + 1]; // inv[i] = 1/i (mod MOD)，即"概率 1/i"的模表示

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        int s;
        scanf("%d", &s);
        cumS[i + 1] = cumS[i] + s;
    }

    // 线性求逆元：inv[i] = -(MOD / i) * inv[MOD % i] (mod MOD)
    inv[1] = 1;
    for (int i = 2; i < n; ++i) {
        inv[i] = (long long)inv[MOD % i] * (MOD - MOD / i) % MOD;
    }

    // 基点：整段的标签按定义就是最终标签的候选，贡献为 1（它自己的 1/(n-1) 因子稍后乘）
    winProb[0][n] = 1;
    // rPtr 初值为 n：区间还很大时右端最多只能吞到 n
    for (int i = 0; i <= n; ++i) rPtr[i] = n;

    // 按"父区间先于子区间"的顺序枚举：l 递增、r 递减。
    // [p, r)（p < l）在更早的 l 轮里算完，(l, q]（q > r）在本轮更早的 r 里算完。
    for (int l = 0; l < n; ++l) {
        for (int r = n; r > l; --r) {
            // 左向合并：最后一步是把 [p, l) 并进 [l, r)。合并后标签归左半段的条件是
            // sum(l, r) >= sum(p, l)，即 sum(p, l) 不能超过 sum(l, r)。
            // l 增大时 sum(l, r) 变小、sum(p, l) 变大，条件越来越难满足，
            // 所以 lPtr[r] 只会右移：越过一个位置就从和里把它减掉。
            while (cumS[l] - cumS[lPtr[r]] > cumS[r] - cumS[l]) {
                lSum[r] -= winProb[lPtr[r]][r];
                if (lSum[r] < 0) lSum[r] += MOD;
                ++lPtr[r];
            }
            // 右向合并：最后一步是把 (r, q] 并进 [l, r)。标签归左半段要求
            // sum(l, r) > sum(r, q)，r 减小时 sum(l, r) 变小、sum(r, q) 变大，
            // 于是 rPtr[l] 只会左移，同样用减法维护区间和。
            while (cumS[r] - cumS[l] <= cumS[rPtr[l]] - cumS[r]) {
                rSum[l] -= winProb[l][rPtr[l]];
                if (rSum[l] < 0) rSum[l] += MOD;
                --rPtr[l];
            }
            // 两侧满足条件的父区间都能把标签传递给 [l, r)
            winProb[l][r] += lSum[r];
            winProb[l][r] += rSum[l];
            if (winProb[l][r] >= MOD) winProb[l][r] -= MOD;
            if (r > l + 1) {
                // [l, r) 内共 r - l 次可能的"最后一步合并"，等概率地恰为上述那一次，
                // 故乘以 1/(r - l)。单元素区间（r = l + 1）没有可选的最后一步，不乘。
                winProb[l][r] = (long long)winProb[l][r] * inv[r - l - 1] % MOD;
            }
            // 把算好的 [l, r) 加入两边的和，供更小的区间查询
            lSum[r] += winProb[l][r];
            if (lSum[r] >= MOD) lSum[r] -= MOD;
            rSum[l] += winProb[l][r];
            if (rSum[l] >= MOD) rSum[l] -= MOD;
        }
    }

    // 单元素区间 [i, i+1) 已经汇总了所有能把它带到整段的路径，即第 i + 1 号细胞的答案
    for (int i = 0; i < n; ++i) {
        printf("%d\n", winProb[i][i + 1]);
    }
    return 0;
}
