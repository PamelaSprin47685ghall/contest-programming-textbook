#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

// 字典中所有单词的字母总数不超过 10^6, 所以单个单词最长也是 10^6.
const int MAXWORD = 1000005;
// 每个待补全的前缀最多 1000 个字母.
const int MAXPREF = 1005;

char WordBuf[MAXWORD];
char PrefBuf[MAXPREF];

struct Word {
    string text;  // 单词内容, 排序的依据
    int index;    // 在字典中的原始行号(从 0 开始), 输出时加 1
};

// 按字典序升序; 单词相同时按原始下标升序, 与官方答案的排序方式保持一致.
bool CompareWord(const Word& a, const Word& b) {
    if (a.text != b.text) {
        return a.text < b.text;
    }
    return a.index < b.index;
}

// 二分求出排序后第一个字典序不小于 pref 的位置.
int LowerBound(const vector<Word>& dict, const string& pref) {
    int lo = 0, hi = (int)dict.size();
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (dict[mid].text < pref) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

// 判断 word 是否以 pref 开头; 长度不够时 compare 不会返回 0, 但也显式判掉更清楚.
bool HasPrefix(const string& word, const string& pref) {
    if (word.size() < pref.size()) {
        return false;
    }
    return word.compare(0, pref.size(), pref) == 0;
}

int main() {
    int W, N;
    scanf("%d%d", &W, &N);
    vector<Word> dict(W);
    for (int i = 0; i < W; ++i) {
        scanf("%s", WordBuf);
        dict[i].text = WordBuf;
        dict[i].index = i;
    }
    sort(dict.begin(), dict.end(), CompareWord);
    for (int q = 0; q < N; ++q) {
        int k;
        scanf("%d%s", &k, PrefBuf);
        string pref = PrefBuf;
        // 以 pref 开头的单词在有序字典中是一段连续区间, 区间起点为 pos,
        // 所以第 k 个补全(如果存在)一定就在 pos + k - 1 这个位置.
        int pos = LowerBound(dict, pref) + k - 1;
        if (pos < (int)dict.size() && HasPrefix(dict[pos].text, pref)) {
            printf("%d\n", dict[pos].index + 1);
        } else {
            printf("-1\n");
        }
    }
    return 0;
}
