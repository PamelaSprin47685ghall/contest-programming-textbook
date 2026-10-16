#include <cstdio>
#include <unordered_map>

// 灯的状态压缩成整数: 第 i 位表示第 i 盏灯 (0 基) 的开关.
struct States {
    int n;

    void Read(int& state) {
        for (int i = 0; i < n; ++i) {
            int v;
            scanf("%d", &v);
            state = state + state + v;
        }
    }

    // 一次闪烁: 每盏灯取反当且仅当左侧灯亮; 第 0 盏看最后一盏.
    void Next(int& state) {
        int left = state >> 1;
        int wrap = ((state & 1) << (n - 1));
        state ^= left + wrap;
    }

    void Write(int state) {
        for (int i = n - 1; i >= 0; --i) {
            printf("%d\n", (state >> i) & 1);
        }
    }
};

int main() {
    int n;
    long long b;
    scanf("%d%lld", &n, &b);
    States states;
    states.n = n;
    int state = 0;
    states.Read(state);

    std::unordered_map<int, int> firstSeen;  // 每个状态第一次出现的步数.
    bool loopFound = false;
    for (long long step = 0; step < b; ++step) {
        if (!loopFound) {
            auto result = firstSeen.emplace(state, step);
            if (!result.second) {
                // 发现循环: 跳过循环的整数倍, 只走剩余部分.
                int base = result.first->second;
                b = step + (b - step) % (step - base);
                loopFound = true;
            }
        }
        states.Next(state);
    }
    states.Write(state);
    return 0;
}