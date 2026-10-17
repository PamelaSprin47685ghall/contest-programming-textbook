#include <cstdio>

using namespace std;

// 任意 15 个连续整数中，恰好有 8 个既不是 3 的倍数也不是 5 的倍数（也就是被念出来的数），
// 并且这 8 个数出现的位置在每个长度为 15 的周期里完全一样，所以答案可以按周期直接定位。
const int PERIOD = 15;
const int PERIOD_COUNT = 8;
const int BASE[PERIOD_COUNT] = {1, 2, 4, 7, 8, 11, 13, 14};  // 第一个周期内被念出的 8 个数

int main() {
    long long n;
    scanf("%lld", &n);

    // 先确定第 N 个数落在第几个周期，再看它是周期内的第几个（0-based）。
    // 每多念 8 个数，数值就整体加一个周期 15。
    long long cycle = (n - 1) / PERIOD_COUNT;
    int indexInCycle = (int)((n - 1) % PERIOD_COUNT);

    printf("%lld\n", BASE[indexInCycle] + cycle * PERIOD);
    return 0;
}
