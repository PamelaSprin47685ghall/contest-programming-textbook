#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 100005;
const int MAXLEAF = 1 << 17;             // 2^17 = 131072 >= N，线段树叶子数上限
const long long NEG = -(long long)1e18;  // 端点处不可能被跳过，用它保证不会成为区间最大值

int n, q;
int posX[MAXN], posY[MAXN];

int leafCount;                   // 线段树的叶子数（取不小于 n 的 2 的幂）
long long treeSum[2 * MAXLEAF];  // 区间内各相邻检查点距离之和
long long treeMax[2 * MAXLEAF];  // 区间内"跳过某个中间检查点能省下的距离"的最大值

// 网格街道，两点之间只能横竖走，所以距离是曼哈顿距离
int manhattan(int a, int b) {
    int dx = posX[a] - posX[b];
    if (dx < 0) dx = -dx;
    int dy = posY[a] - posY[b];
    if (dy < 0) dy = -dy;
    return dx + dy;
}

// 位置 k 的叶子同时存两个量：
//   d[k] = k 到 k+1 的距离；
//   g[k] = 跳过 k 时省下的距离 = d[k-1] + d[k] - dist(k-1, k+1)，
//          只有 2 <= k <= n-1 才是可跳过的中间点，两端点置为 NEG。
void writeLeaf(int k) {
    int p = leafCount + k - 1;
    treeSum[p] = manhattan(k, k + 1);
    treeMax[p] = (k >= 2 && k <= n - 1)
                     ? manhattan(k - 1, k) + manhattan(k, k + 1) - manhattan(k - 1, k + 1)
                     : NEG;
}

// 自底向上重算叶子 k 的所有祖先
void refresh(int k) {
    for (int p = (leafCount + k - 1) >> 1; p >= 1; p >>= 1) {
        treeSum[p] = treeSum[2 * p] + treeSum[2 * p + 1];
        treeMax[p] = max(treeMax[2 * p], treeMax[2 * p + 1]);
    }
}

int main() {
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; ++i) {
        scanf("%d%d", &posX[i], &posY[i]);
    }
    leafCount = 1;
    while (leafCount < n) leafCount <<= 1;
    for (int p = 0; p < 2 * leafCount; ++p) treeMax[p] = NEG;
    for (int k = 1; k <= n - 1; ++k) writeLeaf(k);
    for (int p = leafCount - 1; p >= 1; --p) {
        treeSum[p] = treeSum[2 * p] + treeSum[2 * p + 1];
        treeMax[p] = max(treeMax[2 * p], treeMax[2 * p + 1]);
    }

    char op[4];
    for (int query = 0; query < q; ++query) {
        scanf("%s", op);
        if (op[0] == 'U') {
            int i, newX, newY;
            scanf("%d%d%d", &i, &newX, &newY);
            posX[i] = newX;
            posY[i] = newY;
            // 只有 k = i-1, i, i+1 这几个叶子的 d[k] 或 g[k] 依赖于检查点 i
            int lo = max(1, i - 1), hi = min(n - 1, i + 1);
            for (int k = lo; k <= hi; ++k) writeLeaf(k);
            for (int k = lo; k <= hi; ++k) refresh(k);
        } else {
            int i, j;
            scanf("%d%d", &i, &j);
            if (i == j) {  // 子路线只有一个检查点，不需要移动
                printf("0\n");
                continue;
            }
            long long answer = 0;
            int a = i + leafCount - 1, b = (j - 1) + leafCount - 1;
            while (a <= b) {  // 不跳过任何点时，总路程是 d[i..j-1] 的和
                if (a & 1) answer += treeSum[a++];
                if (!(b & 1)) answer += treeSum[b--];
                a >>= 1;
                b >>= 1;
            }
            if (j >= i + 2) {  // 中间有点可跳：再减去其中最大的节省量
                long long best = NEG;
                a = (i + 1) + leafCount - 1;
                b = (j - 1) + leafCount - 1;
                while (a <= b) {
                    if (a & 1) best = max(best, treeMax[a++]);
                    if (!(b & 1)) best = max(best, treeMax[b--]);
                    a >>= 1;
                    b >>= 1;
                }
                answer -= best;
            }
            printf("%lld\n", answer);
        }
    }
    return 0;
}
