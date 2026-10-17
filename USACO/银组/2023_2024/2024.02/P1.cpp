#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 40005;                  // 单个测试点里目标数上限
const int MAXS = 4 * MAXN;               // 4N 头 cow

int n;
long long leftX;                         // 每个目标左侧两个顶点共用的 x 坐标
long long leftY[MAXS];                   // 2N 个左侧顶点的 y 坐标
long long rightLowX[MAXN], rightLowY[MAXN];    // 右下顶点：只能被正斜率射中
long long rightHighX[MAXN], rightHighY[MAXN];  // 右上顶点：只能被负斜率射中

long long pointX[MAXS], pointY[MAXS];    // 当前方向要射的顶点
long long slope[MAXS];                   // 当前方向的斜率，都存成正数
int pointCount, slopeCount;
long long cap[MAXS];                     // 每个顶点允许的最大斜率

long long posSlope[MAXS], negSlope[MAXS];
int posCount, negCount;

// b > 0 时算 floor(a / b)：C++ 的 / 向 0 取整，负数必须自己向下取整
long long floorDiv(long long a, long long b) {
    if (a >= 0) {
        return a / b;
    }
    return -((-a + b - 1) / b);
}

// 判定能不能让每头 cow 的 y 截距都不小于 guess：
// 顶点 (x, y) 允许的最大斜率是 floor((y - guess) / x)，
// 把「斜率最小的 cow」配给「允许斜率最小的顶点」一一对齐就是最优配对
bool feasible(long long guess) {
    for (int i = 0; i < pointCount; ++i) {
        cap[i] = floorDiv(pointY[i] - guess, pointX[i]);
    }
    sort(cap, cap + pointCount);
    for (int i = 0; i < pointCount; ++i) {
        if (slope[i] > cap[i]) {
            return false;
        }
    }
    return true;
}

// 返回「所有 cow 截距中的最小值」能达到的最大值
long long bestMinIntercept() {
    sort(slope, slope + slopeCount);
    long long minY = pointY[0];
    long long maxX = pointX[0];
    for (int i = 1; i < pointCount; ++i) {
        if (pointY[i] < minY) {
            minY = pointY[i];
        }
        if (pointX[i] > maxX) {
            maxX = pointX[i];
        }
    }
    // 斜率都至少为 1，截距最小值超过所有顶点里最小的 y 是不可能做到的
    long long hi = minY;
    // 无论怎么配对，每头 cow 的截距都不小于 minY - 最大斜率 * 最大 x
    long long lo = minY - slope[slopeCount - 1] * maxX;
    while (lo < hi) {
        long long mid = lo + (hi - lo + 1) / 2;
        if (feasible(mid)) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%lld", &n, &leftX);
        for (int i = 0; i < n; ++i) {
            long long y1, y2, x2;
            scanf("%lld%lld%lld", &y1, &y2, &x2);
            leftY[2 * i] = y1;
            leftY[2 * i + 1] = y2;
            rightLowX[i] = x2;
            rightLowY[i] = y1;
            rightHighX[i] = x2;
            rightHighY[i] = y2;
        }
        posCount = 0;
        negCount = 0;
        for (int i = 0; i < 4 * n; ++i) {
            long long s;
            scanf("%lld", &s);
            if (s > 0) {
                posSlope[posCount++] = s;
            } else {
                negSlope[negCount++] = -s;   // 只存绝对值，负方向靠翻转变换处理
            }
        }
        // 右下顶点只能配正斜率、右上顶点只能配负斜率，两个方向各需要 N 头 cow
        if (posCount < n || negCount < n) {
            printf("-1\n");
            continue;
        }

        // 左侧顶点：y 小的配负斜率、y 大的配正斜率。这样能同时压低负斜率 cow 的最高截距、
        // 抬高正斜率 cow 的最低截距（本题的目标就是让这两个值尽量靠拢）
        sort(leftY, leftY + 2 * n);
        int negLeftCount = negCount - n;    // 分给负斜率的左侧顶点个数

        // 负斜率方向：把顶点 (x, y) 换成 (x, -y)、斜率取相反数，
        // 就化成同一个「最大化最小截距」的问题，结果取相反数即是「最小化最大截距」
        slopeCount = negCount;
        for (int i = 0; i < n; ++i) {
            pointX[i] = rightHighX[i];
            pointY[i] = -rightHighY[i];
            slope[i] = negSlope[i];
        }
        for (int i = 0; i < negLeftCount; ++i) {
            pointX[n + i] = leftX;
            pointY[n + i] = -leftY[i];
            slope[n + i] = negSlope[n + i];
        }
        pointCount = negCount;
        long long upper = -bestMinIntercept();

        // 正斜率方向
        slopeCount = posCount;
        for (int i = 0; i < n; ++i) {
            pointX[i] = rightLowX[i];
            pointY[i] = rightLowY[i];
            slope[i] = posSlope[i];
        }
        for (int i = negLeftCount; i < 2 * n; ++i) {
            int k = n + (i - negLeftCount);
            pointX[k] = leftX;
            pointY[k] = leftY[i];
            slope[k] = posSlope[k];
        }
        pointCount = posCount;
        long long lower = bestMinIntercept();

        // 最近的 cow 由正斜率方向决定，最远的由负斜率方向决定，两者互不影响
        printf("%lld\n", upper - lower);
    }
    return 0;
}
