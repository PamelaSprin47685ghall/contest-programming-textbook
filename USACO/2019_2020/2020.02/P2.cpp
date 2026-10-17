#include <iostream>
#include <string>

int main() {
    int n;
    std::string a, b;
    if (!(std::cin >> n >> a >> b)) return 0;

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

    std::cout << flipCount << "\n";
    return 0;
}
