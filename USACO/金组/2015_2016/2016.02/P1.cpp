#include <cstdio>

using namespace std;

const int MAXN = 100005;   // n 的上限

int n;
long long c[MAXN];   // c[i]: 在 0 号门之后第 i 个门口排队的牛数

// 1^2 + 2^2 + ... + v^2: 距离恰好是一段连续整数时, 一次就能算出平方和
long long sumn2(long long v) {
    if (v <= 0) return 0;
    return v * (v + 1) * (2 * v + 1) / 6;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%lld", &c[i]);
    }

    // 记 x[i] 为"必须从 i-1 号房间走进 i 号房间"的牛数(下标循环), 由守恒
    // x[i+1] - x[i] = c[i] - 1 可知整个 x 只能整体平移。x 必须非负, 于是取平移量
    // 使 min(x) = 0: 前缀和最小的那个位置就是 x = 0 的切点, 之后一路 x >= 0。
    long long pref = 0, bestPref = 0;   // pref 是"以 0 号门为起点"时的 x[i]
    int start = 0;
    for (int i = 0; i < n; i++) {
        // 严格更小才更新, 于是 start 取到的是第一个最小值处, 该处 x 恰为 0
        if (pref < bestPref) {
            bestPref = pref;
            start = i;
        }
        pref += c[i] - 1;
    }

    long long ans = 0;
    long long carry = 0;   // 切点处 x = 0, carry 就是当前的 x[i]
    for (int t = 0; t < n; t++) {
        int i = (start + t) % n;
        // 在门口 i 进来的 c[i] 头牛, 分别要走到 x[i], x[i]+1, ..., x[i]+c[i]-1 步远;
        // 这些"落点"在环上首尾相接, 恰好每个房间被一头牛占一次, 所以是最优分配。
        ans += sumn2(carry + c[i] - 1) - sumn2(carry - 1);
        carry += c[i] - 1;
    }
    printf("%lld\n", ans);
    return 0;
}
