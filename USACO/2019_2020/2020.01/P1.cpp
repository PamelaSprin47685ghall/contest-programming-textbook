#include <iostream>
#include <string>

int main() {
    int n, k;
    if (!(std::cin >> n >> k)) return 0;

    int currentLineChars = 0;
    for (int i = 0; i < n; ++i) {
        std::string word;
        std::cin >> word;
        int len = word.size();

        if (currentLineChars + len <= k) {
            // 可以继续放在当前行
            if (currentLineChars > 0) {
                std::cout << " ";
            }
            std::cout << word;
            currentLineChars += len;
        } else {
            // 换行放置
            std::cout << "\n" << word;
            currentLineChars = len;
        }
    }
    std::cout << "\n";
    return 0;
}
