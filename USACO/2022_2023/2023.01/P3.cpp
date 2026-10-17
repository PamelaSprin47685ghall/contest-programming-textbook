#include <algorithm>
#include <iostream>
#include <string>

void Solve() {
    std::string s;
    std::cin >> s;
    int len = s.size();

    if (len < 3) {
        std::cout << -1 << "\n";
        return;
    }

    // 目标是在字符串中保留一个长为 3 的子串, 并通过修改字符把它变成 "MOO".
    // 其余两端的字符只能通过单字符删除逐个删掉.
    // 将原串删到 3 个字符需要 (len - 3) 次删除.
    // 对于任何长为 3 的子串:
    // "MOO": 0 次修改, 代价 = (len - 3)
    // "MOM": 改末尾 M 为 O (1 次修改), 代价 = (len - 3) + 1
    // "OOO": 改首位 O 为 M (1 次修改), 代价 = (len - 3) + 1
    // "OOM": 改首位和末位 (2 次修改), 代价 = (len - 3) + 2
    // 其余以 O 作为两端, 中间为 M 的 (形如 *M*) 永远无法变成 MOO (中间不能被修改)
    int minExtra = 100;
    for (int i = 0; i + 3 <= len; ++i) {
        std::string sub = s.substr(i, 3);
        if (sub[1] != 'O') {
            continue;  // 中间字母必须是 'O', 因为操作只能替换首尾字符
        }

        int extra = 0;
        if (sub[0] != 'M') ++extra;
        if (sub[2] != 'O') ++extra;

        if (extra < minExtra) {
            minExtra = extra;
        }
    }

    if (minExtra == 100) {
        std::cout << -1 << "\n";
    } else {
        std::cout << (len - 3) + minExtra << "\n";
    }
}

int main() {
    int q;
    if (std::cin >> q) {
        while (q--) {
            Solve();
        }
    }
    return 0;
}
