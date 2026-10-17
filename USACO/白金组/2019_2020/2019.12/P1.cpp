#include <cstdio>

using namespace std;

const int MAXN = 300;

int n, m;

// cover[pie][left][right]：所有"完整落在 [left,right] 内、且吃到派 pie"的奶牛里，体重的最大值。
int cover[MAXN][MAXN][MAXN];

// best[left][right]：只允许使用完整落在区间 [left,right] 内的奶牛时，合法序列的最大总体重。
int best[MAXN][MAXN];

int main() {
    scanf("%d%d", &n, &m);
    for (int i = 0; i < m; ++i) {
        int weight, left, right;
        scanf("%d%d%d", &weight, &left, &right);
        --left;
        --right;
        // 这头奶牛覆盖的每一块派都记下它，供"最后一次吃的是哪块派"这个划分使用
        for (int pie = left; pie <= right; ++pie) {
            if (weight > cover[pie][left][right]) cover[pie][left][right] = weight;
        }
    }

    // 区间向左右扩大只会允许更多奶牛，所以最大值可以沿 left-1 和 right+1 两个方向往外推。
    // left 从大到小、right 从小到大扫描，轮到 (left,right) 时它的两个来源都已扩散完毕。
    for (int pie = 0; pie < n; ++pie) {
        for (int left = pie; left >= 0; --left) {
            for (int right = pie; right < n; ++right) {
                int current = cover[pie][left][right];
                if (left > 0 && current > cover[pie][left - 1][right]) {
                    cover[pie][left - 1][right] = current;
                }
                if (right + 1 < n && current > cover[pie][left][right + 1]) {
                    cover[pie][left][right + 1] = current;
                }
            }
        }
    }

    // 按区间长度从小到大递推：算 (left,right) 时，内部所有更小的区间都已经算好。
    for (int left = n - 1; left >= 0; --left) {
        for (int right = left; right < n; ++right) {
            // 把区间切成互不重叠的两半：两边吃的是不同的派，两条合法序列接起来仍然合法
            for (int split = left; split < right; ++split) {
                int total = best[left][split] + best[split + 1][right];
                if (total > best[left][right]) best[left][right] = total;
            }
            // 也可以让某头奶牛给整个序列收尾，把她吃到的其中一块派记作 pie。
            // 在她之前被吃的派都不可能是 pie（否则 pie 早已被吃掉），
            // 于是前面的奶牛全都避开 pie，被它分成左右两段。
            for (int pie = left; pie <= right; ++pie) {
                if (cover[pie][left][right] == 0) continue;
                int total = cover[pie][left][right];
                if (pie > left) total += best[left][pie - 1];
                if (pie < right) total += best[pie + 1][right];
                if (total > best[left][right]) best[left][right] = total;
            }
        }
    }

    printf("%d\n", best[0][n - 1]);
    return 0;
}
