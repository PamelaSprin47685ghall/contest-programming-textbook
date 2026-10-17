#include <cstdio>

using namespace std;

const int MAXN = 750;

// routes[i][j]：题面给出的 i 到 j（i < j）之间的飞行路线条数奇偶性
char routes[MAXN][MAXN];
// directFrom[j][i]：是否存在 i 到 j（i < j）的直飞。
// 按 (终点, 起点) 转置存放，这样下面重数路线时内层循环沿一行连续取数，
// 不会因为按下标跨行访问而反复丢掉整条缓存行。
char directFrom[MAXN][MAXN];
char digitLine[MAXN + 2];

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n - 1; ++i) {
        scanf("%s", digitLine);
        for (int j = i + 1; j < n; ++j) {
            routes[i][j] = digitLine[j - i - 1] - '0';
        }
    }

    int answer = 0;
    // 从右往左枚举起点：判断 (i, j) 是否直飞时，需要用到所有 i < k < j 的
    // directFrom[j][k]，而这些在 i 更大的轮次里已经确定。
    for (int i = n - 1; i >= 0; --i) {
        for (int j = i + 1; j < n; ++j) {
            // 不使用 i 到 j 这条可能存在的直飞时，从 i 到 j 的路线条数奇偶：
            // 按路线的最后一步来源 k 分类求和。k = i 时 routes[i][i] 恒为 0
            // （数组只填过上三角），所以循环从 k = i 开始也不会引入多余项。
            int withoutDirect = 0;
            for (int k = i; k < j; ++k) {
                withoutDirect ^= routes[i][k] & directFrom[j][k];
            }
            // 题面给的总数奇偶中，多出来的那一份只可能来自 i 到 j 的直飞本身
            int hasDirect = routes[i][j] ^ withoutDirect;
            directFrom[j][i] = hasDirect;
            answer += hasDirect;
        }
    }

    printf("%d\n", answer);
    return 0;
}
