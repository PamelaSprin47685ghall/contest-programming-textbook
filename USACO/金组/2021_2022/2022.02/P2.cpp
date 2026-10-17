#include <cstdio>

using namespace std;

const int MAXN = 1000;  // 非样例测试点个数的上限 T-1

// 原题数据把答案保留到 20 位有效数字，double 只有约 16 位，所以这里用扩展精度
typedef __float128 Real;

Real prob[MAXN + 1];      // prob[i]：一次提交恰好通过 i 个非样例测试点的概率
Real cumProb[MAXN + 2];   // cumProb[i] = prob[0] + ... + prob[i-1]
Real cumScore[MAXN + 2];  // cumScore[i] = 0*prob[0] + ... + (i-1)*prob[i-1]

// 快速幂求 a^q（q 最大 1e9，避免依赖 quadmath 的 powq 库函数）
Real intPow(Real a, long long q) {
    Real res = 1;
    while (q > 0) {
        if (q & 1) res *= a;
        a *= a;
        q >>= 1;
    }
    return res;
}

// 把 v 四舍五入到 20 位有效数字后输出（不足 20 位补 0），与原题数据的格式一致
void print20(Real v) {
    int e = 0;
    Real p10 = 1;  // p10 = 10^e
    while (p10 * 10 <= v) { p10 *= 10; ++e; }
    while (p10 > v) { p10 /= 10; --e; }
    Real m = v / p10;  // 归一化到 [1, 10)
    int digit[20];
    for (int i = 0; i < 20; ++i) {
        int d = (int)m;
        if (d > 9) d = 9;  // 防御浮点误差
        if (d < 0) d = 0;
        digit[i] = d;
        m = (m - d) * 10;
    }
    // 此时 m 是"第 21 位起"的部分放大 10 倍，用它决定是否进位（五入，恰好一半时向偶数舍入）
    if (m > 5 || (m == 5 && (digit[19] & 1))) {
        int i = 19;
        while (i >= 0 && digit[i] == 9) { digit[i] = 0; --i; }
        if (i < 0) {  // 20 位全是 9：进位后成为 1 后面 19 个 0，量级加一
            digit[0] = 1;
            ++e;
        } else {
            ++digit[i];
        }
    }
    for (int i = 0; i <= e; ++i) printf("%d", digit[i]);
    printf(".");
    for (int i = e + 1; i < 20; ++i) printf("%d", digit[i]);
    printf("\n");
}

int main() {
    int T;
    long long K;
    scanf("%d%lld", &T, &K);
    int n = T - 1;  // 样例点必定通过，先摘掉它，最后把这一分加回来

    // 帕斯卡三角：每多一个随机测试点，就相当于对原分布再抛一次公平硬币
    prob[0] = 1;
    for (int t = 1; t <= n; ++t) {
        for (int i = t; i >= 1; --i) prob[i] = (prob[i] + prob[i - 1]) / 2;
        prob[0] /= 2;
    }
    for (int i = 0; i <= n; ++i) {
        cumProb[i + 1] = cumProb[i] + prob[i];
        cumScore[i + 1] = cumScore[i] + prob[i] * i;
    }
    Real totalScore = cumScore[n + 1];  // 交一次卷的期望得分 = n/2

    Real E = totalScore;  // 还剩 1 次提交机会时的期望得分（不含样例那一分）
    long long used = 1;   // 已经用掉的提交次数
    while (used < K) {
        int f = (int)E;  // 当前阈值：这一份的得分 <= f 就继续交，> f 就收手
        if (f > n) f = n;
        // 得分 <= f 的概率，以及得分 > f 的期望得分（未归一化）
        Real lower = cumProb[f + 1];
        Real upper = totalScore - cumScore[f + 1];
        if (upper == 0) break;  // 不可能拿到比 f 更高的分，再交也没用
        Real fix = upper / (1 - lower);  // 阈值保持 f 时 E 迭代的不动点
        long long left = K - used;
        if (fix <= f + 1) {
            // 永远到不了 f+1，剩下的次数直接用等比级数一次性推完
            E = fix - intPow(lower, left) * (fix - E);
            break;
        }
        // 二分出最小的步数 q，使迭代 q 次后 E 首次 >= f+1；lo 是最后一次仍 < f+1 的步数
        long long lo = 0, hi = left;
        while (lo < hi) {
            long long mid = lo + (hi - lo + 1) / 2;
            Real val = fix - intPow(lower, mid) * (fix - E);
            if (val < f + 1) lo = mid;
            else hi = mid - 1;
        }
        long long step = lo + 1;  // 再交一次阈值就会变大
        if (step > left) step = left;  // 次数不够越过 f+1，就把剩下的全部用完
        E = fix - intPow(lower, step) * (fix - E);
        used += step;
    }
    print20(E + 1);  // 加回必定通过的样例点
    return 0;
}
