#include <cstdio>

using namespace std;

const int MAXN = 100000; // 位置 1..N，另外补上两个虚拟位置 0 与 N+1

long long money[MAXN + 2]; // money[k]：在位置 k 跳下能拿到的钱，两端固定为 0
int hull[MAXN + 2];        // 上凸壳上的位置，横坐标沿栈自底向上递增
int hullSize = 0;
int leftHull[MAXN + 2];    // 位置 i 左右两侧最近的凸壳顶点
int rightHull[MAXN + 2];

// 判断凸壳栈顶的 j 是否被 i、k 的连线压到了下方（严格下方才该弹出）
bool belowLine(int i, int j, int k) {
    // 用整数叉积比较：(k-i)*money[j] < (j-i)*money[k] + (k-j)*money[i]
    return (long long)(k - i) * money[j] < (long long)(j - i) * money[k] + (long long)(k - j) * money[i];
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        scanf("%lld", &money[i]);
    }
    money[0] = 0;
    money[n + 1] = 0;

    // 最优期望值 f*(k) 恰好等于点集 (k, money[k]) 上凸壳在 x = k 处的高度：
    // 凸壳是凹的，随机游走一步的期望不会超过当前高度（上界）；而一直抛硬币直到落在凸壳顶点上再跳下，
    // 期望恰好等于起点处的高度（下界）。点已按横坐标排好，所以一趟单调栈就得到上凸壳。
    for (int k = 0; k <= n + 1; ++k) {
        while (hullSize >= 2 && belowLine(hull[hullSize - 2], hull[hullSize - 1], k)) {
            --hullSize;
        }
        hull[hullSize++] = k;
    }

    // 记下每个位置两侧最近的凸壳顶点：凸壳顶点本身就是答案，其余位置落在两个顶点之间
    for (int j = 0; j + 1 < hullSize; ++j) {
        for (int i = hull[j] + 1; i < hull[j + 1]; ++i) {
            leftHull[i] = hull[j];
            rightHull[i] = hull[j + 1];
        }
        leftHull[hull[j]] = hull[j];
        rightHull[hull[j]] = hull[j];
    }
    leftHull[n + 1] = n + 1;
    rightHull[n + 1] = n + 1;

    for (int i = 1; i <= n; ++i) {
        int l = leftHull[i];
        int r = rightHull[i];
        if (l == r) {
            // 凸壳顶点：直接跳下就是最优，答案即 10^5 倍的 money[i]
            printf("%lld\n", 100000LL * money[i]);
        } else {
            // 在两侧顶点之间线性插值后向下取整：floor(10^5 * ((r-i)*money[l] + (i-l)*money[r]) / (r-l))
            // 先取整除再补余数部分，避免乘上 10^5 之后超出 64 位
            long long numerator = (long long)(r - i) * money[l] + (long long)(i - l) * money[r];
            long long denominator = r - l;
            long long scaled = numerator / denominator * 100000 + numerator % denominator * 100000 / denominator;
            printf("%lld\n", scaled);
        }
    }
    return 0;
}
