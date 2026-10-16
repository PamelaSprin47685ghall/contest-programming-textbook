#include <algorithm>
#include <cstdio>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

const char* CowName[7] = {"Bessie",   "Elsie",  "Daisy",     "Gertie",
                          "Annabelle", "Maggie", "Henrietta"};

char Buffer[80];

int main() {
    int n;
    scanf("%d", &n);
    std::unordered_map<std::string, int> milk;
    for (int i = 0; i < n; ++i) {
        int amount;
        scanf("%s%d", Buffer, &amount);
        milk[Buffer] += amount;
    }
    // 先找最低产奶量, 第二低只能在其余牛里找.
    int lowest = std::numeric_limits<int>::max();
    for (int i = 0; i < 7; ++i) {
        lowest = std::min(lowest, milk[CowName[i]]);
    }
    int second_lowest = std::numeric_limits<int>::max();
    for (int i = 0; i < 7; ++i) {
        int amount = milk[CowName[i]];
        if (amount != lowest) {
            second_lowest = std::min(second_lowest, amount);
        }
    }
    std::vector<int> candidates;
    for (int i = 0; i < 7; ++i) {
        if (milk[CowName[i]] == second_lowest) {
            candidates.emplace_back(i);
        }
    }
    // 第二低只有一头牛才输出名字, 否则并列.
    if (candidates.size() != 1) {
        printf("Tie\n");
    } else {
        printf("%s\n", CowName[candidates[0]]);
    }
    return 0;
}
