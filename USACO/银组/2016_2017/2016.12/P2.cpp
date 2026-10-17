#include <cstdio>
#include <map>

using namespace std;

// 键: 把城市名前两个字母和州代码各当成 0..675 的编号, 拼成同一个整数.
map<int, long long> pairCount;

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        char city[16], state[4];
        scanf("%s%s", city, state);
        int cityPrefix = (city[0] - 'A') * 26 + (city[1] - 'A');
        int stateCode = (state[0] - 'A') * 26 + (state[1] - 'A');
        // 前两个字母与所在州相同的城市配不成特殊对: 能跟它配对的另一座城市
        // 也必须同名前缀, 同州, 违反"两座城市来自不同州"的条件.
        if (cityPrefix == stateCode) continue;
        ++pairCount[cityPrefix * 676 + stateCode];
    }

    long long answer = 0;
    for (map<int, long long>::iterator it = pairCount.begin(); it != pairCount.end(); ++it) {
        int key = it->first;
        int swapKey = (key % 676) * 676 + key / 676;  // 前后两段互换, 就是配对城市应有的键
        // 每一对城市会被两个键各数一次, 只统计 key < swapKey 的方向, 避免重复计数.
        if (key >= swapKey) continue;
        map<int, long long>::iterator match = pairCount.find(swapKey);
        if (match != pairCount.end()) {
            answer += it->second * match->second;
        }
    }
    printf("%lld\n", answer);
    return 0;
}
