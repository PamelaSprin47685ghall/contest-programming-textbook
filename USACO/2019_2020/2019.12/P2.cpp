#include <iostream>
#include <string>
#include <unordered_set>

int main() {
    int n;
    std::string s;
    if (!(std::cin >> n >> s)) return 0;

    // 从长度 1 开始尝试连续子串的长度 k
    for (int k = 1; k <= n; ++k) {
        bool allUnique = true;
        std::unordered_set<std::string> seen;

        for (int i = 0; i + k <= n; ++i) {
            std::string sub = s.substr(i, k);
            if (seen.find(sub) != seen.end()) {
                allUnique = false;
                break;
            }
            seen.insert(sub);
        }

        if (allUnique) {
            std::cout << k << "\n";
            return 0;
        }
    }

    std::cout << n << "\n";
    return 0;
}
