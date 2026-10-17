#include <cstdio>
#include <vector>

using namespace std;

// 标记所有可能的最终牛奶总量 (初值为 1000, 范围在合理区间内).
bool possibleTotal[2001];

// 递归模拟 4 次搬运:
// day 1: 谷仓 1 运到 谷仓 2
// day 2: 谷仓 2 运到 谷仓 1
// day 3: 谷仓 1 运到 谷仓 2
// day 4: 谷仓 2 运到 谷仓 1
void Simulate(int day, int milk1, vector<int> b1, vector<int> b2) {
    if (day == 5) {
        if (0 <= milk1 && milk1 <= 2000) {
            possibleTotal[milk1] = true;
        }
        return;
    }

    if (day % 2 == 1) {
        // 从谷仓 1 搬一只桶到谷仓 2
        for (size_t i = 0; i < b1.size(); ++i) {
            int val = b1[i];
            vector<int> nextB1 = b1;
            vector<int> nextB2 = b2;
            nextB1.erase(nextB1.begin() + i);
            nextB2.push_back(val);
            Simulate(day + 1, milk1 - val, nextB1, nextB2);
        }
    } else {
        // 从谷仓 2 搬一只桶到谷仓 1
        for (size_t i = 0; i < b2.size(); ++i) {
            int val = b2[i];
            vector<int> nextB1 = b1;
            vector<int> nextB2 = b2;
            nextB2.erase(nextB2.begin() + i);
            nextB1.push_back(val);
            Simulate(day + 1, milk1 + val, nextB1, nextB2);
        }
    }
}

int main() {
    vector<int> barn1(10);
    vector<int> barn2(10);
    for (int i = 0; i < 10; ++i) scanf("%d", &barn1[i]);
    for (int i = 0; i < 10; ++i) scanf("%d", &barn2[i]);

    Simulate(1, 1000, barn1, barn2);

    int answer = 0;
    for (int i = 0; i <= 2000; ++i) {
        if (possibleTotal[i]) {
            ++answer;
        }
    }
    printf("%d\n", answer);
    return 0;
}
