#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXDIM = 505;
const int MAXCELL = MAXDIM * MAXDIM;
const int MAXEDGE = 2 * MAXDIM * MAXDIM;

int rows, cols, need;
int height[MAXDIM][MAXDIM];
int parent[MAXCELL];
int compSize[MAXCELL];
int answer[MAXCELL];  // 该点所在连通块首次凑够 need 个格子时的落差, -1 表示还没凑够

struct Edge {
    int diff;
    int u;
    int v;

    bool operator<(const Edge &other) const {
        return diff < other.diff;
    }
};

Edge edges[MAXEDGE];
int edgeCnt;

int CellId(int r, int c) {
    return r * cols + c;
}

// 并查集, 额外把"已经定下来的答案"沿父链传给下面的点: 一个连通块首次凑够
// need 个格子时, 块里每个格子的难度就固定了, 之后再和谁合并都不会变; 而当时
// 打上答案标记的是那棵树的根, 现在这个根可能已经变成别人的儿子, 所以要传递.
int Find(int x) {
    if (parent[x] == x) return x;
    int p = parent[x];
    int root = Find(p);
    if (answer[x] == -1 && answer[p] != -1) answer[x] = answer[p];
    parent[x] = root;
    return root;
}

void Union(int a, int b, int diff) {
    // a, b 是两棵树的根. 合并之后才第一次凑够 need 个格子的那一边, 答案就是
    // 当前这条边的落差: 更小的边都已经加完了, 那时它自己还不够 need 个格子.
    if (compSize[a] >= need) {
        if (compSize[b] < need) answer[b] = diff;
    } else if (compSize[b] >= need) {
        answer[a] = diff;
    } else if (compSize[a] + compSize[b] >= need) {
        answer[a] = diff;
        answer[b] = diff;
    }

    if (compSize[a] < compSize[b]) {
        parent[a] = b;
        compSize[b] += compSize[a];
    } else {
        parent[b] = a;
        compSize[a] += compSize[b];
    }
}

int main() {
    scanf("%d %d %d", &rows, &cols, &need);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) scanf("%d", &height[r][c]);
    }

    // 相邻格子的落差就是图的边权, 每个相邻关系只连一次(只往右和往下).
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (r + 1 < rows) {
                int diff = height[r][c] - height[r + 1][c];
                if (diff < 0) diff = -diff;
                edges[edgeCnt].diff = diff;
                edges[edgeCnt].u = CellId(r, c);
                edges[edgeCnt].v = CellId(r + 1, c);
                edgeCnt++;
            }
            if (c + 1 < cols) {
                int diff = height[r][c] - height[r][c + 1];
                if (diff < 0) diff = -diff;
                edges[edgeCnt].diff = diff;
                edges[edgeCnt].u = CellId(r, c);
                edges[edgeCnt].v = CellId(r, c + 1);
                edgeCnt++;
            }
        }
    }

    for (int i = 0; i < rows * cols; i++) {
        parent[i] = i;
        compSize[i] = 1;
        answer[i] = (need <= 1) ? 0 : -1;  // 只要 1 个格子时难度为 0
    }
    sort(edges, edges + edgeCnt);

    // 按落差从小到大加边: 加完所有落差 <= D 的边之后, 每个连通块正好是从块内
    // 任意格子出发、落差不超过 D 能走到的全部格子, 于是连通块第一次涨到 need
    // 个格子时的那条边权, 就是块内每个起点的难度.
    for (int i = 0; i < edgeCnt; i++) {
        int a = Find(edges[i].u);
        int b = Find(edges[i].v);
        if (a != b) Union(a, b, edges[i].diff);
    }

    long long sum = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int isStart;
            scanf("%d", &isStart);
            if (isStart) {
                int id = CellId(r, c);
                Find(id);  // 取答案前先把标记沿线传下来
                sum += answer[id];
            }
        }
    }
    printf("%lld\n", sum);
    return 0;
}
