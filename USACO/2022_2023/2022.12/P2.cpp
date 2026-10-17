#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

void Solve() {
    int n, k;
    std::string s;
    std::cin >> n >> k >> s;

    std::string ans(n, '.');
    int patches = 0;

    // 分别贪心安置 G 和 H 草地
    int lastG = -k - 1;
    int lastH = -k - 1;

    for (int i = 0; i < n; ++i) {
        if (s[i] == 'G') {
            if (i > lastG + k) {
                // 需要开辟一块新的 G 草地, 贪心放在 min(n - 1, i + k) 处
                int pos = std::min(n - 1, i + k);
                // 如果该格已被占用, 往前找空位
                while (ans[pos] != '.') {
                    --pos;
                }
                ans[pos] = 'G';
                lastG = pos;
                ++patches;
            }
        } else {
            if (i > lastH + k) {
                // 需要开辟一块新的 H 草地, 贪心放在 min(n - 1, i + k) 处
                int pos = std::min(n - 1, i + k);
                while (ans[pos] != '.') {
                    --pos;
                }
                ans[pos] = 'H';
                lastH = pos;
                ++patches;
            }
        }
    }

    std::cout << patches << "\n" << ans << "\n";
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
