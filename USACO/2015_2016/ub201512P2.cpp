#include <algorithm>
#include <cstdio>
#include <vector>

// 把每段限速和每段实际速度的起点都记成一个事件，按坐标排序后扫描：
// 每到达一个边界先结算上一段（用当前的限速与实际速度），
// 再切换到新一段的数值。
struct Event {
    int X;     // 该段的起点坐标
    int Type;  // 0 限速段，1 实际速度段，2 终点
    int Value; // 限速或实际速度
};

bool operator<(const Event& e0, const Event& e1) {
    return e0.X < e1.X;
}

int main() {
    int n, m;
    scanf("%d%d", &n, &m);

    std::vector<Event> events;
    int limitPos = 0, speedPos = 0;
    for (int i = 0; i < n; ++i) {
        int length, limit;
        scanf("%d%d", &length, &limit);
        events.push_back(Event{limitPos, 0, limit});
        limitPos += length;
    }
    for (int i = 0; i < m; ++i) {
        int length, speed;
        scanf("%d%d", &length, &speed);
        events.push_back(Event{speedPos, 1, speed});
        speedPos += length;
    }
    events.push_back(Event{100, 2, 0});
    std::sort(events.begin(), events.end());

    int currentLimit = 0, currentSpeed = 0;
    int lastX = 0, answer = 0;
    for (const Event& e : events) {
        if (lastX < e.X) {
            lastX = e.X;
            // 上一段内超出的速度，取所有段的最大值
            answer = std::max(answer, currentSpeed - currentLimit);
        }
        if (e.Type == 0) {
            currentLimit = e.Value;
        } else if (e.Type == 1) {
            currentSpeed = e.Value;
        }
        // Type 2 是终点，无需切换任何数值
    }
    printf("%d\n", answer);
    return 0;
}