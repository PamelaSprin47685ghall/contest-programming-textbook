#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

char Buffer[1000001];

// 判断 s 是否以 prefix 开头.
bool HasPrefix(const std::string& s, const std::string& prefix) {
    return s.substr(0, prefix.size()) == prefix;
}

struct Word {
    std::string text;
    int id; // 输入中的原始编号
};

bool operator<(const Word& a, const Word& b) { return a.text < b.text; }

struct Words {
    std::vector<Word> data;

    void Read(int w) {
        data.resize(w);
        for (int i = 0; i < w; ++i) {
            scanf("%s", Buffer);
            data[i].text = Buffer;
            data[i].id = i + 1;
        }
        std::sort(data.begin(), data.end());
    }

    // 返回以 prefix 为前缀的第 k 个单词的编号; 不存在则返回 -1.
    int Query(int k, const std::string& prefix) {
        Word target{prefix, 0};
        int index =
            std::lower_bound(data.begin(), data.end(), target) - data.begin();
        index += k - 1;
        if (index < (int)data.size() &&
            HasPrefix(data[index].text, prefix)) {
            return data[index].id;
        }
        return -1;
    }
};

int main() {
    int w, n;
    scanf("%d%d", &w, &n);
    Words words;
    words.Read(w);
    for (int i = 0; i < n; ++i) {
        int k;
        scanf("%d%s", &k, Buffer);
        printf("%d\n", words.Query(k, Buffer));
    }
    return 0;
}