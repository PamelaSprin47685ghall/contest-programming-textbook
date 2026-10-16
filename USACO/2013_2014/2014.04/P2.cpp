#include <algorithm>
#include <cstdio>
#include <unordered_map>
#include <vector>

struct Cow {
    int x;
    char breed; // 'G' 或 'H'
};

int main() {
    int n;
    scanf("%d", &n);
    std::vector<Cow> cows(n);
    for (Cow& cow : cows) scanf("%d %c", &cow.x, &cow.breed);
    std::sort(cows.begin(), cows.end(),
              [](const Cow& a, const Cow& b) { return a.x < b.x; });

    // 前缀和: 把 G 记为 +1、H 记为 -1. 前缀和相同的两个位置之间,
    // 两种牛的数量相等, 是"公平"的候选区间.
    std::vector<int> prefix(cows.size() + 1);
    for (int i = 1; i < (int)prefix.size(); ++i) {
        prefix[i] = prefix[i - 1] + (cows[i - 1].breed == 'G' ? 1 : -1);
    }

    int answer = 0;
    // 情况一: 区间内两种牛数量相等. 相同前缀和的最左位置到当前位置
    // 构成一个公平区间, 取最远的.
    std::unordered_map<int, int> firstSeen;
    for (int i = 0; i < (int)prefix.size(); ++i) {
        std::unordered_map<int, int>::iterator it = firstSeen.find(prefix[i]);
        if (it != firstSeen.end()) {
            int distance = cows[i - 1].x - cows[it->second].x;
            answer = std::max(answer, distance);
        } else {
            firstSeen[prefix[i]] = i;
        }
    }

    // 情况二: 区间内全是同一种牛. 按连续相同牛种分块, 块内任意
    // 子区间都公平, 取整块跨度最大者.
    int begin = 0;
    for (int i = 1; i <= n; ++i) {
        if (i != n && cows[i].breed == cows[i - 1].breed) continue;
        int distance = cows[i - 1].x - cows[begin].x;
        answer = std::max(answer, distance);
        begin = i;
    }

    printf("%d\n", answer);
    return 0;
}