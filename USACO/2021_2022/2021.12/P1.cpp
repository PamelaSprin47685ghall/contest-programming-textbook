#include <iostream>
#include <string>
#include <vector>

int main() {
    int n;
    std::string s;
    if (!(std::cin >> n >> s)) return 0;

    // leftCount[i]: 在位置 i 左侧, 紧邻的与 s[i] 品种不同的连续牛数量
    // rightCount[i]: 在位置 i 右侧, 紧邻的与 s[i] 品种不同的连续牛数量
    std::vector<long long> leftCount(n, 0);
    std::vector<long long> rightCount(n, 0);

    long long count = 0;
    for (int i = 0; i < n; ++i) {
        if (i > 0 && s[i] == s[i - 1]) {
            count = 0;
        } else if (i > 0) {
            count = 1;
            while (i - 1 - count >= 0 && s[i - 1 - count] == s[i - 1]) {
                ++count;
            }
        }
        leftCount[i] = count;
    }

    count = 0;
    for (int i = n - 1; i >= 0; --i) {
        if (i < n - 1 && s[i] == s[i + 1]) {
            count = 0;
        } else if (i < n - 1) {
            count = 1;
            while (i + 1 + count < n && s[i + 1 + count] == s[i + 1]) {
                ++count;
            }
        }
        rightCount[i] = count;
    }

    long long answer = 0;
    for (int i = 0; i < n; ++i) {
        long long l = leftCount[i];
        long long r = rightCount[i];

        // 1. 作为中间唯一的孤独牛: 左侧选 >= 1 只, 右侧选 >= 1 只
        answer += l * r;

        // 2. 作为区间最左侧的孤独牛: 右侧选 >= 2 只不同品种牛
        if (r >= 2) {
            answer += (r - 1);
        }

        // 3. 作为区间最右侧的孤独牛: 左侧选 >= 2 只不同品种牛
        if (l >= 2) {
            answer += (l - 1);
        }
    }

    std::cout << answer << "\n";
    return 0;
}
