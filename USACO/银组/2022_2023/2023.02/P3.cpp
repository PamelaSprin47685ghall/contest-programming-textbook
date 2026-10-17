#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 200005;
const long long INF = 1000000000000000000LL;   // 1e18，比任何可能的到达时刻都大

struct Flight {
    long long depart;    // 起飞时刻（已经扣掉出发机场的转机等待时间）
    int dest;            // 到达机场
    long long arrive;    // 到达时刻
};

int n, m;
vector<Flight> flights[MAXN];   // 按出发机场分组的航班
long long layover[MAXN];        // 每个机场的转机等待时间
long long earliest[MAXN];       // 目前已知的到达各机场的最早时刻
int nextFlight[MAXN];           // 每个机场：下一个还没检查过的航班在列表中的位置
Flight pending[MAXN];           // 已经确定能赶上的航班队列
int head, tail;

// 起飞时刻从大到小
bool byDepartDesc(const Flight &a, const Flight &b) {
    return a.depart > b.depart;
}

// 已知最早能在时刻 t 到达机场 i，把该机场所有还没检查过、此刻能赶上的航班放进队列。
// 机场的航班按起飞时刻降序存放，所以到达时刻变小后从 nextFlight[i] 继续往后扫就行，
// 每条航班最多进队一次。
void enqueueCatchable(int i, long long t) {
    int size = (int)flights[i].size();
    while (nextFlight[i] < size && flights[i][nextFlight[i]].depart >= t) {
        pending[tail++] = flights[i][nextFlight[i]];
        ++nextFlight[i];
    }
}

int main() {
    scanf("%d %d", &n, &m);
    for (int j = 0; j < m; ++j) {
        int c, d;
        long long r, s;
        scanf("%d %lld %d %lld", &c, &r, &d, &s);
        Flight f;
        f.depart = r;
        f.dest = d;
        f.arrive = s;
        flights[c].push_back(f);
    }
    for (int i = 1; i <= n; ++i) scanf("%lld", &layover[i]);
    layover[1] = 0;    // 起点机场不需要等待，任何时刻从机场 1 出发的航班都能坐

    for (int i = 1; i <= n; ++i) {
        // 把"在机场 i 转机要等 a_i"折算进起飞时刻：
        // 赶得上 r 时刻的航班 ⟺ 能在 r - a_i 时刻之前到达机场 i
        for (size_t k = 0; k < flights[i].size(); ++k) {
            flights[i][k].depart -= layover[i];
        }
        sort(flights[i].begin(), flights[i].end(), byDepartDesc);
        earliest[i] = INF;
        nextFlight[i] = 0;
    }
    earliest[1] = 0;

    head = 0;
    tail = 0;
    enqueueCatchable(1, 0);
    while (head < tail) {
        Flight f = pending[head++];
        if (f.arrive >= earliest[f.dest]) continue;   // 没有更早，不必再往外扩展
        earliest[f.dest] = f.arrive;
        enqueueCatchable(f.dest, f.arrive);
    }

    for (int i = 1; i <= n; ++i) {
        if (earliest[i] == INF) printf("-1\n");
        else printf("%lld\n", earliest[i]);
    }
    return 0;
}
