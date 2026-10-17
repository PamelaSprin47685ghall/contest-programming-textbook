#include <cstdio>

using namespace std;

const int MAXN = 500;

int n;
int grid[MAXN + 5][MAXN + 5];
char colOk[MAXN + 5];  // 当前上下边界之间，第 k 列是否整列都满足阈值

// 统计最小值不小于 thr 的子矩形个数
long long countAtLeast(int thr) {
    long long ans = 0;
    for (int top = 0; top < n; ++top) {
        for (int bottom = top; bottom < n; ++bottom) {
            if (bottom == top) {
                for (int k = 0; k < n; ++k) {
                    colOk[k] = (grid[top][k] >= thr);
                }
            } else {
                // 上边界固定时，[top,bottom] 的合法性 = [top,bottom-1] 的合法性 && 新的一行
                for (int k = 0; k < n; ++k) {
                    colOk[k] = colOk[k] && (grid[bottom][k] >= thr);
                }
            }
            // 上下边界固定后问题退化成一维：一段长 run 的连续合法列贡献 run*(run+1)/2 个矩形，
            // 逐列累加 run 恰好等价于这个计数
            int run = 0;
            for (int k = 0; k < n; ++k) {
                if (colOk[k]) {
                    ++run;
                    ans += run;
                } else {
                    run = 0;
                }
            }
        }
    }
    return ans;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            scanf("%d", &grid[i][j]);
        }
    }
    // 最小值恰好为 100 的矩形 = 最小值 >= 100 的矩形 - 最小值 >= 101 的矩形
    printf("%lld\n", countAtLeast(100) - countAtLeast(101));
    return 0;
}
