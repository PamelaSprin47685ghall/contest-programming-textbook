#include <algorithm>
#include <cstdio>
#include <limits>
#include <vector>

using namespace std;

// 覆盖次数变化的事件: 在某个时间点覆盖层数增加或减少 1.
struct Event {
    int time;
    int delta;
};

struct Events {
    vector<Event> events;

    void Read() {
        int n, at = 0;
        scanf("%d", &n);
        for (int i = 0; i < n; ++i) {
            int distance;
            char direction;
            scanf("%d %c", &distance, &direction);
            if (direction == 'L') {
                // 向左走: 覆盖从 at-distance 到 at.
                events.push_back(Event{at, -1});
                events.push_back(Event{at - distance, 1});
                at -= distance;
            } else {
                events.push_back(Event{at, 1});
                events.push_back(Event{at + distance, -1});
                at += distance;
            }
        }
    }

    // 按时间排序后扫描, 累加覆盖至少 2 层的区间长度.
    int Process() {
        sort(events.begin(), events.end(), [](const Event& e0, const Event& e1) {
            return e0.time < e1.time;
        });
        int previousTime = numeric_limits<int>::min();
        int thickness = 0;
        int answer = 0;
        for (const Event& e : events) {
            if (2 <= thickness) answer += e.time - previousTime;
            previousTime = e.time;
            thickness += e.delta;
        }
        return answer;
    }
};

int main() {
    Events events;
    events.Read();
    printf("%d\n", events.Process());
    return 0;
}
