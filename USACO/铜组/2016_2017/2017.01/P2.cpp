#include <algorithm>
#include <cstdio>

int main() {
    int n;
    scanf("%d", &n);
    // outcome[d] 记录手势差为 d 的对局数, 差按模 3 归约.
    int outcome[3]{};
    for (int i = 0; i < n; ++i) {
        int a, b;
        scanf("%d%d", &a, &b);
        int difference = (b + 3 - a) % 3;
        ++outcome[difference];
    }
    // 贝茜可任选手势映射, 让差 1 或差 2 的对局都能算她赢, 取较大者.
    printf("%d\n", std::max(outcome[1], outcome[2]));
    return 0;
}
