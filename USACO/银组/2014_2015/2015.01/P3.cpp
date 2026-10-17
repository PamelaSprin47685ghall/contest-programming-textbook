#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 100;
// 路只能从低编号田走向高编号田, 任何一条路径最多经过 99 条路,
// 每条路两人最多各花 100 时间, 所以 9900 之后的时刻不可能到达.
const int MAXTIME = 100 * MAXN + 5;

bool bessieReach[MAXN + 1][MAXTIME];
bool elsieReach[MAXN + 1][MAXTIME];

// 计算"恰好花 t 时间"从 1 号田出发能到达哪些田 (reach[v][t]).
void ComputeReach(const vector<vector<int> >& cost, int n, bool reach[MAXN + 1][MAXTIME]) {
    reach[1][0] = true;
    // 所有路都是 a->b 且 a<b, 所以按编号从小到大推一遍就够:
    // 轮到 v 时, 所有通向 v 的状态都已经算好了.
    for (int v = 1; v <= n; v++) {
        for (int w = 1; w <= n; w++) {
            int c = cost[v][w];
            if (c <= 0) continue;  // 没有这条路
            for (int t = 0; t + c < MAXTIME; t++) {
                if (reach[v][t]) reach[w][t + c] = true;
            }
        }
    }
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    vector<vector<int> > bessieCost(n + 1, vector<int>(n + 1, 0));
    vector<vector<int> > elsieCost(n + 1, vector<int>(n + 1, 0));
    for (int i = 0; i < m; i++) {
        int a, b, c, d;
        scanf("%d %d %d %d", &a, &b, &c, &d);
        bessieCost[a][b] = c;
        elsieCost[a][b] = d;
    }

    ComputeReach(bessieCost, n, bessieReach);
    ComputeReach(elsieCost, n, elsieReach);

    // 两人同时出发, 中途都不停留, 所以第一次"两人都能恰好此刻到达"的时刻就是答案.
    for (int t = 0; t < MAXTIME; t++) {
        if (bessieReach[n][t] && elsieReach[n][t]) {
            printf("%d\n", t);
            return 0;
        }
    }
    printf("IMPOSSIBLE\n");
    return 0;
}
