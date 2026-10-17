#include <cstdio>

using namespace std;

// 把 gen_string 看作从 (0,0) 走向 (a,b) 的格路：当前点在直线 y = b/a * x
// 上或上方（叉积判定）就向右走，否则向上走。
//
// 一个好前缀 (x,y) 等价于：以 (x,y) 为右上角的矩形里，除 (x,y) 外所有点
// 都落在过原点的两条直线（斜率 y/x 与 b/a）的同一侧。
// 由这个判定可以把好点分成"在 (a,b) 射线下方"和"在射线上方"两类，
// 分别枚举 y / x 逐个筛出最左 / 最低的候选（每行至多一个候选）。
//
// 下面用官方题解的连分数式加速：f1、f2 是两条"边界射线"上的相邻格点，
// f' = (a,b)。crsBelow = cross(f1, f')，crsAbove = cross(f', f2)，
// 每次一步只改动其中一个；用除法一次跳过同一分支的连续多步，
// 循环次数即欧几里得算法的步数。
long long solve(long long a, long long b) {
    long long crsBelow = b, crsAbove = a;  // 初始 f1 = (1,0)，f2 = (0,1)
    long long ans = 0;
    bool onYAxis = true;  // f2 还停在 (0,1) 时，"f2 的倍数"最多只有一个算好点
    while (true) {
        if (crsBelow > crsAbove) {
            // 令 f1 = f1 + f2：f3 = f1 + f2 一定是好点，
            // 且当 f2 不在 y 轴上时，f2 的"下一个倍数"也被确认为好点，共 2 份
            long long mul = (crsBelow - 1) / crsAbove;
            ans += mul * (onYAxis ? 1 : 2);
            crsBelow -= mul * crsAbove;
        } else {
            // 令 f2 = f1 + f2：f3 = f1 + f2 是好点
            long long mul = crsAbove / crsBelow;
            ans += mul;
            crsAbove -= mul * crsBelow;
            onYAxis = false;
        }
        if (crsAbove == 0) break;  // (a,b) 已经是 f2 的整数倍，循环终止
    }
    // 收尾：(a,b) 在 f2 射线上，(0,0) 与 f1 之间还夹着 crsBelow 个 f2 的倍数，
    // 每个倍数 m 都孪生出 f1 + m 与 m 两个好点
    return ans + 2 * (crsBelow - 1);
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        long long a, b;
        scanf("%lld %lld", &a, &b);
        printf("%lld\n", solve(a, b));
    }
    return 0;
}
