#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 250;
const int MAXCELL = MAXN * MAXN;

int n;
int board[MAXN][MAXN];
int comp[MAXN][MAXN];        // 每个格子所属的同值连通块编号(从 1 开始, 0 表示未访问)
int compSize[MAXCELL + 1];   // 每个连通块的大小
int compValue[MAXCELL + 1];  // 每个连通块的数值
int compCount;

const int DX[4] = {-1, 1, 0, 0};
const int DY[4] = {0, 0, -1, 1};

int stackX[MAXCELL];  // 洪泛填充用的显式栈: 区域可能很大, 递归会爆栈
int stackY[MAXCELL];

struct Edge {
    long long key;  // 值对 (较小值, 较大值) 的编码, 用来把同值对的边界边聚在一起
    int a, b;       // 相邻的两个连通块, 分别由这两种值的牛占领
};

Edge edges[2 * MAXCELL + 5];
int edgeCount;

bool cmpEdge(const Edge &x, const Edge &y) { return x.key < y.key; }

// 并查集: 用"戳"(stamp) 做惰性初始化, 否则每处理一个值对都要清空整个 parent 数组, 代价过大
int parent[MAXCELL + 1];
int dsuStamp[MAXCELL + 1];
int curStamp;

int findRoot(int x) {
    if (dsuStamp[x] != curStamp) {  // 本值对内第一次见到 x: 自成一个集合
        dsuStamp[x] = curStamp;
        parent[x] = x;
        return x;
    }
    int root = x;
    while (parent[root] != root) root = parent[root];
    while (parent[x] != root) {  // 路径压缩
        int nxt = parent[x];
        parent[x] = root;
        x = nxt;
    }
    return root;
}

void unite(int a, int b) {
    int ra = findRoot(a), rb = findRoot(b);
    if (ra != rb) parent[ra] = rb;
}

// 统计每个连通块分量的大小时, 同样用"戳"避免重复累加与反复清零
int seenNode[MAXCELL + 1];
int seenRoot[MAXCELL + 1];
int rootSum[MAXCELL + 1];

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) scanf("%d", &board[i][j]);

    // 第一步: 洪泛填充, 求出所有"同值四连通"的连通块(区域)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (comp[i][j] != 0) continue;
            int id = ++compCount;
            compValue[id] = board[i][j];
            int top = 0;
            comp[i][j] = id;
            stackX[top] = i;
            stackY[top] = j;
            top++;
            int size = 0;
            while (top > 0) {
                top--;
                int x = stackX[top], y = stackY[top];
                size++;
                for (int d = 0; d < 4; d++) {
                    int nx = x + DX[d], ny = y + DY[d];
                    if (nx < 0 || nx >= n || ny < 0 || ny >= n) continue;
                    if (comp[nx][ny] != 0) continue;
                    if (board[nx][ny] != board[x][y]) continue;
                    comp[nx][ny] = id;
                    stackX[top] = nx;
                    stackY[top] = ny;
                    top++;
                }
            }
            compSize[id] = size;
        }
    }

    int answer1 = 0;
    for (int id = 1; id <= compCount; id++)
        if (compSize[id] > answer1) answer1 = compSize[id];

    // 第二步: 找出所有"相邻且数值不同"的连通块对, 并按它们所属的值对分组.
    // 只有这样的两块才有可能被同一支队伍的两种编号连起来.
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i + 1 < n && board[i + 1][j] != board[i][j]) {
                int v1 = board[i][j], v2 = board[i + 1][j];
                if (v1 > v2) { int t = v1; v1 = v2; v2 = t; }
                edges[edgeCount].key = (long long)v1 * 1000001LL + v2;
                edges[edgeCount].a = comp[i][j];
                edges[edgeCount].b = comp[i + 1][j];
                edgeCount++;
            }
            if (j + 1 < n && board[i][j + 1] != board[i][j]) {
                int v1 = board[i][j], v2 = board[i][j + 1];
                if (v1 > v2) { int t = v1; v1 = v2; v2 = t; }
                edges[edgeCount].key = (long long)v1 * 1000001LL + v2;
                edges[edgeCount].a = comp[i][j];
                edges[edgeCount].b = comp[i][j + 1];
                edgeCount++;
            }
        }
    }

    sort(edges, edges + edgeCount, cmpEdge);

    // 第三步: 对每个值对, 把该值对内部的相邻连通块合并, 其中最大的连通分量就是这对牛的最大区域
    int answer2 = 0;
    int i = 0;
    while (i < edgeCount) {
        int j = i;
        curStamp++;
        while (j < edgeCount && edges[j].key == edges[i].key) {
            unite(edges[j].a, edges[j].b);
            j++;
        }
        for (int k = i; k < j; k++) {
            int nodes[2] = {edges[k].a, edges[k].b};
            for (int t = 0; t < 2; t++) {
                int c = nodes[t];
                if (seenNode[c] == curStamp) continue;  // 每个连通块的大小只累加一次
                seenNode[c] = curStamp;
                int root = findRoot(c);
                if (seenRoot[root] != curStamp) {
                    seenRoot[root] = curStamp;
                    rootSum[root] = 0;
                }
                rootSum[root] += compSize[c];
                if (rootSum[root] > answer2) answer2 = rootSum[root];
            }
        }
        i = j;
    }

    printf("%d\n%d\n", answer1, answer2);
    return 0;
}
