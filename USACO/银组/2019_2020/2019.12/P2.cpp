#include <cstdio>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

const int MAXN = 50000;

struct Cow {
    int w;  // 体重
    int x;  // 初始位置
    int d;  // 初始方向：1 向右，-1 向左
};

bool byPosition(const Cow &a, const Cow &b) {
    return a.x < b.x;
}

int main() {
    int n, L;
    scanf("%d%d", &n, &L);

    vector<Cow> cow(n);
    for (int i = 0; i < n; ++i) {
        scanf("%d%d%d", &cow[i].w, &cow[i].x, &cow[i].d);
    }
    // 两头牛相遇时互相交换速度，所以任意时刻奶牛的左右次序都不变：
    // 排序后的第 k 头牛永远位于当前第 k 靠左的位置上。
    sort(cow.begin(), cow.end(), byPosition);

    // ================= 第一步：求出时刻 T =================
    // 把"交换速度"改看成"互相穿过"，每头牛到达谷仓的时刻集合不变：
    // 向左走的 z 头牛各自贡献时刻 x，其余 n - z 头向右走的牛各自贡献时刻 L - x。
    // 又因为次序不变，真正到达左谷仓的恰是排序后的前 z 头牛，到达右谷仓的是其余牛，
    // 并且到达时刻按从小到大依次对应过去（左侧取前 z 小，右侧取后 n - z 小）。
    vector<int> leftTime;   // 左谷仓的到达时刻，随下标递增而递增
    vector<int> rightTime;  // 右谷仓的到达时刻，随下标递增而递减
    int z = 0;              // 向左走的奶牛数量
    for (int i = 0; i < n; ++i) {
        if (cow[i].d == -1) {
            leftTime.push_back(cow[i].x);
            ++z;
        }
    }
    for (int i = 0; i < n; ++i) {
        if (cow[i].d == 1) {
            rightTime.push_back(L - cow[i].x);
        }
    }

    vector<pair<int, int> > event;  // (到达时刻, 该时刻到达的奶牛的体重)
    for (int i = 0; i < (int)leftTime.size(); ++i) {
        event.push_back(make_pair(leftTime[i], cow[i].w));
    }
    for (int i = 0; i < (int)rightTime.size(); ++i) {
        event.push_back(make_pair(rightTime[i], cow[z + i].w));
    }
    sort(event.begin(), event.end());

    long long totalWeight = 0;
    for (int i = 0; i < n; ++i) {
        totalWeight += cow[i].w;
    }

    // remaining = 总体重 - 2 * (已停止奶牛的体重和)，一旦 <= 0 就说明停下的体重达到一半。
    long long remaining = totalWeight;
    long long T = 0;
    for (int i = 0; i < (int)event.size(); ++i) {
        remaining -= 2LL * event[i].second;
        if (remaining <= 0) {
            T = event[i].first;
            break;
        }
    }

    // ================= 第二步：统计 [0, T] 内的相遇次数 =================
    // 时间 T 确定后可以忽略体重，直接把它当作"互相穿过"来数交点：
    // 位置靠左且向右的牛 i 与位置靠右且向左的牛 j 在时刻 (x_j - x_i) / 2 相遇，
    // 该时刻 <= T 等价于 x_i + 2T >= x_j。
    queue<int> goingRight;  // 队列中的位置递增，即还没被扫过的向左走的牛追上的候选牛
    long long meetings = 0;
    for (int i = 0; i < n; ++i) {
        if (cow[i].d == -1) {
            // 追不上的候选牛以后更追不上，直接从队首丢弃。
            while (!goingRight.empty() && (long long)goingRight.front() + 2LL * T < cow[i].x) {
                goingRight.pop();
            }
            meetings += (long long)goingRight.size();
        } else {
            goingRight.push(cow[i].x);
        }
    }

    printf("%lld\n", meetings);
    return 0;
}
