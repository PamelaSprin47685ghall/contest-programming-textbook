#include <iostream>
#include <map>
#include <string>
#include <vector>

int main() {
    int k, n;
    if (!(std::cin >> k >> n)) return 0;

    std::vector<std::string> names(n);
    std::map<std::string, int> nameToId;
    for (int i = 0; i < n; ++i) {
        std::cin >> names[i];
        nameToId[names[i]] = i;
    }

    // answer[i][j]: '?' 未知, '0' i 比 j 资历低 (更初级), '1' i 比 j 资历高, 'B' 同一人
    std::vector<std::string> answer(n, std::string(n, '?'));
    for (int i = 0; i < n; ++i) answer[i][i] = 'B';

    for (int p = 0; p < k; ++p) {
        std::vector<std::string> authors(n);
        for (int i = 0; i < n; ++i) {
            std::cin >> authors[i];
        }

        // 寻找付出努力不同的分界点:
        // 如果 authors[x] > authors[x+1] (字典序逆序), 说明贡献必定不同:
        // x 之前的每一位作者的贡献都严格大于 x+1 之后的每一位作者!
        // 从而前段中的所有人资历必定严格低于后段中的所有人 (资历深的人贡献少或等于).
        for (int i = 0; i < n; ++i) {
            bool isAlphabetical = true;
            for (int j = i + 1; j < n; ++j) {
                if (authors[j - 1] > authors[j]) {
                    isAlphabetical = false;
                }
                if (!isAlphabetical) {
                    // authors[i] 的贡献严格大于 authors[j], 故 authors[i] 资历严格低于 authors[j]
                    int u = nameToId[authors[i]];
                    int v = nameToId[authors[j]];
                    answer[u][v] = '0';
                    answer[v][u] = '1';
                }
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        std::cout << answer[i] << "\n";
    }
    return 0;
}
