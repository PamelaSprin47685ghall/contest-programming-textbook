// USACO 2016 US Open Silver P1 - Field Reduction
// 思路：卖掉至多 3 头牛后，剩下点的最小/最大 x、y 只可能是「原来的第 1..4 小/大」这几个坐标之一
// （因为最多只有 3 头更极端的牛消失）。于是每条边只有 4 个候选值，枚举 4^4 = 256 个矩形，
// 逐个统计矩形外的牛数，不超过 3 就说明卖掉这些牛后该矩形够用，取合法矩形面积的最小值。
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 50005;
int n;
int x[MAXN], y[MAXN];
int ix[MAXN], iy[MAXN];

bool cmpX(int a, int b) { return x[a] < x[b]; }
bool cmpY(int a, int b) { return y[a] < y[b]; }

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d%d", &x[i], &y[i]);
        ix[i] = i;
        iy[i] = i;
    }
    sort(ix, ix + n, cmpX);
    sort(iy, iy + n, cmpY);

    // 每条边 4 个候选值；坐标允许重复，正好覆盖「删掉 3 头最极端的牛」的情形
    int loX[4], hiX[4], loY[4], hiY[4];
    for (int i = 0; i < 4; i++) {
        loX[i] = x[ix[i]];
        hiX[i] = x[ix[n - 1 - i]];
        loY[i] = y[iy[i]];
        hiY[i] = y[iy[n - 1 - i]];
    }

    long long ans = -1;  // -1 表示尚未找到任何合法矩形
    for (int a = 0; a < 4; a++)
        for (int b = 0; b < 4; b++) {
            int x1 = loX[a], x2 = hiX[b];
            if (x1 > x2) continue;
            for (int c = 0; c < 4; c++)
                for (int d = 0; d < 4; d++) {
                    int y1 = loY[c], y2 = hiY[d];
                    if (y1 > y2) continue;
                    // 统计矩形外的牛数，超过 3 头就卖不完，这个矩形不合法
                    int out = 0;
                    for (int i = 0; i < n; i++)
                        if (x[i] < x1 || x[i] > x2 || y[i] < y1 || y[i] > y2) out++;
                    if (out > 3) continue;
                    long long area = (long long)(x2 - x1) * (y2 - y1);
                    if (ans < 0 || area < ans) ans = area;
                }
        }
    printf("%lld\n", ans);
    return 0;
}
