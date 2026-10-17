#include <cstdio>

using namespace std;

const int MAXV = 1500 * 1500;  // 格子总数上限

int N, M, Q, total;      // 行数、列数、询问数、格子总数
char grid[MAXV + 5];     // 每个格子的字符
char rowBuf[1505];       // 读入一行网格用的缓冲

const int dr[4] = {-1, 0, 1, 0};  // 0 上、1 右、2 下、3 左
const int dc[4] = {0, 1, 0, -1};

int startPos;   // 奶牛初始位置
int boxPos;     // 箱子初始位置
int boxDir0;    // 奶牛相对箱子的初始方向（即奶牛站在 箱子+dir[boxDir0]）；-1 表示走不到箱子旁

int dfn[MAXV + 5], low[MAXV + 5];
char parentDir[MAXV + 5];       // 父边方向（由父亲指向自己），根记为 -1
int dfsStack[MAXV + 5];         // 迭代版 DFS 的顶点栈
char dfsDir[MAXV + 5];          // 每个顶点下一个要尝试的方向
int edgeStack[4 * MAXV + 5];    // Tarjan 的边栈，边用 顶点*4+方向 编码
int edgeTop;
int timerCnt, blockCnt;
int blockId[MAXV + 5][4];       // 每条边所属的块（点双连通分量）编号
unsigned short sideMask[MAXV + 5];  // 每个格子的四个方向邻居按“能否绕过本格互相到达”分组

char stateVisited[MAXV + 5];    // 用第 d 位表示“箱子在本格、奶牛在 本格+dir[d]”这一状态
int bfsQueue[4 * MAXV + 5];

// 格子 p 沿方向 d 走一步到达的格子；越界或为干草时返回 -1
int neighbor(int p, int d) {
    int q;
    if (d == 0) {
        q = p - M;
        if (q < 0) return -1;
    } else if (d == 2) {
        q = p + M;
        if (q >= total) return -1;
    } else if (d == 1) {
        if (p % M == M - 1) return -1;
        q = p + 1;
    } else {
        if (p % M == 0) return -1;
        q = p - 1;
    }
    if (grid[q] == '#') return -1;
    return q;
}

// 迭代版 Tarjan：从 root 出发做一次 DFS，同时用边栈把每条边归入一个块。
// 边的两个端点属于同一个块，当且仅当删掉它们之外的任意一个格子后仍连通。
void dfsTree(int root, bool fromStart) {
    int top = 0;
    dfn[root] = low[root] = ++timerCnt;
    parentDir[root] = -1;
    dfsStack[0] = root;
    dfsDir[0] = 0;
    while (top >= 0) {
        int u = dfsStack[top];
        int d = dfsDir[top];
        if (d == 4) {
            // u 的所有邻边都处理完了，回溯
            --top;
            if (top >= 0) {
                int p = dfsStack[top];
                if (low[u] < low[p]) low[p] = low[u];
                if (low[u] >= dfn[p]) {
                    // p 是割点（或根），弹出一个块：直到弹出树边 p->u 为止
                    ++blockCnt;
                    int stopEdge = p * 4 + (int)parentDir[u];
                    while (true) {
                        int e = edgeStack[--edgeTop];
                        int a = e >> 2, da = e & 3;
                        int b = neighbor(a, da);
                        blockId[a][da] = blockCnt;
                        blockId[b][(da + 2) & 3] = blockCnt;
                        if (e == stopEdge) break;
                    }
                }
            }
            continue;
        }
        dfsDir[top] = d + 1;
        int v = neighbor(u, d);
        if (v < 0) continue;
        if (parentDir[u] != (char)-1 && d == ((int)parentDir[u] + 2) % 4) continue;  // 不走回父边
        if (dfn[v] == 0) {
            // 奶牛从 u 走到盒子旁：说明 u 不经过盒子就能到达，方向为 u->盒子
            if (fromStart && v == boxPos && boxDir0 < 0) boxDir0 = (d + 2) % 4;
            parentDir[v] = (char)d;
            dfn[v] = low[v] = ++timerCnt;
            edgeStack[edgeTop++] = u * 4 + d;
            ++top;
            dfsStack[top] = v;
            dfsDir[top] = 0;
        } else if (dfn[v] < dfn[u]) {
            // 指向祖先的反向边：入栈，它可能和上面的边同属一个块；
            // 同时用它把 low[u] 拉向更浅的祖先
            if (dfn[v] < low[u]) low[u] = dfn[v];
            edgeStack[edgeTop++] = u * 4 + d;
        }
    }
}

