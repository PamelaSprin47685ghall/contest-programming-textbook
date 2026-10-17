#include <cstdio>
#include <string>

using namespace std;

int main() {
    int n, k;
    if (scanf("%d %d", &n, &k) != 2) return 0;

    int currentLineChars = 0;
    for (int i = 0; i < n; ++i) {
        char buf[128];
        scanf("%s", buf);
        string word = buf;
        int len = word.size();

        if (currentLineChars + len <= k) {
            // 可以继续放在当前行
            if (currentLineChars > 0) {
                printf(" ");
            }
            printf("%s", word.c_str());
            currentLineChars += len;
        } else {
            // 换行放置
            printf("\n%s", word.c_str());
            currentLineChars = len;
        }
    }
    printf("\n");
    return 0;
}
