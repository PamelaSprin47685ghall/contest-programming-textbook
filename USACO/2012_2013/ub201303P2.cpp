#include <algorithm>
#include <cstdio>
#include <unordered_map>

int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    std::unordered_map<int, int> lastIndex;  // 每个品种上一次出现的下标.
    int answer = -1;
    for (int i = 0; i < n; ++i) {
        int breed;
        scanf("%d", &breed);
        auto result = lastIndex.emplace(breed, i);
        if (!result.second) {
            int& previous = result.first->second;
            if (i - previous <= k) answer = std::max(answer, breed);
            previous = i;
        }
    }
    printf("%d\n", answer);
    return 0;
}