// P1 Circle of Cows
// 结论（官方题解）：对每个 k 都存在一组最优解，使得任意两条被选中的弦都相交
// （把两条不相交的弦按 (p1,p2),(p3,p4) -> (p1,p3),(p2,p4) 交换端点，
//  各弦的圆周距离都不会变小，且不会破坏与其他弦的相交关系，故可反复交换）。
// 于是端点沿圆周交替出现：任取一条弦作为“基准弦”，它的两端把所有候选弦
// 夹在中间。枚举所有 N(N-1)/2 条基准弦，用双指针沿同一方向贪心添加弦：
// 只有当前两条指针的圆周距离 >= 基准距离时才接受这条弦，否则前进那根
// “有机会让距离变大”的指针。这样得到以该基准距离为最小值时的最大弦数 cnt，
// 用基准距离更新 ans[cnt]；最终对 ans 做后缀最大值即得每个 k 的答案
// （因为若 k 条弦能做到最小距离 d，则任意 k'<=k 条弦也能做到 d）。
#include <cstdio>
#include <algorithm>
using namespace std;

int N, C;
int L[2005];   // 倍增：L[t] = 原位置 L[t % N] + C * (t / N)，避免取模
int ans[505];  // ans[c] = 用了 c 条弦时能达到的最大最小距离

int main() {
    scanf("%d %d", &N, &C);
    for (int i = 0; i < N; ++i) scanf("%d", &L[i]);
    for (int i = 0; i < N; ++i) L[i + N] = L[i] + C;

    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            int d = L[j] - L[i];               // 正向弧长
            if (C - d < d) d = C - d;          // 圆周距离
            int lim = C - d;                   // 允许的最大正向弧长
            int i2 = i + 1, j2 = j + 1, iend = i + N;
            int cnt = 1;                       // 基准弦本身算一条
            while (i2 < j && j2 < iend) {
                int cur = L[j2] - L[i2];       // i2 到 j2 的正向弧长（0 < cur < C）
                if (cur < d) {
                    ++j2;                      // 距离太小：让右端右移以拉大
                } else if (cur > lim) {
                    ++i2;                      // 距离太大：让左端右移以缩小
                } else {
                    ++cnt;                     // 圆周距离落在 [d, lim] 内，弦合法
                    ++i2;
                    ++j2;
                }
            }
            if (cnt <= N / 2 && d > ans[cnt]) ans[cnt] = d;
        }
    }

    for (int k = N / 2 - 1; k >= 1; --k)
        if (ans[k + 1] > ans[k]) ans[k] = ans[k + 1];

    for (int k = 1; k <= N / 2; ++k)
        printf("%d%c", ans[k], k == N / 2 ? '\n' : ' ');
    return 0;
}
