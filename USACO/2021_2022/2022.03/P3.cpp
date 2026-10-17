#include <algorithm>
#include <cstdio>
#include <vector>

int n;
std::vector<int> counts;
std::vector<std::vector<int>> recipe;

// 递归尝试合成 1 单位目标金属 targetMetal:
// 若成功合成返回 true 并实际扣除消耗的原料, 否则返回 false
bool Synthesize(int target) {
    if (counts[target] > 0) {
        counts[target]--;
        return true;
    }

    if (recipe[target].empty()) {
        return false;
    }

    // 尝试递归满足合成该金属所需的所有原料
    // 注意若中途合成失败, 需要支持回溯 (或由于每次只合成 1 单位, 贪心合成也是等价的)
    // 题目保证每个原料编号严格小于 target (无环 DAG)
    std::vector<int> consumed;
    for (int ingredient : recipe[target]) {
        if (Synthesize(ingredient)) {
            consumed.push_back(ingredient);
        } else {
            // 回退已经扣除的原料
            for (int restored : consumed) {
                counts[restored]++;
            }
            return false;
        }
    }

    return true;
}

int main() {
    scanf("%d", &n);
    counts.resize(n + 1);
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &counts[i]);
    }

    int k;
    scanf("%d", &k);
    recipe.resize(n + 1);
    for (int i = 0; i < k; ++i) {
        int l, m;
        scanf("%d%d", &l, &m);
        recipe[l].resize(m);
        for (int j = 0; j < m; ++j) {
            scanf("%d", &recipe[l][j]);
        }
    }

    // 贪心尝试一次次合成目标金属 n
    int totalProduced = counts[n];
    counts[n] = 0;  // 现有的直接计入答案

    while (Synthesize(n)) {
        ++totalProduced;
    }

    printf("%d\n", totalProduced);
    return 0;
}
