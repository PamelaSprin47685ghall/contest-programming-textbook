#include <cstdio>

using namespace std;

// 关键结论：艾尔西总共只能拿走 N/2-1 块蛋糕，而且她可以只从两端拿；
// 反过来贝西能保证艾尔西拿不到更多，于是艾尔西会挑"两端取 N/2-1 块"
// 里总和最大的一种，等价于让贝西拿到一段长度 N/2+1 的连续蛋糕中总最小的那段。
const int MAXN = 500005;

long long prefixSum[MAXN]; // prefixSum[i] 表示前 i 块蛋糕的大小之和

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        int N;
        scanf("%d", &N);
        prefixSum[0] = 0;
        for (int i = 1; i <= N; ++i) {
            long long cakeSize;
            scanf("%lld", &cakeSize);
            prefixSum[i] = prefixSum[i - 1] + cakeSize;
        }
        // 贝西最终吃到的是一段长度 N/2+1 的连续蛋糕，取其中总最小的一段
        int windowLength = N / 2 + 1;
        long long bessieCake = -1; // 负数表示还没有取到任何一段
        for (int left = 0; left + windowLength <= N; ++left) {
            long long windowSum = prefixSum[left + windowLength] - prefixSum[left];
            if (bessieCake < 0 || windowSum < bessieCake) {
                bessieCake = windowSum;
            }
        }
        long long elsieCake = prefixSum[N] - bessieCake;
        printf("%lld %lld\n", bessieCake, elsieCake);
    }
    return 0;
}
