#include <algorithm>
#include <cstdio>

using namespace std;

// 解法：坐标范围太大不能逐格统计，但矩形的边界总共只有 2N 个 x、2N 个 y 坐标。
// 用这些坐标把平面切成竖长条，每个矩形要么整条覆盖一个长条、要么完全不碰它，
// 于是可以逐条用差分数组统计被覆盖的 y 长度。
const int MAXN = 1000;

int n;
long long xLow[MAXN], yLow[MAXN], xHigh[MAXN], yHigh[MAXN]; // 每个矩形的左下角与右上角
long long xs[2 * MAXN], ys[2 * MAXN];                       // 所有出现过的 x、y 坐标
int yLowIndex[MAXN], yHighIndex[MAXN];                      // 矩形上下边界在 ys 中的下标
int cover[2 * MAXN];                                        // 每个 y 小区间覆盖层数的差分量

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        long long ax, ay, bx, by;
        scanf("%lld %lld %lld %lld", &ax, &ay, &bx, &by);
        // 题面给的是左上角与右下角，先把每个矩形统一成左下、右上两个端点，
        // 后面判断覆盖时只需要比大小，不必再关心端点的方向
        xLow[i] = min(ax, bx);
        xHigh[i] = max(ax, bx);
        yLow[i] = min(ay, by);
        yHigh[i] = max(ay, by);
        xs[2 * i] = xLow[i];
        xs[2 * i + 1] = xHigh[i];
        ys[2 * i] = yLow[i];
        ys[2 * i + 1] = yHigh[i];
    }
    sort(xs, xs + 2 * n);
    sort(ys, ys + 2 * n);
    for (int i = 0; i < n; ++i) {
        yLowIndex[i] = lower_bound(ys, ys + 2 * n, yLow[i]) - ys;
        yHighIndex[i] = lower_bound(ys, ys + 2 * n, yHigh[i]) - ys;
    }

    long long area = 0;
    // 相邻两条竖线之间的横坐标区间里，矩形的左右边界都不在中间，
    // 所以"哪些矩形覆盖这一整条"是固定的，可以整条竖着算
    for (int i = 0; i + 1 < 2 * n; ++i) {
        if (xs[i] == xs[i + 1]) continue;
        for (int j = 0; j < 2 * n; ++j) cover[j] = 0;
        for (int j = 0; j < n; ++j) {
            if (xLow[j] <= xs[i] && xs[i + 1] <= xHigh[j]) {
                ++cover[yLowIndex[j]];  // 差分：从下边界起多盖一层
                --cover[yHighIndex[j]]; // 到上边界结束
            }
        }
        int layers = 0; // 当前 y 小区间被几个矩形盖住
        for (int j = 0; j + 1 < 2 * n; ++j) {
            layers += cover[j];
            if (layers > 0) {
                area += (xs[i + 1] - xs[i]) * (ys[j + 1] - ys[j]);
            }
        }
    }
    // 覆盖面积最大可达 4*10^16，必须用 long long 累加
    printf("%lld\n", area);
    return 0;
}
