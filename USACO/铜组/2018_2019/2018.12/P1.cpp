#include <algorithm>
#include <cstdio>

// 模拟倒牛奶的过程: 把牛奶从源桶倒入目标桶, 直到源桶倒空或目标桶装满.
void Pour(int sourceCapacity, int& sourceMilk, int targetCapacity, int& targetMilk) {
    (void)sourceCapacity;
    int amount = std::min(sourceMilk, targetCapacity - targetMilk);
    sourceMilk -= amount;
    targetMilk += amount;
}

int main() {
    int capacity[3];
    int milk[3];
    for (int i = 0; i < 3; ++i) {
        scanf("%d%d", &capacity[i], &milk[i]);
    }

    // 依序进行 100 次倒牛奶操作: 0->1, 1->2, 2->0, 0->1...
    for (int step = 0; step < 100; ++step) {
        int source = step % 3;
        int target = (step + 1) % 3;
        Pour(capacity[source], milk[source], capacity[target], milk[target]);
    }

    for (int i = 0; i < 3; ++i) {
        printf("%d\n", milk[i]);
    }
    return 0;
}
