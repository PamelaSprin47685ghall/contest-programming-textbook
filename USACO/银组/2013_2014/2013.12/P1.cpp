#include <cstdio>
#include <algorithm>
#include <queue>

using namespace std;

const int MAXN = 10005;
const int MAXD = 10000;

struct Cow {
    int g; // 产奶量
    int d; // 截止时间

    // 让 priority_queue 成为按产奶量的大根堆
    bool operator<(const Cow &other) const {
        return g < other.g;
    }
};

Cow cows[MAXN];

// 按截止时间从大到小排序，方便从后往前扫描时逐只“解锁”奶牛
bool cmpByDeadline(const Cow &a, const Cow &b) {
    return a.d > b.d;
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &cows[i].g, &cows[i].d);
    }
    sort(cows, cows + n, cmpByDeadline);

    // 从最后时刻往前安排：时刻 t 的候选集合只会越来越大
    // （奶牛一旦满足“截止时间 >= t”就永远留在候选集合里），
    // 因此每次直接取当前产奶量最大的奶牛一定不亏。
    priority_queue<Cow> pq;
    int idx = 0;
    int total = 0;
    for (int t = MAXD; t >= 1; t--) {
        while (idx < n && cows[idx].d >= t) {
            pq.push(cows[idx]);
            idx++;
        }
        if (!pq.empty()) {
            total += pq.top().g;
            pq.pop();
        }
    }

    printf("%d\n", total);
    return 0;
}
