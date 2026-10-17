#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    int n;
    std::string s;
    if (!(std::cin >> n >> s)) return 0;

    std::vector<int> blocks;
    int cur = 0;
    for (int i = 0; i < n; ++i) {
        if (s[i] == '1') {
            ++cur;
        } else {
            if (cur > 0) blocks.push_back(cur);
            cur = 0;
        }
    }
    if (cur > 0) blocks.push_back(cur);

    if (blocks.empty()) {
        std::cout << 0 << "\n";
        return 0;
    }

    // 计算经过的天数 D 的最大可能值:
    // 对于两端受限的完整块, 长度 L = 2D + 1, 所以每个块最多经历 D <= (L - 1) / 2
    // 对于靠边界 (端点) 的块, 感染只能向单侧扩散, 长度 L = D + 1, 所以 D <= L - 1
    int minDays = n;
    int firstBlock = 0, lastBlock = (int)blocks.size() - 1;

    if (s[0] == '1') {
        minDays = std::min(minDays, blocks[0] - 1);
        firstBlock = 1;
    }
    if (s[n - 1] == '1' && lastBlock >= firstBlock) {
        minDays = std::min(minDays, blocks.back() - 1);
        lastBlock--;
    }

    for (int i = firstBlock; i <= lastBlock; ++i) {
        minDays = std::min(minDays, (blocks[i] - 1) / 2);
    }

    // 每个原本的染病源在 minDays 天内最多能扩散到 2 * minDays + 1 头牛
    int window = 2 * minDays + 1;
    int totalInitial = 0;
    for (int b : blocks) {
        totalInitial += (b + window - 1) / window;
    }

    std::cout << totalInitial << "\n";
    return 0;
}
