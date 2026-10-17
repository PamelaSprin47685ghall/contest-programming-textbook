#include <cstdio>

using namespace std;

const int MAXN = 1000;           // 画布边长上限
const int MAXC = MAXN * MAXN;    // 颜色种类上限 N^2

int n;
int art[MAXN][MAXN];             // 最终画布

// 每种颜色出现过的格子确定了它的最小外接矩形；
// 为了让"能被最先画"的颜色尽量多，总是假设画家的矩形就是这些最小矩形
int minRow[MAXC + 1], maxRow[MAXC + 1];
int minCol[MAXC + 1], maxCol[MAXC + 1];
bool appeared[MAXC + 1];         // 该颜色是否在画布上露过面
bool banned[MAXC + 1];           // 该颜色是否被否定（不可能是最先画的）

int diff[MAXN + 1][MAXN + 1];    // 二维差分：矩形四角打标记
int prefixPrev[MAXN + 1];        // 滚动前缀和：上一行
int prefixCur[MAXN + 1];         // 滚动前缀和：当前行

int main() {
    scanf("%d", &n);
    int colors = n * n;
    for (int c = 1; c <= colors; ++c) {
        minRow[c] = n;
        maxRow[c] = -1;
        minCol[c] = n;
        maxCol[c] = -1;
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int c;
            scanf("%d", &c);
            art[i][j] = c;
            if (c > 0) {
                appeared[c] = true;
                if (i < minRow[c]) minRow[c] = i;
                if (i > maxRow[c]) maxRow[c] = i;
                if (j < minCol[c]) minCol[c] = j;
                if (j > maxCol[c]) maxCol[c] = j;
            }
        }
    }
    int seenCount = 0;
    for (int c = 1; c <= colors; ++c)
        if (appeared[c]) ++seenCount;

    // 把每种颜色的最小矩形用二维差分打上标记：给矩形内部整体 +1。
    // 之后某个格子上的前缀和就是"盖住这个格子的矩形个数"
    for (int c = 1; c <= colors; ++c) {
        if (!appeared[c]) continue;
        ++diff[minRow[c]][minCol[c]];
        --diff[minRow[c]][maxCol[c] + 1];
        --diff[maxRow[c] + 1][minCol[c]];
        ++diff[maxRow[c] + 1][maxCol[c] + 1];
    }

    // 逐行求前缀和。若最终颜色为 c 的格子被至少两个矩形盖住，
    // 那么另一个矩形一定画在 c 之前，c 就不可能是最先画的
    int *prev = prefixPrev, *cur = prefixCur;
    for (int i = 0; i < n; ++i) {
        int rowSum = 0;                  // 本行差分的前缀和
        for (int j = 0; j < n; ++j) {
            rowSum += diff[i][j];
            cur[j] = prev[j] + rowSum;   // 二维前缀和公式
            int c = art[i][j];
            if (c > 0 && cur[j] >= 2) banned[c] = true;
        }
        int *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    int bannedCount = 0;
    for (int c = 1; c <= colors; ++c)
        if (appeared[c] && banned[c]) ++bannedCount;

    long long answer;
    if (seenCount == 1) {
        // 只有一种颜色露面时，它必定是最后画的（最后画的矩形一定没被盖住），
        // 因此只有它不能最先画；其余颜色可以先画、再被它完全盖掉。
        // N = 1 时整幅画只画了一次，那种颜色既是最先也是最后画的
        answer = (n == 1) ? 1 : (long long)colors - 1;
    } else {
        // 没露面的颜色都可以最先画，露过面的颜色中只有被否定的不能
        answer = (long long)colors - bannedCount;
    }
    printf("%lld\n", answer);
    return 0;
}
