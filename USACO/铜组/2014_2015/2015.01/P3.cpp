#include <cstdio>
#include <string>
#include <unordered_map>

using namespace std;

const int MAX_BASE = 15000;

char buffer[80];

// 把字符串按各种进制解释成数值，记录"数值 -> 进制"。
unordered_map<int, int> values(const string& s) {
    unordered_map<int, int> answer;
    for (int base = 10; base <= MAX_BASE; ++base) {
        int value = 0;
        for (char c : s) value = value * base + (c - '0');
        answer[value] = base;
    }
    return answer;
}

void solve() {
    scanf("%s", buffer);
    string x = buffer;
    scanf("%s", buffer);
    string y = buffer;
    unordered_map<int, int> valueX = values(x), valueY = values(y);
    for (const auto& entry : valueX) {
        auto it = valueY.find(entry.first);
        if (it == valueY.end()) continue;
        printf("%d %d\n", entry.second, it->second);
        return;
    }
}

int main() {
    int t;
    scanf("%d", &t);
    for (int i = 0; i < t; ++i) solve();
    return 0;
}
