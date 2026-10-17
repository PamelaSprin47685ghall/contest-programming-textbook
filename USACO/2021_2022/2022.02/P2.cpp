#include <algorithm>
#include <cstdio>
#include <vector>

int main() {
    int n;
    scanf("%d", &n);
    std::vector<int> a(n);
    std::vector<int> b(n);
    for (int i = 0; i < n; ++i) scanf("%d", &a[i]);
    for (int i = 0; i < n; ++i) scanf("%d", &b[i]);

    // posInB[cow]: 奶牛 cow 在目标数组 b 中的目标位置
    std::vector<int> posInB(n + 1);
    for (int i = 0; i < n; ++i) {
        posInB[b[i]] = i;
    }

    // 任何一头牛只有当它在 a 中的相对次序落后于后面的牛在 b 中的次序时, 必须被向前调整移动
    // 从左到右遍历 a: 维护当前在 b 中已经出现过的元素的最大目标下标 maxTargetPos
    int moves = 0;

    // 实际上从右到左更好想:
    // 如果一头牛必须向左移, 那些保持不动的牛在 a 和 b 中必定构成公共子序列!
    // 逆向思考:
    // 从左往右对齐 b[j]: 如果 a[i] 还在后面, 前面的必须移走.
    // 经典贪心:
    // 在 b 从左到右按序匹配:
    // 维护一个指针 pa 指向 a 的当前可用牛, 用 moved 标记哪些牛已经被提前移走.
    std::vector<bool> moved(n + 1, false);
    int pa = 0;
    for (int pb = 0; pb < n; ++pb) {
        int targetCow = b[pb];
        while (pa < n && moved[a[pa]]) {
            ++pa;
        }
        if (pa < n && a[pa] == targetCow) {
            // 当前牛不需要移动, 本来就在最前面
            ++pa;
        } else {
            // 目标牛不在当前队头, 必须把它从后方抽出来向左移动到当前队头
            ++moves;
            moved[targetCow] = true;
        }
    }

    printf("%d\n", moves);
    return 0;
}
