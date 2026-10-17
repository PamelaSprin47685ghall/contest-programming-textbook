#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

bool CanSpell(const std::string& word, const std::vector<std::string>& blocks) {
    int wlen = word.size();
    std::vector<int> p = {0, 1, 2, 3};

    // 枚举由哪几只积木分别按序拼出单词的各个字母 (全排列 4! = 24 种)
    do {
        bool match = true;
        for (int i = 0; i < wlen; ++i) {
            int blockIdx = p[i];
            char targetChar = word[i];
            // 检查积木 blockIdx 的 6 个面上是否含有 targetChar
            bool found = false;
            for (char ch : blocks[blockIdx]) {
                if (ch == targetChar) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                match = false;
                break;
            }
        }
        if (match) return true;
    } while (std::next_permutation(p.begin(), p.end()));

    return false;
}

int main() {
    int n;
    if (!(std::cin >> n)) return 0;

    std::vector<std::string> blocks(4);
    for (int i = 0; i < 4; ++i) {
        std::cin >> blocks[i];
    }

    for (int i = 0; i < n; ++i) {
        std::string word;
        std::cin >> word;
        if (CanSpell(word, blocks)) {
            std::cout << "YES\n";
        } else {
            std::cout << "NO\n";
        }
    }

    return 0;
}
