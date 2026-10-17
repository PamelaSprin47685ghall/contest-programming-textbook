#include <cstdio>

using namespace std;

const int MAXN = 200000;                    // 顶点数上限
const long long INF = 1000000000000000LL;   // 远大于任何可行代价的"无穷大"

int N;
int parent[MAXN + 1];      // 每个顶点的父亲
int subtreeSize[MAXN + 1]; // 子树大小

// state[x][a][b]：只处理子树 x 内的边（以及从 x 连向虚拟点 S 的边）时的最小代价。
// a = 1 表示 x 所在连通块已经连上 S；a = 0 表示还没连上（必须靠父亲那条边接上）。
// b 表示 x 当前度数的奇偶性。
// 约定：子树内除了 x 所在连通块以外的所有顶点，都已经连上了 S。
long long state[MAXN + 1][2][2];

int main() {
    scanf("%d", &N);
    for (int i = 2; i <= N; ++i) scanf("%d", &parent[i]);

    for (int v = 1; v <= N; ++v) subtreeSize[v] = 1;
    // 题目保证 p_i < i，所以儿子编号一定大于父亲，倒序累加即可
    for (int v = N; v >= 2; --v) subtreeSize[parent[v]] += subtreeSize[v];

    for (int v = 1; v <= N; ++v) {
        state[v][0][0] = 0;                     // 不用任何边：不连 S，度数为偶
        state[v][0][1] = INF;                   // 不连 S 时度数不可能为奇，标记为不可达
        state[v][1][0] = 2LL * subtreeSize[v];  // S-v 边走两次：连上 S 且度数为偶
        state[v][1][1] = subtreeSize[v];        // S-v 边走一次：连上 S 但度数为奇
    }

    // 自底向上合并：处理完 v 后把它的状态并入父亲
    for (int v = N; v >= 2; --v) {
        int p = parent[v];
        // 在原问题的度量里，从"激活 p 的子树"变到"激活 v 的子树"要翻转虚线两侧的顶点，
        // 恰好是 sub[p] - sub[v] 次操作，这就是 p-v 这条边的权值
        long long edgeCost = subtreeSize[p] - subtreeSize[v];

        long long merged[2][2];
        for (int a = 0; a < 2; ++a)
            for (int b = 0; b < 2; ++b) merged[a][b] = INF;

        for (int a = 0; a < 2; ++a) {
            for (int b = 0; b < 2; ++b) {
                if (state[p][a][b] >= INF) continue;
                for (int c = 0; c < 2; ++c) {
                    for (int d = 0; d < 2; ++d) {
                        if (state[v][c][d] >= INF) continue;
                        long long base = state[p][a][b] + state[v][c][d];
                        if (d == 1) {
                            // p-v 走一次：v 的度数由奇变偶，p 的度数奇偶翻转；
                            // 两块合并后是否连上 S 取"或"
                            long long cand = base + edgeCost;
                            if (cand < merged[a | c][b ^ 1]) merged[a | c][b ^ 1] = cand;
                        } else {
                            // p-v 一次也不走：v 所在连通块必须已经连上 S，否则再没机会
                            if (c == 1 && base < merged[a][b]) merged[a][b] = base;
                            // p-v 走两次：两边的度数都不变，两块合并
                            long long cand = base + 2 * edgeCost;
                            if (cand < merged[a | c][b]) merged[a | c][b] = cand;
                        }
                    }
                }
            }
        }
        for (int a = 0; a < 2; ++a)
            for (int b = 0; b < 2; ++b)
                state[p][a][b] = (merged[a][b] < INF) ? merged[a][b] : INF;
    }

    // 根必须连上 S（否则整张图不连通），并且度数必须为偶（欧拉回路的要求）
    printf("%lld\n", state[1][1][0]);
    return 0;
}
