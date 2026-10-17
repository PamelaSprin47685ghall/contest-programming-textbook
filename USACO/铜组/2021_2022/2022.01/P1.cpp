#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

int main() {
    vector<string> target(3);
    vector<string> guess(3);
    char buffer[8];
    for (int i = 0; i < 3; ++i) {
        scanf("%s", buffer);
        target[i] = buffer;
    }
    for (int i = 0; i < 3; ++i) {
        scanf("%s", buffer);
        guess[i] = buffer;
    }

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
        yellow += min(targetFreq[i], guessFreq[i]);
    }

    printf("%d\n%d\n", green, yellow);
    return 0;
}
