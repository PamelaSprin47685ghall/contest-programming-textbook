#include <algorithm>
#include <cstdio>
#include <vector>

struct Cow {
    int position = 0;
    char direction;
};

struct Event {
    int time;
    int no;
    char direction;
};

// 每头牛在某时刻改变方向的事件, 按时间排序后模拟相遇.
struct Events {
    std::vector<Event> events;

    void Read() {
        int counts[2];
        scanf("%d%d", &counts[0], &counts[1]);
        for (int cow = 0; cow < 2; ++cow) {
            int time = 0;
            for (int i = 0; i < counts[cow]; ++i) {
                int delta;
                char direction;
                scanf("%d %c", &delta, &direction);
                events.push_back(Event{time, cow, direction});
                time += delta;
            }
            // 结束事件: 之后不再移动.
            events.push_back(Event{time, cow, ' '});
        }
    }

    // 让牛按当前方向走 deltaTime 秒.
    void Move(Cow& cow, int deltaTime) {
        if (cow.direction == 'L') cow.position -= deltaTime;
        else if (cow.direction == 'R') cow.position += deltaTime;
    }

    int Process() {
        std::sort(events.begin(), events.end(), [](const Event& e0, const Event& e1) {
            return e0.time < e1.time;
        });
        Cow cows[2];
        int sign = 0;  // 上一时刻 牛0 相对 牛1 的左右关系 (0=重合).
        int time = 0;
        int answer = 0;
        auto sgn = [](int v) { return (0 < v) - (v < 0); };
        for (const Event& e : events) {
            for (Cow& cow : cows) Move(cow, e.time - time);
            time = e.time;
            cows[e.no].direction = e.direction;
            int newSign = sgn(cows[0].position - cows[1].position);
            // 只有真正擦肩而过 (符号翻转) 才计一次相遇.
            if (sign != 0 && newSign != sign) ++answer;
            sign = newSign;
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