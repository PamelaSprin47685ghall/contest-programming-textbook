#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

char Buffer[80];

// 统计一个单词里 26 个小写字母各出现几次.
vector<int> CountLetters(const string& word) {
    vector<int> counts(26);
    for (char letter : word) {
        ++counts[letter - 'a'];
    }
    return counts;
}

// 同一块木板的两面都要写, 所以每个字母至少要有两词中较多的那个数量.
vector<int> MaxPerLetter(const vector<int>& first,
                         const vector<int>& second) {
    vector<int> answer(26);
    for (int i = 0; i < 26; ++i) {
        answer[i] = max(first[i], second[i]);
    }
    return answer;
}

void AddCounts(vector<int>& total, const vector<int>& extra) {
    for (int i = 0; i < 26; ++i) {
        total[i] += extra[i];
    }
}

int main() {
    int n;
    scanf("%d", &n);
    vector<int> answer(26);
    for (int i = 0; i < n; ++i) {
        scanf("%s", Buffer);
        string first = Buffer;
        scanf("%s", Buffer);
        string second = Buffer;
        // 每块木板贡献它自己的最大需求, 最后把所有木板累加.
        AddCounts(answer, MaxPerLetter(CountLetters(first), CountLetters(second)));
    }
    for (int count : answer) {
        printf("%d\n", count);
    }
    return 0;
}
