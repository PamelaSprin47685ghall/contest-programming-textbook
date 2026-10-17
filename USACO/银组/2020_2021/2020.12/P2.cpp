#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 2500;

struct Point {
    int x, y;
};

int n;
Point p[MAXN];
int sum[MAXN + 1][MAXN + 1];  // 二维前缀和，下标取 1..n

// 按真实坐标排序，用于压缩；所有 x、y 互不相同，名次即为压缩后的坐标
bool cmpX(const Point &a, const Point &b) {
    return a.x < b.x;
}

bool cmpY(const Point &a, const Point &b) {
    return a.y < b.y;
}

// 统计压缩后 x 落在 [x1,x2]、y 落在 [y1,y2] 内的奶牛数量
int rectSum(int x1, int y1, int x2, int y2) {
    return sum[x2 + 1][y2 + 1] - sum[x2 + 1][y1] - sum[x1][y2 + 1] + sum[x1][y1];
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d%d", &p[i].x, &p[i].y);
    }

    sort(p, p + n, cmpX);
    for (int i = 0; i < n; ++i) {
        p[i].x = i;
    }
    sort(p, p + n, cmpY);
    for (int i = 0; i < n; ++i) {
        p[i].y = i;
    }
    // 此时数组按 y 递增，下标区间 [i,j] 恰好对应 y 的闭区间 [p[i].y, p[j].y]

    for (int i = 0; i < n; ++i) {
        sum[p[i].x + 1][p[i].y + 1] = 1;
    }
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            sum[i][j] += sum[i - 1][j] + sum[i][j - 1] - sum[i - 1][j - 1];
        }
    }

    // 一个矩形合法当且仅当四条边上都有牛；枚举下边牛 i 与上边牛 j 之后，
    // 左边牛可任取 x 不超过 min 且在 y 区间内的牛，右边牛同理，方案数相乘
    long long ans = 1;  // 空集也要算一种
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            int xLow = min(p[i].x, p[j].x);
            int xHigh = max(p[i].x, p[j].x);
            int yLow = p[i].y;
            int yHigh = p[j].y;
            long long left = rectSum(0, yLow, xLow, yHigh);
            long long right = rectSum(xHigh, yLow, n - 1, yHigh);
            ans += left * right;
        }
    }

    printf("%lld\n", ans);
    return 0;
}
