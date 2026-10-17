#include <algorithm>
#include <cstdio>
#include <functional>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

const int MAXN = 100000;

struct Cow {
    int arrival;    // 到达草地的时刻
    int duration;   // 吃草所需时长
    int seniority;  // 资历编号, 越小越优先
};

bool ByArrival(const Cow& x, const Cow& y) {
    return x.arrival < y.arrival;
}

int n;
Cow cows[MAXN];

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        cows[i].seniority = i;  // 输入顺序即资历顺序, 越靠前越优先
        scanf("%d%d", &cows[i].arrival, &cows[i].duration);
    }
    sort(cows, cows + n, ByArrival);

    // 等待中的奶牛只需按期找出资历最靠前的一头, 所以用最小堆(键为资历编号).
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > waiting;
    // 草地重新空闲的时刻; 首头到达的奶牛立刻开吃, 等待时间为 0.
    long long freeTime = (long long)cows[0].arrival + cows[0].duration;
    long long answer = 0;
    int next = 1;  // 下一头尚未到达的奶牛
    while (next < n || !waiting.empty()) {
        // 恰好在别人吃完那一刻到达也算等待, 所以用 <= 判断
        while (next < n && cows[next].arrival <= freeTime) {
            waiting.push(make_pair(cows[next].seniority, next));
            ++next;
        }
        if (waiting.empty()) {
            // 队伍空且下一位还没到: 草地空闲一段时间, 下一位到达者直接开吃
            freeTime = (long long)cows[next].arrival + cows[next].duration;
            ++next;
        } else {
            int who = waiting.top().second;
            waiting.pop();
            answer = max(answer, freeTime - cows[who].arrival);
            freeTime += cows[who].duration;
        }
    }
    printf("%lld\n", answer);
    return 0;
}
