#include <cstdio>

using namespace std;

const int MAXT = 5000005;

int T, A, B;
char noWater[MAXT];   // noWater[i]: 还没喝水时, 能达到饱满度 i
char drunk[MAXT];     // drunk[i]: 已经喝过水后, 能达到饱满度 i

int main() {
    scanf("%d %d %d", &T, &A, &B);

    // 吃橘子、吃柠檬都只会让饱满度变大, 所以按饱满度从小到大扫一遍,
    // 扫到 i 时 noWater[i] 已经定下来, 可以放心地往外推广一步
    noWater[0] = 1;
    for (int i = 0; i <= T; i++) {
        if (!noWater[i]) continue;
        if (i + A <= T) noWater[i + A] = 1;
        if (i + B <= T) noWater[i + B] = 1;
    }

    // 喝水只会发生一次, 且发生在"还没喝过水"的时候: 从每个这样的状态折半
    for (int i = 0; i <= T; i++) {
        if (noWater[i]) drunk[i / 2] = 1;
    }

    // 喝过水之后只能继续吃水果, 同样是从小到大扫一遍即可
    for (int i = 0; i <= T; i++) {
        if (!drunk[i]) continue;
        if (i + A <= T) drunk[i + A] = 1;
        if (i + B <= T) drunk[i + B] = 1;
    }

    for (int i = T; i >= 0; i--) {
        if (noWater[i] || drunk[i]) {
            printf("%d\n", i);
            break;
        }
    }
    return 0;
}
