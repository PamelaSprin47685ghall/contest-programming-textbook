#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

// 归并两条减速事件链: 按时间发生的减速, 按距离发生的减速.
// 当前速度是 1/invSpeed 英里/分钟, 每次减速后速度变慢.
struct Race {
    vector<double> timeEvents;
    vector<double> distanceEvents;

    void Read() {
        int n;
        scanf("%d", &n);
        for (int i = 0; i < n; ++i) {
            char kind;
            double value;
            scanf(" %c%lf", &kind, &value);
            if (kind == 'T') timeEvents.push_back(value);
            else distanceEvents.push_back(value);
        }
        sort(timeEvents.begin(), timeEvents.end());
        sort(distanceEvents.begin(), distanceEvents.end());
    }

    double Solve() {
        int invSpeed = 1; // 速度的倒数, 即每英里所需分钟数
        int timeIndex = 0, distanceIndex = 0;
        double distance = 0.0, time = 0.0;
        int timeCount = (int)timeEvents.size();
        int distanceCount = (int)distanceEvents.size();
        while (timeIndex < timeCount || distanceIndex < distanceCount) {
            double nextTimeEvent = 1.0 / 0.0;   // 正无穷, 表示没有该事件
            double nextDistanceEvent = 1.0 / 0.0;
            if (timeIndex < timeCount) {
                nextTimeEvent = timeEvents[timeIndex];
            }
            if (distanceIndex < distanceCount) {
                // 按当前速度, 到达该距离点的时间
                nextDistanceEvent =
                    (distanceEvents[distanceIndex] - distance) * invSpeed +
                    time;
            }
            if (nextTimeEvent < nextDistanceEvent) {
                distance += (nextTimeEvent - time) / invSpeed;
                time = nextTimeEvent;
                ++timeIndex;
            } else {
                distance = distanceEvents[distanceIndex];
                time = nextDistanceEvent;
                ++distanceIndex;
            }
            ++invSpeed;
        }
        return (1000.0 - distance) * invSpeed + time;
    }
};

int main() {
    Race race;
    race.Read();
    printf("%.0lf\n", race.Solve());
    return 0;
}