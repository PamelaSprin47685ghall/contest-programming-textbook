#include <algorithm>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <vector>

char Buffer[80];

// 三个名字排序后作为键, 统计每种三人组出现的次数.
struct StringsHash {
    size_t operator()(const std::vector<std::string>& words) const {
        size_t answer = words.size();
        for (const std::string& word : words) {
            answer = answer * size_t(9875321) + word.size();
            for (char c : word) answer = answer * size_t(9875321) + c;
        }
        return answer;
    }
};

struct StringsEq {
    bool operator()(const std::vector<std::string>& a,
                    const std::vector<std::string>& b) const {
        return a == b;
    }
};

int main() {
    int n;
    scanf("%d", &n);

    std::unordered_map<std::vector<std::string>, int, StringsHash, StringsEq>
        groupCount;
    for (int i = 0; i < n; ++i) {
        std::vector<std::string> names(3);
        for (int j = 0; j < 3; ++j) {
            scanf("%s", Buffer);
            names[j] = Buffer;
        }
        std::sort(names.begin(), names.end());
        ++groupCount[names];
    }

    int answer = 0;
    for (const auto& entry : groupCount) {
        answer = std::max(answer, entry.second);
    }
    printf("%d\n", answer);
    return 0;
}