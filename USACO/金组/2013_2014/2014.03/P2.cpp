#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100005;

int n;
double milk[MAXN];      // 每台机器的产奶量
double adjusted[MAXN];  // 减去猜测值 guess 之后的产奶量

// 判断是否存在一种拆卸方案, 使剩下的机器平均产奶量 <= guess.
// 剩余平均值 <= guess 等价于 (总和 - 被拆段的和) <= guess * (剩余台数),
// 也等价于被拆段 "每台减去 guess 后的和" >= 全部机器减去 guess 后的总和;
// 而把所有机器的值都减去 guess 后, 被拆段的和越大越容易满足, 所以只需看和最大的那一段.
bool Feasible(double guess) {
    double total = 0.0;
    for (int i = 1; i <= n; i++) {
        adjusted[i] = milk[i] - guess;
        total += adjusted[i];
    }
    // 在 2..n-1 内找一段非空连续段, 使其和尽量大
    double cur = adjusted[2];  // 以当前位置结尾的最大段和
    double best = adjusted[2];
    for (int j = 3; j <= n - 1; j++) {
        cur = max(cur + adjusted[j], adjusted[j]);
        best = max(best, cur);
    }
    // 拆掉和最大的那段后, 剩下的机器平均产奶量最小; 若它不超过 guess 则该猜测可行
    return total - best <= 0.0;
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        int v;
        scanf("%d", &v);
        milk[i] = v;
    }
    // 答案 (最小可能的剩余平均值) 必定落在 [0, 10001) 内; 二分足够多次后取中点.
    double lo = 0.0, hi = 10001.0;
    for (int it = 0; it < 100; it++) {
        double mid = (lo + hi) / 2.0;
        if (Feasible(mid)) hi = mid;
        else lo = mid;
    }
    printf("%.3f\n", (lo + hi) / 2.0);
    return 0;
}
