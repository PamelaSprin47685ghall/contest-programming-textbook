#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXS = 752 * 752;   // (N+2)^2：N 最大 750，四周再补一圈"无限高"的边框
const int INFH = 1000000000;

int N, W;
int H[MAXS];      // 每个格子的高度，边框格记成 INFH
int order[MAXS];  // 按高度从小到大排好的格子下标

// 并查集，同时维护两个量：连通块的格子数，以及连通块的"总曲率"。
// 曲率按网格角点累计：格点周围 4 个格子的分布决定该点贡献 +1/-1/-3 等，
// 一个连通块的总曲率等于 4 - 4*(洞的个数)，所以总曲率为正就是"没有洞"。
// （这里对洞的判定与题目一致：补集按八连通，斜着贴角不算把洞封死。）
int parent[MAXS];
int compSize[MAXS];
int compCurve[MAXS];

int findRoot(int x) {
    int r = x;
    while (parent[r] != r) r = parent[r];
    while (parent[x] != r) {
        int nxt = parent[x];
        parent[x] = r;
        x = nxt;
    }
    return r;
}

void unite(int a, int b) {
    a = findRoot(a);
    b = findRoot(b);
    if (a == b) return;
    parent[a] = b;
    compSize[b] += compSize[a];
    compCurve[b] += compCurve[a];
}

// a、c 是角点两侧的正交邻居，b 是对角邻居；三个布尔量表示它们是否已和当前格子同属一块。
int cornerCurvature(int a, int b, int c) {
    int s = a + b + c;
    if (s == 0) return 1;
    if (s == 1) return b == 1 ? 1 : -1;
    if (s == 2) return b == 0 ? -3 : -1;
    return 1;
}

int main() {
    scanf("%d", &N);
    W = N + 2;
    for (int s = 0; s < W * W; ++s) H[s] = INFH;
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            int v;
            scanf("%d", &v);
            H[i * W + j] = v;
        }
    }
    for (int s = 0; s < W * W; ++s) {
        order[s] = s;
        parent[s] = s;
        compSize[s] = 0;
        compCurve[s] = 0;
    }

    // 按高度递增逐格插入：插入某格时，已插入的格子恰好是高度比它低的所有格子，
    // 因此当前的每个连通块都是"某个高度阈值下的低地连通块"，其边界格子全都还没插入、都更高。
    sort(order, order + W * W, [](int a, int b) { return H[a] < H[b]; });

    long long answer = 0;
    for (int t = 0; t < W * W; ++t) {
        int cur = order[t];
        if (H[cur] >= INFH) break;  // 轮到自己了，后面全是边框格
        int up = cur - W;
        int down = cur + W;
        int left = cur - 1;
        int right = cur + 1;

        compSize[cur] = 1;
        // 高度互不相同，"邻居高度更小"等价于"邻居已插入"；边框格高度为 INFH，永远不会被并入。
        if (H[up] < H[cur]) unite(cur, up);
        if (H[down] < H[cur]) unite(cur, down);
        if (H[left] < H[cur]) unite(cur, left);
        if (H[right] < H[cur]) unite(cur, right);

        // 新格子只会改变它 4 个角点处的曲率，其余角点不受影响；
        // 用并查集判断每个角点上的三个格子是否与本块相连，再按角点形态累计曲率。
        int rc = findRoot(cur);
        int x = cornerCurvature(findRoot(up) == rc, findRoot(up - 1) == rc, findRoot(left) == rc)
              + cornerCurvature(findRoot(left) == rc, findRoot(down - 1) == rc, findRoot(down) == rc)
              + cornerCurvature(findRoot(down) == rc, findRoot(down + 1) == rc, findRoot(right) == rc)
              + cornerCurvature(findRoot(right) == rc, findRoot(up + 1) == rc, findRoot(up) == rc);
        compCurve[rc] += x;

        // 只有"以当前格为最高点的那一块"才可能是新的山谷：它无洞时恰好满足山谷定义。
        if (compCurve[rc] > 0) answer += compSize[rc];
    }

    printf("%lld\n", answer);
    return 0;
}
