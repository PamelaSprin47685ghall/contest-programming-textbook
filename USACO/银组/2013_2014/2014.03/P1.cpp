#include <cstdio>

using namespace std;

const int MAXN = 2000;
// 最长的合法管道长度为 2*1000^2 = 2*10^6, 用它之外的大数表示"无边"即可.
const int INF = 2000000000;

int N, C;
int x[MAXN], y[MAXN];     // 每个田地的坐标
int best[MAXN];           // 已建成部分到每个未接入点的最短合法边
bool used[MAXN];          // 该点是否已接入生成树

// 两点之间的管道造价(平方欧氏距离)
int Cost(int i, int j) {
    int dx = x[i] - x[j];
    int dy = y[i] - y[j];
    return dx * dx + dy * dy;
}

int main() {
    scanf("%d %d", &N, &C);
    for (int i = 0; i < N; i++) {
        scanf("%d %d", &x[i], &y[i]);
    }

    for (int i = 0; i < N; i++) {
        best[i] = INF;
    }

    long long total = 0;      // 总造价可能达到 2000*2*10^6, 超过 int
    int cur = 0;              // 从 0 号田地开始扩展(起点任取不影响结果)

    // Prim: 逐轮把一个新点接入生成树, 共接入 N-1 个点
    for (int round = 0; round < N - 1; round++) {
        used[cur] = true;

        // 用新接入的点松弛其余各点的候选边
        for (int j = 0; j < N; j++) {
            if (used[j]) {
                continue;
            }
            int d = Cost(cur, j);
            // 造价小于 C 的管道承包商拒绝安装, 直接当作不存在
            if (d >= C && d < best[j]) {
                best[j] = d;
            }
        }

        // 在候选边中选造价最小的一条
        int nxt = -1;
        for (int j = 0; j < N; j++) {
            if (!used[j] && best[j] != INF) {
                if (nxt == -1 || best[j] < best[nxt]) {
                    nxt = j;
                }
            }
        }

        // 还有未接入的点却没有候选边, 说明无法连通
        if (nxt == -1) {
            printf("-1\n");
            return 0;
        }

        total += best[nxt];
        cur = nxt;
    }

    // N = 1 时不需要任何管道, 循环不执行, 输出 0
    printf("%lld\n", total);
    return 0;
}
