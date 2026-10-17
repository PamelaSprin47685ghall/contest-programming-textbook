#include <cstdio>
#include <string>

using namespace std;

int main() {
    char buffer[1005];
    if (scanf("%s", buffer) != 1) return 0;
    string cowphabet = buffer;
    if (scanf("%s", buffer) != 1) return 0;
    string heard = buffer;

    int pos[26] = {};
    for (int i = 0; i < 26; ++i) {
        pos[cowphabet[i] - 'a'] = i;
    }

    // 只要当前字母在字母表中的位置 <= 上一个字母的位置, 就必须唱新的一轮
    int loops = 1;
    for (int i = 1; i < (int)heard.size(); ++i) {
        if (pos[heard[i] - 'a'] <= pos[heard[i - 1] - 'a']) {
            ++loops;
        }
    }

    printf("%d\n", loops);
    return 0;
}
