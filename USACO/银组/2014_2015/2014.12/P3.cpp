#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 100005;

int main() {
    int n;
    long long minutes;
    scanf("%d %lld", &n, &minutes);

    // freeEnd[i]：第 i 头牛若一路不被挡住，T 分钟后应处的位置。位置和速度最大 10^9，
    // 落在 10^18 量级，必须用 long long。
    vector<long long> freeEnd(n);
    for (int i = 0; i < n; ++i) {
        long long pos, speed;
        scanf("%lld %lld", &pos, &speed);
        freeEnd[i] = pos + speed * minutes;
    }

    // 跑道只有一条，后面的牛不能超过前面的牛，因此第 i 头牛的实际终点
    // 等于 min(自己的自由终点, 前面所有牛实际终点中最靠后的那个)。位置按升序
    // 给出，下标 i+1 的牛就在下标 i 的牛前面。
    // 从最前面的牛往回想：若第 i 头牛的自由终点比这个"前面最靠后的实际终点"
    // 还小，说明它会落到所有前面的牛后面，无法与任何牛会合，只能自己形成一组；
    // 否则它必然追上前面最慢的那一组并并进去，不产生新组。
    long long groups = 1;                    // 最前面的牛必定自成一组
    long long frontFinal = freeEnd[n - 1];   // 已经扫过的（更靠前的）牛实际终点的最小值
    for (int i = n - 2; i >= 0; --i) {
        if (freeEnd[i] < frontFinal) {
            // 严格更靠后就是独立的一组；相等表示恰好并到同一位置，算同一组。
            ++groups;
            frontFinal = freeEnd[i];
        }
        // freeEnd[i] >= frontFinal 时这头牛会追上前面的组并停下，frontFinal 不变。
    }
    printf("%lld\n", groups);
    return 0;
}
