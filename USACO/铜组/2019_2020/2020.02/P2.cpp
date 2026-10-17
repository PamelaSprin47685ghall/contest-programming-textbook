#include <cstdio>
#include <string>

using namespace std;

int main() {
    int n;
    char bufA[1005], bufB[1005];
    if (scanf("%d %s %s", &n, bufA, bufB) != 3) return 0;
    string a = bufA, b = bufB;

    // 每次操作可以将一段连续子串的 H/G 相互翻转.
    // 找出序列中由于 B 与 A 不一致而形成的极长不匹配连续区间的数量.
    int flipCount = 0;
    bool inMismatch = false;

    for (int i = 0; i < n; ++i) {
        if (a[i] != b[i]) {
            if (!inMismatch) {
                inMismatch = true;
                ++flipCount;
            }
        } else {
            inMismatch = false;
        }
    }

    printf("%d\n", flipCount);
    return 0;
}
