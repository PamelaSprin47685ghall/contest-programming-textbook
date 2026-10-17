#include <algorithm>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

char Buffer[80];

// 三个名字排序后作为键, 统计每种三人组出现的次数.
struct StringsHash {
    size_t operator()(const vector<string>& words) const {
        size_t answer = words.size();
        for (const string& word : words) {
            answer = answer * size_t(9875321) + word.size();
            for (char c : word) answer = answer * size_t(9875321) + c;
        }
        return answer;
    }
};

struct StringsEq {
    bool operator()(const vector<string>& a,
                    const vector<string>& b) const {
        return a == b;
    }
};

int main() {
    int n;
    scanf("%d", &n);

    unordered_map<vector<string>, int, StringsHash, StringsEq>
        groupCount;
    for (int i = 0; i < n; ++i) {
        vector<string> names(3);
        for (int j = 0; j < 3; ++j) {
            scanf("%s", Buffer);
            names[j] = Buffer;
        }
        sort(names.begin(), names.end());
        ++groupCount[names];
    }

    int answer = 0;
    for (const auto& entry : groupCount) {
        answer = max(answer, entry.second);
    }
    printf("%d\n", answer);
    return 0;
}