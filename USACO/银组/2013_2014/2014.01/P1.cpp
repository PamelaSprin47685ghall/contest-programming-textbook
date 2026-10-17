#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 10005;
const long double EPS = 1e-9L;

int timeEvents[MAXN];      // "T x" 事件, 按时刻升序排列
int distanceEvents[MAXN];  // "D x" 事件, 按距离升序排列
int timeCount = 0, distanceCount = 0;

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        char type;
        int x;
        scanf(" %c%d", &type, &x);
        if (type == 'T') timeEvents[timeCount++] = x;
        else distanceEvents[distanceCount++] = x;
    }

    // 同类事件之间的先后顺序是固定的("T 8" 一定早于 "T 12", "D 10" 一定早于 "D 15"),
    // 所以把两类事件各自排好序, 之后每一轮只要比较两个队首就能知道下一个事件是谁.
    sort(timeEvents, timeEvents + timeCount);
    // 把终点也当成一个"距离事件", 跑到它就算跑完全程, 省得单独讨论收尾.
    distanceEvents[distanceCount++] = 1000;
    sort(distanceEvents, distanceEvents + distanceCount);

    long double now = 0.0L;  // 当前时刻
    long double pos = 0.0L;  // 当前已经滑过的距离
    int slowed = 0;          // 已经减速的次数, 所以当前速度是 1/(slowed+1) 米每秒
    int ti = 0, di = 0;      // 两个事件队列的队首下标

    while (pos < 1000.0L - EPS) {
        // 以当前速度跑到下一个距离事件需要等到哪一刻
        long double reachTime = now + (distanceEvents[di] - pos) * (slowed + 1);
        if (ti < timeCount && timeEvents[ti] < reachTime) {
            // 时刻事件先发生: 先按当前速度前进到那一刻, 再减速
            long double target = timeEvents[ti++];
            pos += (target - now) / (slowed + 1);
            now = target;
        } else {
            // 距离事件先发生(两者同时发生时也先按这个分支处理: 下一轮时刻事件
            // 的等待时间是 0, 不会漏掉, 也不会重复前进)
            now = reachTime;
            pos = distanceEvents[di++];
        }
        ++slowed;
    }

    // 四舍五入到最近的整数秒(0.5 向上), 加一点容差抵消浮点累积误差
    printf("%d\n", (int)(now + 0.500000001L));
    return 0;
}
