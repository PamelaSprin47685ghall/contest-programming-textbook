#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> target(3);
    std::vector<std::string> guess(3);
    for (int i = 0; i < 3; ++i) std::cin >> target[i];
    for (int i = 0; i < 3; ++i) std::cin >> guess[i];

    int green = 0;
    int targetFreq[26] = {};
    int guessFreq[26] = {};

    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            if (guess[r][c] == target[r][c]) {
                ++green;
            } else {
                targetFreq[target[r][c] - 'A']++;
                guessFreq[guess[r][c] - 'A']++;
            }
        }
    }

    int yellow = 0;
    for (int i = 0; i < 26; ++i) {
        yellow += std::min(targetFreq[i], guessFreq[i]);
    }

    std::cout << green << "\n" << yellow << "\n";
    return 0;
}
