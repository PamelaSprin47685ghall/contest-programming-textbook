#include <algorithm>
#include <cstdio>
#include <numeric>
#include <vector>

void Solve() {
    int n;
    scanf("%d", &n);
    std::vector<int> a(n);
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
        sum += a[i];
    }

    if (sum == 0) {
        printf("0\n");
        return;
    }

    // 枚举最终剩余的元素个数 numBlocks 从 n 递减到 1
    // 如果总和 sum 能被 numBlocks 整除, 目标每个块的和为 targetSum = sum / numBlocks
    for (int numBlocks = n; numBlocks >= 1; --numBlocks) {
        if (sum % numBlocks != 0) continue;

        int targetSum = sum / numBlocks;
        int currentSum = 0;
        bool possible = true;

        for (int i = 0; i < n; ++i) {
            currentSum += a[i];
            if (currentSum == targetSum) {
                currentSum = 0;
            } else if (currentSum > targetSum) {
                possible = false;
                break;
            }
        }

        if (possible && currentSum == 0) {
            // 合并操作数 = 原元素个数 - 最终块数
            printf("%d\n", n - numBlocks);
            return;
        }
    }
}

int main() {
    int t;
    if (scanf("%d", &t) == 1) {
        while (t--) {
            Solve();
        }
    }
    return 0;
}
