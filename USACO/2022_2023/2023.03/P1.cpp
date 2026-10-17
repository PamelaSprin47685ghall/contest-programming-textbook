#include <iostream>
#include <string>
#include <vector>

int main() {
    int n;
    std::string s;
    if (!(std::cin >> n >> s)) return 0;

    int first = 0;
    while (first < n && s[first] == 'F') ++first;

    // 全是 'F': 兴奋值可以是从 0 到 n-1 的任意整数
    if (first == n) {
        std::cout << n << "\n";
        for (int i = 0; i < n; ++i) {
            std::cout << i << "\n";
        }
        return 0;
    }

    int last = n - 1;
    while (last >= 0 && s[last] == 'F') --last;

    int minScore = 0;
    int maxScore = 0;

    int curr = first;
    while (curr < last) {
        int nxt = curr + 1;
        while (s[nxt] == 'F') ++nxt;
        int len = nxt - curr - 1;

        if (s[curr] == s[nxt]) {
            if (len % 2 == 0) {
                minScore += 1;
            }
            maxScore += len + 1;
        } else {
            if (len % 2 == 1) {
                minScore += 1;
            }
            maxScore += len;
        }
        curr = nxt;
    }

    int freeEnds = first + (n - 1 - last);
    maxScore += freeEnds;

    // 如果首尾存在任意 'F', 奇偶性可自由调节 (步长为 1); 否则每次修改改变 2, 步长为 2
    int step = (freeEnds > 0) ? 1 : 2;

    std::vector<int> results;
    for (int v = minScore; v <= maxScore; v += step) {
        results.push_back(v);
    }

    std::cout << results.size() << "\n";
    for (int v : results) {
        std::cout << v << "\n";
    }

    return 0;
}
