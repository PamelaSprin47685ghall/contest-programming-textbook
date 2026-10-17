#include <algorithm>
#include <cstdio>
#include <vector>

struct Cow {
    int position = 0;
    int speed = 0;
};

struct Event {
    int time;
    int no;
    int speed;
};

// 每头牛在某时刻改变速度的事件, 按时间排序后模拟谁领先.
struct Events {
    std::vector<Event> events;

    void Read() {
        int counts[2];
        scanf("%d%d", &counts[0], &counts[1]);
        for (int cow = 0; cow < 2; ++cow) {
            int time = 0;
            for (int i = 0; i < counts[cow]; ++i) {
                int speed, delta;
                scanf("%d%d", &speed, &delta);
                events.push_back(Event{time, cow, speed});
                time += delta;
            }
            // 结束事件: 之后速度不变, 只用来推进时间.
            events.push_back(Event{time, cow, 0});
        }
    }

    // 让牛按当前速度走 deltaTime 秒.
    void Move(Cow& cow, int deltaTime) {
        cow.position += deltaTime * cow.speed;
    }

    int Process() {
        std::sort(events.begin(), events.end(), [](const Event& e0, const Event& e1) {
            return e0.time < e1.time;
        });
        Cow cows[2];
        int sign = 0;  // 上一时刻 牛0 相对 牛1 的领先关系 (0=并列).
        int time = 0;
        int answer = 0;
        auto sgn = [](int v) { return (0 < v) - (v < 0); };
        for (const Event& e : events) {
            for (Cow& cow : cows) Move(cow, e.time - time);
            time = e.time;
            cows[e.no].speed = e.speed;
            int newSign = sgn(cows[0].position - cows[1].position);
            if (newSign == 0) continue;  // 并列不算换位.
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