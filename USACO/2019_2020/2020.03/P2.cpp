#include <algorithm>
#include <cstdio>
#include <vector>

struct Cow {
    int x;
    int sick;
};

int main() {
    int n;
    scanf("%d", &n);
    std::vector<Cow> cows(n);
    for (int i = 0; i < n; ++i) {
        scanf("%d%d", &cows[i].x, &cows[i].sick);
    }

    std::sort(cows.begin(), cows.end(), [](const Cow& a, const Cow& b) {
        return a.x < b.x;
    });

    // 1. 确定传染半径 R 的严格上界:
    // 如果一头染病牛与一头未染病牛相邻, 它们之间的距离必严格大于 R,
    // 否则未染病牛也会被传染.
    int minGapBetweenDiff = 1000000000;
    for (int i = 0; i < n - 1; ++i) {
        if (cows[i].sick != cows[i + 1].sick) {
            minGapBetweenDiff = std::min(minGapBetweenDiff, cows[i + 1].x - cows[i].x);
        }
    }

    // 任何严格小于 minGapBetweenDiff 的正数都可以作为 R,
    // 也就是说两个染病牛如果在距离 < minGapBetweenDiff 之内, 它们可能属于同一连通块.
    // 换言之, 若距离 >= minGapBetweenDiff, 它们绝不可能互相传染, 必然源自不同的初始病牛!
    int maxR = minGapBetweenDiff - 1;

    // 2. 统计染病牛的连通块数量
    int initialSickCount = 0;
    bool inCluster = false;
    int prevSickX = -1;

    for (int i = 0; i < n; ++i) {
        if (cows[i].sick == 1) {
            if (!inCluster || (cows[i].x - prevSickX > maxR)) {
                ++initialSickCount;
            }
            inCluster = true;
            prevSickX = cows[i].x;
        } else {
            inCluster = false;
        }
    }

    printf("%d\n", initialSickCount);
    return 0;
}
