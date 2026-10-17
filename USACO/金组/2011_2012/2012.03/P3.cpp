#include <cstdio>

using namespace std;

const int MAXN = 18;
const int MAXMASK = 1 << MAXN;  // 子集总数 (N <= 18)
const int INF = 1000000000;

int n, capacity;
int weight[MAXN];
int subsetSum[MAXMASK];  // subsetSum[mask]: mask 里所有奶牛的总重量
int rides[MAXMASK];      // rides[mask]: 运走 mask 里所有奶牛所需的最少趟数
int lastLoad[MAXMASK];   // lastLoad[mask]: 趟数最少时, 最后一趟已装重量的最小值

int main() {
    scanf("%d %d", &n, &capacity);
    for (int i = 0; i < n; i++) {
        scanf("%d", &weight[i]);
    }
    int full = (1 << n) - 1;

    for (int mask = 1; mask <= full; mask++) {
        int lowBit = 0;
        while (((mask >> lowBit) & 1) == 0) {
            lowBit++;
        }
        subsetSum[mask] = subsetSum[mask ^ (1 << lowBit)] + weight[lowBit];
    }

    // 一趟一趟地往下装: 每加入一头牛, 它要么挤进当前最后一趟, 要么自己另起一趟.
    // 于是未来的选择只取决于"已经排好的趟数"和"最后一趟装了多少", 这两个数取字典序最小即可.
    // rides[0] = 1 是把当前这趟空电梯也算进去, 全部装完时趟数正好是 rides[full].
    rides[0] = 1;
    lastLoad[0] = 0;
    for (int mask = 1; mask <= full; mask++) {
        rides[mask] = INF;
        lastLoad[mask] = INF;
        for (int i = 0; i < n; i++) {
            if (((mask >> i) & 1) == 0) {
                continue;  // 只考虑 mask 里的牛
            }
            int prev = mask ^ (1 << i);
            int nowRides = rides[prev];
            int load = lastLoad[prev];
            if (load + weight[i] <= capacity) {
                load += weight[i];  // 挤进最后一趟
            } else {
                nowRides++;         // 另起一趟
                load = weight[i];
            }
            if (nowRides < rides[mask] || (nowRides == rides[mask] && load < lastLoad[mask])) {
                rides[mask] = nowRides;
                lastLoad[mask] = load;
            }
        }
    }

    printf("%d\n", rides[full]);

    // 还原方案: 每一趟都取"仍能保持总趟数最少"里最满的那一组,
    // 并且强制每组含当前最小编号的牛, 保证每头牛恰好安排一次.
    int rest = full;
    while (rest != 0) {
        int lowBit = 0;
        while (((rest >> lowBit) & 1) == 0) {
            lowBit++;
        }
        int best = 0;  // subsetSum[0] = 0, 而候选组的重量至少为 1, 所以它一定会被顶掉
        for (int part = rest; part != 0; part = (part - 1) & rest) {
            if (((part >> lowBit) & 1) == 0) {
                continue;
            }
            if (subsetSum[part] > capacity) {
                continue;
            }
            // 这一趟走完还得能用最少趟数装走剩下的牛; part == rest 表示剩下的 0 头牛要用 0 趟,
            // 而 rides[0] 的 1 只是"当前这趟还是空的"的约定, 所以这种情况直接算合格.
            if (part != rest && rides[rest ^ part] != rides[rest] - 1) {
                continue;
            }
            if (subsetSum[part] > subsetSum[best]) {
                best = part;
            }
        }
        int cnt = 0;
        for (int i = 0; i < n; i++) {
            if ((best >> i) & 1) {
                cnt++;
            }
        }
        printf("%d", cnt);
        for (int i = 0; i < n; i++) {
            if ((best >> i) & 1) {
                printf(" %d", i + 1);
            }
        }
        printf("\n");
        rest ^= best;
    }
    return 0;
}
