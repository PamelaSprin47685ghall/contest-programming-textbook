#include <iostream>
#include <string>

int main() {
    std::string cowphabet, heard;
    if (!(std::cin >> cowphabet >> heard)) return 0;

    int pos[26] = {};
    for (int i = 0; i < 26; ++i) {
        pos[cowphabet[i] - 'a'] = i;
    }

    // 只要当前字母在字母表中的位置 <= 上一个字母的位置, 就必须唱新的一轮
    int loops = 1;
    for (std::size_t i = 1; i < heard.size(); ++i) {
        if (pos[heard[i] - 'a'] <= pos[heard[i - 1] - 'a']) {
            ++loops;
        }
    }

    std::cout << loops << "\n";
    return 0;
}
