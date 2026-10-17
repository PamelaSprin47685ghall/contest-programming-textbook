#include <cstdio>

using namespace std;

const int MAXM = 5000;  // 端点坐标的最大值

long long countA[MAXM + 1];      // 左端点等于 x 的区间个数
long long countB[MAXM + 1];      // 右端点等于 x 的区间个数
long long pairsA[2 * MAXM + 1];  // 左端点之和等于 k 的有序对个数
long long pairsB[2 * MAXM + 1];  // 右端点之和等于 k 的有序对个数

int main() {
    int N, M;
    scanf("%d%d", &N, &M);
    for (int i = 0; i < N; ++i) {
        int a, b;
        scanf("%d%d", &a, &b);
        ++countA[a];
        ++countB[b];
    }

    // 端点之和只与"某个值出现了几次"有关: 端点为 x 和 y 的有序对共
    // countA[x] * countA[y] 个. M 只有 5000, 直接枚举端点值对是 O(M^2).
    for (int x = 0; x <= M; ++x) {
        for (int y = 0; y <= M; ++y) {
            pairsA[x + y] += countA[x] * countA[y];
            pairsB[x + y] += countB[x] * countB[y];
        }
    }

    // 条件 a_i + a_j <= k <= b_i + b_j 等价于两个独立的不等式:
    // a_i + a_j <= k, 以及 b_i + b_j >= k (即 b_i + b_j <= k - 1 不成立).
    // 所以答案 = (左端点和 <= k 的有序对数) - (右端点和 <= k - 1 的有序对数),
    // 两个量都能随着 k 增大用前缀和一路累加出来.
    long long allSmallA = 0;  // 左端点和 <= 当前 k 的有序对个数
    long long tooSmallB = 0;  // 右端点和 <= 当前 k - 1 的有序对个数
    for (int k = 0; k <= 2 * M; ++k) {
        allSmallA += pairsA[k];
        printf("%lld\n", allSmallA - tooSmallB);
        tooSmallB += pairsB[k];
    }
    return 0;
}