// 对每个格子，把它四周的开阔邻居按块编号分成若干组，组号（1..4）压进 2 字节
void buildSideMask() {
    for (int p = 0; p < total; ++p) {
        if (grid[p] == '#') continue;
        int lab[4] = {0, 0, 0, 0};
        int cnt = 0;
        unsigned short mask = 0;
        for (int d = 0; d < 4; ++d) {
            if (neighbor(p, d) < 0) continue;
            int l = 0;
            for (int e = 0; e < d; ++e)
                if (lab[e] != 0 && blockId[p][e] == blockId[p][d]) {
                    l = lab[e];
                    break;
                }
            if (l == 0) l = ++cnt;
            lab[d] = l;
            mask |= (unsigned short)(l << (4 * d));
        }
        sideMask[p] = mask;
    }
}

// 状态 = 箱子所在格子 * 4 + 奶牛相对箱子的方向
void bfs() {
    int head = 0, tail = 0;
    stateVisited[boxPos] = (char)(1 << boxDir0);
    bfsQueue[tail++] = boxPos * 4 + boxDir0;
    while (head < tail) {
        int s = bfsQueue[head++];
        int p = s >> 2, d = s & 3;
        // 奶牛把箱子朝自己的方向推：箱子前进一格，奶牛站到箱子的旧位置
        int np = neighbor(p, (d + 2) & 3);
        if (np >= 0 && !(stateVisited[np] & (1 << d))) {
            stateVisited[np] |= (char)(1 << d);
            bfsQueue[tail++] = np * 4 + d;
        }
        // 绕着箱子走到另一侧：两侧邻居要在同一个块里才能绕过本格
        int mySide = (sideMask[p] >> (4 * d)) & 15;
        for (int d2 = 0; d2 < 4; ++d2) {
            if (d2 == d) continue;
            int other = (sideMask[p] >> (4 * d2)) & 15;
            if (other == 0 || other != mySide) continue;
            if (stateVisited[p] & (1 << d2)) continue;
            stateVisited[p] |= (char)(1 << d2);
            bfsQueue[tail++] = p * 4 + d2;
        }
    }
}

int main() {
    scanf("%d%d%d", &N, &M, &Q);
    total = N * M;
    for (int r = 0; r < N; ++r) {
        scanf("%s", rowBuf);
        for (int c = 0; c < M; ++c) {
            grid[r * M + c] = rowBuf[c];
            if (rowBuf[c] == 'A') {
                startPos = r * M + c;
            } else if (rowBuf[c] == 'B') {
                boxPos = r * M + c;
            }
        }
    }
    boxDir0 = -1;
    // 先从奶牛起点搜一遍（顺带确定初始方向），再把其余连通块补全
    dfsTree(startPos, true);
    for (int p = 0; p < total; ++p)
        if (grid[p] != '#' && dfn[p] == 0) dfsTree(p, false);
    buildSideMask();
    if (boxDir0 >= 0) bfs();
    for (int i = 0; i < Q; ++i) {
        int R, C;
        scanf("%d%d", &R, &C);
        int p = (R - 1) * M + (C - 1);
        bool ok = (p == boxPos) || stateVisited[p] != 0;
        printf("%s\n", ok ? "YES" : "NO");
    }
    return 0;
}
