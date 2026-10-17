#include <iostream>
#include <string>

void Solve() {
    std::string s;
    std::cin >> s;

    // 博弈论分析:
    // 每次可以减去一个回文数 (所有 1..9 都是回文数).
    // 任何非 10 的倍数, 都可以通过减去它的个位数 (1..9, 回文数) 变成 10 的倍数!
    // 而从 10 的倍数出发, 任何合法减法 (回文数不能以 0 结尾, 故不能是 10 的倍数)
    // 必定使得数字变成非 10 的倍数!
    // 因此 10 的倍数是必败态 (后手 Elsie 必胜), 其余是必胜态 (先手 Bessie 必胜).
    if (s.back() == '0') {
        std::cout << "E\n";
    } else {
        std::cout << "B\n";
    }
}

int main() {
    int t;
    if (std::cin >> t) {
        while (t--) {
            Solve();
        }
    }
    return 0;
}
