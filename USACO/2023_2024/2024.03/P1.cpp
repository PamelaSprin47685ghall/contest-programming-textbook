#include <iostream>
#include <string>
#include <vector>

const int INF = 1000000000;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, q;
    if (!(std::cin >> n >> q)) return 0;

    std::vector<std::string> s(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> s[i];
    }

    std::vector<int> group(n);
    int groupNo = 0;
    for (int i = 0; i < n; ++i) {
        if (s[i] == "or") {
            ++groupNo;
        } else if (i % 2 == 0) {
            group[i] = groupNo;
        }
    }

    std::vector<int> firstFalse(groupNo + 1, INF);
    std::vector<int> lastFalse(groupNo + 1, -1);
    for (int i = 0; i < n; i += 2) {
        int g = group[i];
        if (s[i] == "false") {
            if (firstFalse[g] == INF) {
                firstFalse[g] = i;
            }
            lastFalse[g] = i;
        }
    }

    int totalFirstTrue = INF;
    int totalLastTrue = -1;
    for (int i = 0; i <= groupNo; ++i) {
        if (firstFalse[i] == INF) {
            if (totalFirstTrue == INF) {
                totalFirstTrue = i;
            }
            totalLastTrue = i;
        }
    }

    while (q--) {
        int l, r;
        std::string target;
        std::cin >> l >> r >> target;
        --l; --r;

        int lGroup = group[l];
        int rGroup = group[r];

        // 如果在被替换区间外侧已经存在恒为 true 的与块, 整个表达式恒为 true
        if (totalFirstTrue < lGroup || totalLastTrue > rGroup) {
            std::cout << (target == "true" ? 'Y' : 'N');
            continue;
        }

        if (target == "true") {
            // 检查 lGroup 在 l 之前是否已经含有 false, 以及 rGroup 在 r 之后是否含有 false
            std::cout << (firstFalse[lGroup] < l || lastFalse[rGroup] > r ? 'N' : 'Y');
        } else {
            // 总是可以通过把该区间替换为 false 使得包含该区间的这部分与块为 false
            std::cout << 'Y';
        }
    }
    std::cout << "\n";
    return 0;
}
