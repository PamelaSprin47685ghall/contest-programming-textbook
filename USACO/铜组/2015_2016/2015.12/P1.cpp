#include <algorithm>
#include <cstdio>
#include <vector>

// 每段栅栏的端点是一个事件：左端点进入覆盖，右端点离开覆盖。
// 按坐标排序后扫描，当前覆盖层数大于 0 的区间长度累加起来，
// 就是被至少一个画家涂过的总长度。
struct Event {
    int X;     // 端点坐标
    int Delta; // +1 进入覆盖，-1 离开覆盖
};

bool operator<(const Event& e0, const Event& e1) {
    return e0.X < e1.X; // 按端点坐标排序
}

int main() {
    int a, b, c, d;
    scanf("%d%d%d%d", &a, &b, &c, &d);

    std::vector<Event> events;
    events.push_back(Event{a, 1});
    events.push_back(Event{b, -1});
    events.push_back(Event{c, 1});
    events.push_back(Event{d, -1});
    std::sort(events.begin(), events.end());

    int lastX = -1; // 上一个端点的坐标
    int layers = 0; // 当前被覆盖的层数
    int answer = 0;
    for (const Event& e : events) {
        if (layers > 0) {
            answer += e.X - lastX; // 上一段区间有覆盖，累加其长度
        }
        lastX = e.X;
        layers += e.Delta;
    }
    printf("%d\n", answer);
    return 0;
}