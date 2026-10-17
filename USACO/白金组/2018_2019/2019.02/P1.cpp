#include <cstdio>

using namespace std;

const int MAXN = 1000000;

double p[MAXN];    // 每头公牛的接受概率，读入的是 1000000 倍的整数
double ratio[MAXN]; // p_i / (1 - p_i)，区间概率分解后只用到这个量

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        int scaled;
        scanf("%d", &scaled);
        p[i] = scaled * 0.000001;
        ratio[i] = p[i] / (1 - p[i]);
    }

    // 区间 [l, r) 恰好收到一封接受邀请的概率 = prod * sum，
    // 其中 prod = Π(1-p_i)，sum = Σ p_i/(1-p_i)。
    // 往右扩展一位 p' 后概率变大（P(1-p')(sum + p'/(1-p')) > P*sum）
    // 当且仅当扩展前的 sum < 1；而 sum 随扩展单调不减，
    // 所以对固定的 l，最优的右端点就是"第一个使 sum >= 1 的位置"（或者右端到底）。
    // 又因为每个 p_i > 0，l 变大时这个最优右端点不会左移，可以用双指针。
    int l = 0, r = 0;
    double prod = 1, sum = 0;
    double best = -1;
    while (l < n) {
        // 不断右扩，直到 sum 不再小于 1（此时再扩只会变小）
        while (r < n && sum < 1) {
            prod *= (1 - p[r]);
            sum += ratio[r];
            ++r;
        }
        double cur = prod * sum;
        if (cur > best) best = cur;
        // 左端点右移一位：把 p[l] 从乘积和求和里摘掉
        prod /= (1 - p[l]);
        sum -= ratio[l];
        ++l;
    }

    // 题目要求输出 1000000 倍的概率并向下取整
    printf("%d\n", (int)(1000000 * best + 1e-6));
    return 0;
}
