#include <cstdio>
#include <string>
#include <unordered_set>

using namespace std;

int main() {
    int n;
    char buf[128];
    if (scanf("%d %s", &n, buf) != 2) return 0;
    string s = buf;

    // 从长度 1 开始尝试连续子串的长度 k
    for (int k = 1; k <= n; ++k) {
        bool allUnique = true;
        unordered_set<string> seen;

        for (int i = 0; i + k <= n; ++i) {
            string sub = s.substr(i, k);
            if (seen.find(sub) != seen.end()) {
                allUnique = false;
                break;
            }
            seen.insert(sub);
        }

        if (allUnique) {
            printf("%d\n", k);
            return 0;
        }
    }

    printf("%d\n", n);
    return 0;
}
