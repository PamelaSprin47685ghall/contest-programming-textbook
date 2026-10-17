#include <cstdio>
#include <algorithm>
#include <deque>

using namespace std;

const int MAXN = 200005;
const long long TOWER_TOP_INF = 1LL << 60;   // 空塔可以装作塔顶重量无穷大

struct CowGroup {
    long long weight;   // 这一组奶牛的体重
    long long amount;   // 这一组奶牛的数量
};

int n;
long long maxTowers, minGap;
CowGroup groups[MAXN];
deque<CowGroup> towers;   // 队首是塔顶奶牛最重的那些塔

// 体重大的先处理：这样新放上去的奶牛一定比之前所有奶牛都轻
bool cmpByWeightDesc(const CowGroup &x, const CowGroup &y) {
    return x.weight > y.weight;
}

int main() {
    scanf("%d%lld%lld", &n, &maxTowers, &minGap);
    for (int i = 0; i < n; ++i) {
        scanf("%lld%lld", &groups[i].weight, &groups[i].amount);
    }
    sort(groups, groups + n, cmpByWeightDesc);

    towers.push_back(CowGroup{TOWER_TOP_INF, maxTowers});
    long long answer = 0;
    for (int i = 0; i < n; ++i) {
        long long weight = groups[i].weight;
        long long remaining = groups[i].amount;
        // 一头体重为 weight 的奶牛只站得上塔顶比它重 minGap 的塔；
        // 队首塔顶最重，队首都放不下就说明没有塔放得下，剩下的奶牛只能作罢
        while (!towers.empty() && remaining > 0 &&
               towers.front().weight >= weight + minGap) {
            if (towers.front().amount > remaining) {
                towers.front().amount -= remaining;
                remaining = 0;
            } else {
                remaining -= towers.front().amount;
                towers.pop_front();
            }
        }
        long long placed = groups[i].amount - remaining;
        if (placed > 0) {
            // 这些塔的新塔顶是当前体重，它比之前出现过的体重都轻，压到队尾仍保持降序
            towers.push_back(CowGroup{weight, placed});
            answer += placed;
        }
    }
    printf("%lld\n", answer);
    return 0;
}
