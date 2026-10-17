#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 100005;

long long tails[MAXN];  // tails[k]：长度为 k+1 的不下降子序列末尾元素的最小可能值

int main() {
    int n;
    long long t;
    scanf("%d%lld", &n, &t);
    int length = 0;
    for (int i = 0; i < n; ++i) {
        long long start, speed;
        scanf("%lld%lld", &start, &speed);
        // 位置、速度、时间都可达 1e9，乘起来会超出 32 位，必须用 64 位整数
        long long finish = start + speed * t;
        // 初始位置靠后的牛 i 会追上牛 j（i < j）当且仅当 finish[i] >= finish[j]，
        // 这样的两头牛必须分在不同跑道；反过来，若一段牛的 finish 严格递增，
        // 它们就能共用一条跑道。于是最少跑道数 = finish 序列的最长不上升子序列长度，
        // 取相反数后即求最长不下降子序列，用经典贪心 + 二分。
        long long key = -finish;
        int pos = (int)(upper_bound(tails, tails + length, key) - tails);
        tails[pos] = key;
        if (pos == length) ++length;
    }
    printf("%d\n", length);
    return 0;
}
