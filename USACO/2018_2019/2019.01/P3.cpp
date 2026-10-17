#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Animal {
    std::string name;
    std::vector<std::string> traits;
};

// 计算两只动物拥有的共同特征数
int CommonTraits(const Animal& a, const Animal& b) {
    int common = 0;
    for (const std::string& t1 : a.traits) {
        for (const std::string& t2 : b.traits) {
            if (t1 == t2) {
                ++common;
                break;
            }
        }
    }
    return common;
}

int main() {
    int n;
    if (!(std::cin >> n)) return 0;

    std::vector<Animal> animals(n);
    for (int i = 0; i < n; ++i) {
        int k;
        std::cin >> animals[i].name >> k;
        animals[i].traits.resize(k);
        for (int j = 0; j < k; ++j) {
            std::cin >> animals[i].traits[j];
        }
    }

    // 任意两只动物共有的特征数量的最大值, 再加上 1 即为能回答 YES 的最大次数
    int maxCommon = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int common = CommonTraits(animals[i], animals[j]);
            if (common > maxCommon) {
                maxCommon = common;
            }
        }
    }

    std::cout << maxCommon + 1 << "\n";
    return 0;
}
