#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    int n;
    std::string s;
    if (!(std::cin >> n >> s)) return 0;

    std::vector<int> ones;
    for (int i = 0; i < n; ++i) {
        if (s[i] == '1') ones.push_back(i);
    }

    // 全为 0 的特殊情况: 放在两端 0 和 n-1
    if (ones.empty()) {
        std::cout << n - 1 << "\n";
        return 0;
    }

    // 计算已有 1 之间的最小距离
    int minExistingDist = n;
    for (std::size_t i = 1; i < ones.size(); ++i) {
        minExistingDist = std::min(minExistingDist, ones[i] - ones[i - 1]);
    }

    // 二分答案 D: 能否放置 2 头新牛, 使得所有牛两两距离至少为 D
    auto canPlace = [&](int d) {
        int placed = 0;

        // 1. 最左侧区间 [0, ones[0]]
        placed += ones[0] / d;

        // 2. 中间区间 [ones[i-1], ones[i]]
        for (std::size_t i = 1; i < ones.size(); ++i) {
            int gap = ones[i] - ones[i - 1];
            if (gap >= 2 * d) {
                placed += (gap / d) - 1;
            }
        }

        // 3. 最右侧区间 [ones.back(), n-1]
        placed += (n - 1 - ones.back()) / d;

        return placed >= 2;
    };

    int low = 1, high = minExistingDist, bestD = 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (canPlace(mid)) {
            bestD = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    std::cout << bestD << "\n";
    return 0;
}
