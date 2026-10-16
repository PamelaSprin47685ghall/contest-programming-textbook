#include <algorithm>
#include <cstdio>
#include <vector>

int main() {
    int n;
    scanf("%d", &n);
    std::vector<int> heights(n);
    for (int i = 0; i < n; ++i) scanf("%d", &heights[i]);
    // 排序后的序列是目标顺序.
    std::vector<int> sorted(heights);
    std::sort(sorted.begin(), sorted.end());
    // 统计与目标顺序不同的位置个数.
    // 这些错位位置构成一个轮换, 一个大小为 k 的轮换只需 k-1 次对换.
    int misplaced = 0;
    for (int i = 0; i < n; ++i) misplaced += (heights[i] != sorted[i]);
    if (misplaced == 0) {
        printf("0\n");
    } else {
        printf("%d\n", misplaced - 1);
    }
    return 0;
}