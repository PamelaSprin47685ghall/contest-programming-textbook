#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Constraint {
    std::string cow1;
    std::string cow2;
};

int main() {
    int n;
    if (!(std::cin >> n)) return 0;

    std::vector<Constraint> constraints(n);
    for (int i = 0; i < n; ++i) {
        std::string junk;
        std::cin >> constraints[i].cow1 >> junk >> junk >> junk >> junk >> constraints[i].cow2;
    }

    // 8 头奶牛的名字
    std::vector<std::string> cows = {
        "Bessie", "Buttercup", "Belinda", "Beatrice",
        "Bella", "Blue", "Betsy", "Sue"
    };

    // 按照字典序从小到大排序, 保证全排列搜索最先得到字典序最小的方案
    std::sort(cows.begin(), cows.end());

    do {
        bool valid = true;
        for (const Constraint& c : constraints) {
            // 找到两头牛在当前排列中的位置
            int pos1 = -1, pos2 = -1;
            for (int i = 0; i < 8; ++i) {
                if (cows[i] == c.cow1) pos1 = i;
                if (cows[i] == c.cow2) pos2 = i;
            }
            if (std::abs(pos1 - pos2) != 1) {
                valid = false;
                break;
            }
        }

        if (valid) {
            for (int i = 0; i < 8; ++i) {
                std::cout << cows[i] << "\n";
            }
            return 0;
        }
    } while (std::next_permutation(cows.begin(), cows.end()));

    return 0;
}
