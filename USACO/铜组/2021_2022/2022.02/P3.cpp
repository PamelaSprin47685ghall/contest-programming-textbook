#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

bool CanSpell(const string& word, const vector<string>& blocks) {
    int wlen = word.size();
    vector<int> p = {0, 1, 2, 3};

    // 枚举由哪几只积木分别按序拼出单词的各个字母 (全排列 4! = 24 种)
    do {
        bool match = true;
        for (int i = 0; i < wlen; ++i) {
            int blockIdx = p[i];
            char targetChar = word[i];
            // 检查积木 blockIdx 的 6 个面上是否含有 targetChar
            bool found = false;
            for (char ch : blocks[blockIdx]) {
                if (ch == targetChar) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                match = false;
                break;
            }
        }
        if (match) return true;
    } while (next_permutation(p.begin(), p.end()));

    return false;
}

int main() {
    int n;
    char buffer[16];
    if (scanf("%d", &n) != 1) return 0;

    vector<string> blocks(4);
    for (int i = 0; i < 4; ++i) {
        scanf("%s", buffer);
        blocks[i] = buffer;
    }

    for (int i = 0; i < n; ++i) {
        scanf("%s", buffer);
        string word = buffer;
        if (CanSpell(word, blocks)) {
            printf("YES\n");
        } else {
            printf("NO\n");
        }
    }

    return 0;
}
