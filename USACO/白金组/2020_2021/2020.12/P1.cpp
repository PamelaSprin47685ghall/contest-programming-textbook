#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 3000;       // 奶牛 / 牛棚数量上限
const int MOD = 1000000007;  // 方案数取模

struct Event {
    int size;  // 牛或牛棚的大小
    int kind;  // 0 表示一头牛，1 表示一个牛棚
};

Event events[2 * MAXN];
// pending[a][f]：扫到当前位置时，有 a 头"已经确定进匹配、但还没分到牛棚"的牛；
// f = 1 表示前面已经出现过一头不进匹配的牛，f = 0 表示见过的牛全都进了匹配
int pending[MAXN + 1][2];
int nextPending[MAXN + 1][2];

// 大小相同时把牛排在牛棚前面：这样"大小为 x 的牛配不上大小为 x 的空牛棚"这条规则
// 可以统一交给"扫描到牛棚时 f 是否已经为 1"来判断
bool compareEvent(const Event &x, const Event &y) {
    if (x.size != y.size) return x.size < y.size;
    return x.kind < y.kind;
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &events[i].size);
        events[i].kind = 0;
    }
    for (int i = 0; i < n; ++i) {
        scanf("%d", &events[n + i].size);
        events[n + i].kind = 1;
    }
    sort(events, events + 2 * n, compareEvent);

    pending[0][0] = 1;
    for (int pos = 0; pos < 2 * n; ++pos) {
        for (int a = 0; a <= n; ++a) {
            nextPending[a][0] = 0;
            nextPending[a][1] = 0;
        }
        if (events[pos].kind == 0) {
            // 一头牛：让它进匹配（先欠着牛棚），或者不让它进匹配
            for (int a = 0; a < n; ++a) {
                for (int f = 0; f < 2; ++f) {
                    int ways = pending[a][f];
                    if (ways == 0) continue;
                    nextPending[a + 1][f] = (nextPending[a + 1][f] + ways) % MOD;
                    // 它是目前为止最小的没进匹配的牛，之后再遇到牛棚就必须用掉
                    nextPending[a][1] = (nextPending[a][1] + ways) % MOD;
                }
            }
        } else {
            // 一个牛棚：拿去配一头正在等的牛（a 种选法），或者让它空着
            for (int a = 0; a <= n; ++a) {
                for (int f = 0; f < 2; ++f) {
                    int ways = pending[a][f];
                    if (ways == 0) continue;
                    // 只有在还没出现过没进匹配的牛时才能空置：否则那头更小的牛
                    // 就能睡进来，匹配就不是极大的了
                    if (f == 0) nextPending[a][0] = (nextPending[a][0] + ways) % MOD;
                    if (a > 0) {
                        nextPending[a - 1][f] =
                            (int)((nextPending[a - 1][f] + (long long)a * ways) % MOD);
                    }
                }
            }
        }
        for (int a = 0; a <= n; ++a) {
            pending[a][0] = nextPending[a][0];
            pending[a][1] = nextPending[a][1];
        }
    }
    // a 必须为 0：所有"欠着牛棚"的牛最终都分到了牛棚
    printf("%d\n", (pending[0][0] + pending[0][1]) % MOD);
    return 0;
}
