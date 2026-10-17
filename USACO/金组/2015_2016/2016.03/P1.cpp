#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 50005;            // 奶牛数量上限
const long long INF = 1LL << 62;   // 大于任何合法面积, 用来表示"这个方向切不开"

int n;
struct Cow {
    int x, y;
};
Cow cow[MAXN];

// 按 x 排序; x 相同按 y 排, 让同一竖直位置上的牛聚成连续的一段
bool cmpCow(const Cow &a, const Cow &b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

int preMin[MAXN], preMax[MAXN];    // 排序后前缀 cow[0..i] 的 y 最小 / 最大值
int sufMin[MAXN], sufMax[MAXN];    // 排序后后缀 cow[i..n-1] 的 y 最小 / 最大值

// 只考虑被某条竖直线分开的分割: 枚举切点求左右两群外接矩形面积之和的最小值。
// 若所有牛 x 相同, 一个合法切点都没有, 返回 INF。
long long solveVerticalCut() {
    sort(cow, cow + n, cmpCow);

    preMin[0] = preMax[0] = cow[0].y;
    for (int i = 1; i < n; i++) {
        preMin[i] = min(preMin[i - 1], cow[i].y);
        preMax[i] = max(preMax[i - 1], cow[i].y);
    }
    sufMin[n - 1] = sufMax[n - 1] = cow[n - 1].y;
    for (int i = n - 2; i >= 0; i--) {
        sufMin[i] = min(sufMin[i + 1], cow[i].y);
        sufMax[i] = max(sufMax[i + 1], cow[i].y);
    }

    long long best = INF;
    for (int j = 1; j < n; j++) {
        // 切点必须落在两个不同的 x 之间: 若 cow[j-1].x == cow[j].x, 两个矩形会共享
        // 边界 x = cow[j].x, 违反"不许重叠"的规则, 跳到下一个真正有间隙的位置。
        if (cow[j].x == cow[j - 1].x) continue;
        long long leftW = (long long)cow[j - 1].x - cow[0].x;
        long long leftH = (long long)preMax[j - 1] - preMin[j - 1];
        long long rightW = (long long)cow[n - 1].x - cow[j].x;
        long long rightH = (long long)sufMax[j] - sufMin[j];
        best = min(best, leftW * leftH + rightW * rightH);
    }
    return best;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d%d", &cow[i].x, &cow[i].y);
    }

    // 一个围栏时的面积: 所有牛的外接矩形
    int minX = cow[0].x, maxX = cow[0].x;
    int minY = cow[0].y, maxY = cow[0].y;
    for (int i = 1; i < n; i++) {
        minX = min(minX, cow[i].x);
        maxX = max(maxX, cow[i].x);
        minY = min(minY, cow[i].y);
        maxY = max(maxY, cow[i].y);
    }
    long long total = (long long)(maxX - minX) * (maxY - minY);

    // 两个不重叠的轴平行矩形一定被一条竖直或水平直线分开; 竖直方向直接算,
    // 交换 x 与 y 后再算一次就覆盖了水平方向。
    long long best = solveVerticalCut();
    for (int i = 0; i < n; i++) {
        swap(cow[i].x, cow[i].y);
    }
    best = min(best, solveVerticalCut());

    printf("%lld\n", total - best);
    return 0;
}
