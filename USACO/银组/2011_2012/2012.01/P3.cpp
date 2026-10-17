#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 25005;

int n;

struct Cow {
    int up, down;
} cows[MAXN];

// 贪心顺序: 先放"上坡比下坡快"(up < down) 的牛, 按上坡时间递增;
// 再放其余牛, 按下坡时间递减. 交换相邻两头不合此序的牛总不会让答案变差.
bool BetterOrder(const Cow& a, const Cow& b) {
    bool aQuickUp = a.up < a.down;
    bool bQuickUp = b.up < b.down;
    if (aQuickUp != bQuickUp) return aQuickUp;
    if (aQuickUp) return a.up < b.up;
    return a.down > b.down;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d%d", &cows[i].up, &cows[i].down);
    sort(cows, cows + n, BetterOrder);

    long long reachedTop = 0;                          // 当前这头牛登顶的时刻
    long long finished = 0;                            // 前 i 头牛全部下山的时刻
    for (int i = 0; i < n; i++) {
        reachedTop += cows[i].up;                      // 只有一头牛能同时上山
        // 下山农民既受限于这头牛何时登顶, 也受限于自己上一趟何时结束
        finished = max(finished, reachedTop) + cows[i].down;
    }
    printf("%lld\n", finished);
    return 0;
}
