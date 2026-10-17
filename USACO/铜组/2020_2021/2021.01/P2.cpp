#include <cstdio>

int main() {
    int n;
    scanf("%d", &n);

    int odd = 0, even = 0;
    for (int i = 0; i < n; ++i) {
        int x;
        scanf("%d", &x);
        if (x % 2 == 0) ++even;
        else ++odd;
    }

    // 贪心交替生成: 偶, 奇, 偶, 奇...
    // 两个奇数可以组合成一个偶数
    while (odd > even) {
        odd -= 2;
        even += 1;
    }

    // 如果 even > odd + 1, 多余的偶数只能合并到前面的偶数集合中
    if (even > odd + 1) {
        even = odd + 1;
    }

    printf("%d\n", even + odd);
    return 0;
}
