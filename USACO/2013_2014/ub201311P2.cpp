#include <algorithm>
#include <cstdio>
#include <vector>

struct Event {
    int time;
    int milkDelta;
};

// 温度从低到高扫描: 每头牛在 [a, b] 内产奶 y, 其余温度产奶 x.
// 在 a 处奶量增加 y-x, 在 b+1 处减少 y-x, 前缀和就是当前温度的产奶量.
int Solve() {
    int n, x, y, z;
    scanf("%d%d%d%d", &n, &x, &y, &z);

    std::vector<Event> events;
    for (int i = 0; i < n; ++i) {
        int a, b;
        scanf("%d%d", &a, &b);
        events.push_back(Event{a, y - x});     // 进入 [a, b] 区间: 由 x 变 y
        events.push_back(Event{b + 1, z - y}); // 离开区间: 由 y 变 z
    }

    std::sort(events.begin(), events.end(), [](const Event& e0, const Event& e1) {
        if (e0.time != e1.time) return e0.time < e1.time;
        return e0.milkDelta < e1.milkDelta;
    });

    int current = x * n; // 所有牛都按低温 x 产奶
    int answer = 0;      // 与旧实现一致: 从 0 开始取最大
    for (const Event& e : events) {
        current += e.milkDelta;
        answer = std::max(answer, current);
    }
    return answer;
}

int main() {
    printf("%d\n", Solve());
    return 0;
}
