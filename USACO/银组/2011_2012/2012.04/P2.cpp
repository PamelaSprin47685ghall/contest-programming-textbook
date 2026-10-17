#include <cstdio>

using namespace std;

// N <= 2000，直接枚举每层书架的第一本书，O(N^2) 足够
const int MAXN = 2000;
const long long INF = 1000000000000000000LL;   // 用来标记“还不可达”

int height[MAXN + 1];
long long widthPrefix[MAXN + 1];   // 前 i 本书的总宽度
long long best[MAXN + 1];          // best[i]：前 i 本书摆好的最小总高度

int main() {
    int n = 0;
    long long limit = 0;
    scanf("%d %lld", &n, &limit);
    for (int i = 1; i <= n; ++i) {
        long long w = 0;
        scanf("%d %lld", &height[i], &w);
        widthPrefix[i] = widthPrefix[i - 1] + w;
    }

    best[0] = 0;
    for (int i = 1; i <= n; ++i) {
        best[i] = INF;
        // 最后一层书架放第 j..i 本书，宽度用前缀和一步算出。
        // j 从 i 往前扫，宽度只会变大，超过 L 就可以停，j 再小也装不下。
        int shelfHeight = 0;
        for (int j = i; j >= 1; --j) {
            if (widthPrefix[i] - widthPrefix[j - 1] > limit) break;
            if (height[j] > shelfHeight) shelfHeight = height[j];
            long long candidate = best[j - 1] + shelfHeight;
            if (candidate < best[i]) best[i] = candidate;
        }
    }

    printf("%lld\n", best[n]);
    return 0;
}
