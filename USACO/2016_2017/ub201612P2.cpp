#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

char Buffer[80];

// 统计一个单词里 26 个小写字母各出现几次.
std::vector<int> CountLetters(const std::string& word) {
    std::vector<int> counts(26);
    for (char letter : word) {
        ++counts[letter - 'a'];
    }
    return counts;
}

// 同一块木板的两面都要写, 所以每个字母至少要有两词中较多的那个数量.
std::vector<int> MaxPerLetter(const std::vector<int>& first,
                              const std::vector<int>& second) {
    std::vector<int> answer(26);
    for (int i = 0; i < 26; ++i) {
        answer[i] = std::max(first[i], second[i]);
    }
    return answer;
}

void AddCounts(std::vector<int>& total, const std::vector<int>& extra) {
    for (int i = 0; i < 26; ++i) {
        total[i] += extra[i];
    }
}

int main() {
    int n;
    scanf("%d", &n);
    std::vector<int> answer(26);
    for (int i = 0; i < n; ++i) {
        scanf("%s", Buffer);
        std::string first = Buffer;
        scanf("%s", Buffer);
        std::string second = Buffer;
        // 每块木板贡献它自己的最大需求, 最后把所有木板累加.
        AddCounts(answer, MaxPerLetter(CountLetters(first), CountLetters(second)));
    }
    for (int count : answer) {
        printf("%d\n", count);
    }
    return 0;
}
