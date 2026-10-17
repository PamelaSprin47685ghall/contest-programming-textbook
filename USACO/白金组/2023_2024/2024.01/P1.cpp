#include <cstdio>

using namespace std;

// 全部概率都在模 1e9+7 下计算，除法一律换成乘逆元。
const int MOD = 1000000007;
const int MAXN = 10005; // 顶点数上限（题目保证所有测试点 N 之和 ≤ 10^4）
const int MAXE = 40005; // 边数上限（M ≤ 3/2·(N-1)，所以 2M < 3·10^4）

int inv[MAXN]; // inv[x] = x 在模 MOD 下的逆元；inv[0] 特判成 0

int N, M;                 // 当前测试点的顶点数、边数
int rawU[MAXE], rawV[MAXE]; // 读入的边（先读一遍才能建 CSR 邻接表）
int contProb[MAXN];       // 1 - p_i：在 i 停留时"不主动结束"的概率

// CSR 邻接表
int degCnt[MAXN], startPos[MAXN + 1], fillPos[MAXN], adjTo[2 * MAXE];

// DFS 树
int parentOf[MAXN], depthOf[MAXN], stackBuf[MAXN];
char onCycEdge[MAXN]; // i 到父亲的边是否在某个环上（等价于 i 有没有"逃逸边"）

// 环：每个环由唯一的非树边 (cycBot -> cycTop) 确定，
// 环上其余顶点就是树的父子链 cycBot, parentOf[cycBot], ..., 环顶的儿子。
int cycTop[MAXN], cycBot[MAXN], cycNext[MAXN], firstCycOf[MAXN], cycTopCnt[MAXN], cycN;
int cycleZ[MAXN]; // 绕某个环整整走一圈又回到环顶的概率

// 后继（父边是桥的儿子、以及以 v 为环顶的环上其余顶点）
int succHead[MAXN], succNext[MAXN], bridgeCnt[MAXN];
int chainBuf[MAXN]; // 临时按自底向上顺序存放一条环上的链

// DP 结果
int escapeProb[MAXN]; // 从 i 出发最终用掉"逃逸边"的概率（无逃逸边则为 0）
int visitProb[MAXN];  // 从"前驱"出发最终访问到 i 的概率（答案挂在后继上）
int reachProb[MAXN];  // 走到 i 的概率
int answer[MAXN];

int polyBuf[MAXN], weightBuf[MAXN]; // 生成函数与它的系数
int ordByDepth[MAXN], depthStart[MAXN + 2];

int add(int a, int b) {
    int s = a + b;
    if (s >= MOD) s -= MOD;
    return s;
}

int sub(int a, int b) {
    int s = a - b;
    if (s < 0) s += MOD;
    return s;
}

int mul(int a, int b) {
    return (int)((long long)a * b % MOD);
}

// 建 DFS 树：用父亲指针 + 深度描述"仙人掌"的块结构
void buildTree() {
    for (int i = 1; i <= N; i++) fillPos[i] = startPos[i];
    int sp = 0;
    stackBuf[sp++] = 1;
    depthOf[1] = 1;
    parentOf[1] = 0;
    while (sp > 0) {
        int u = stackBuf[sp - 1];
        if (fillPos[u] == startPos[u + 1]) {
            sp--;
            continue;
        }
        int v = adjTo[fillPos[u]++];
        // 只在 v 还没访问过时建树边；否则 v 要么是父亲，要么是祖先/后代
        if (depthOf[v] == 0) {
            depthOf[v] = depthOf[u] + 1;
            parentOf[v] = u;
            stackBuf[sp++] = v;
        }
    }
}

// 找出所有环：无向图 DFS 中的非树边一定连接祖先与后代，
// 于是从后代 u 往祖先 v 的方向找，只要 depth[v] < depth[u] 且 v 不是父亲，
// (u, v) 就是确定唯一一个环的非树边。
void findCycles() {
    for (int u = 1; u <= N; u++)
        for (int e = startPos[u]; e < startPos[u + 1]; e++) {
            int v = adjTo[e];
            if (depthOf[v] < depthOf[u] && v != parentOf[u]) {
                cycTop[cycN] = v;
                cycBot[cycN] = u;
                cycNext[cycN] = firstCycOf[v];
                firstCycOf[v] = cycN;
                cycTopCnt[v]++;
                // 链上的每个点都靠它在环上的父边活着，这条父边正是它的"逃逸边"
                for (int k = u; k != v; k = parentOf[k]) onCycEdge[k] = 1;
                cycN++;
            }
        }
}

// 给每个点挂上它的后继：进入子仙人掌的入口
void buildSuccessorList() {
    // 环顶的其它成员都是环顶的后继（可以从两个方向中的任意一个先到达）
    for (int c = 0; c < cycN; c++)
        for (int k = cycBot[c]; k != cycTop[c]; k = parentOf[k]) {
            succNext[k] = succHead[cycTop[c]];
            succHead[cycTop[c]] = k;
        }
    // 父边是桥的儿子：走进去就再也回不来，也是一个独立的入口
    for (int j = 2; j <= N; j++)
        if (!onCycEdge[j]) {
            succNext[j] = succHead[parentOf[j]];
            succHead[parentOf[j]] = j;
            bridgeCnt[parentOf[j]]++;
        }
}

// 按深度做一次计数排序：自底向上（逆序）与自顶向下（正序）各扫一遍即可
void orderByDepth() {
    for (int d = 0; d <= N + 1; d++) depthStart[d] = 0;
    for (int v = 1; v <= N; v++) depthStart[depthOf[v]]++;
    int acc = 0;
    for (int d = 0; d <= N + 1; d++) {
        int c = depthStart[d];
        depthStart[d] = acc;
        acc += c;
    }
    for (int v = 1; v <= N; v++) ordByDepth[depthStart[depthOf[v]]++] = v;
}

// 自底向上：先算出每个点的"逃逸概率"和后继的访问概率
void calcBottomUp() {
    for (int idx = N - 1; idx >= 0; idx--) {
        int v = ordByDepth[idx];
        int K = cycTopCnt[v]; // 以 v 为环顶的环数 = 生成函数次数
        // d 是首次走到 v 时还没走过的相邻边数：桥 + 每个环两条边 + 可能的逃逸边
        int d = bridgeCnt[v] + 2 * K + (onCycEdge[v] ? 1 : 0);
        // weight[c] = c!·2^c·(1-p)^{c+1}·∏_{h≤c} 1/(d-2h)：
        // 先完整走完 c 个环（顺序任意、每个环两个方向），最后选中某一条指定的边
        weightBuf[0] = mul(contProb[v], inv[d]);
        for (int c = 1; c <= K; c++)
            weightBuf[c] = mul(mul(mul(weightBuf[c - 1], 2 * c), contProb[v]), inv[d - 2 * c]);
        // 生成函数 ∏(1 + z_C x)，系数就是"选出的环集合"的初等对称多项式
        polyBuf[0] = 1;
        for (int c = 1; c <= K; c++) polyBuf[c] = 0;
        int deg = 0;
        for (int c = firstCycOf[v]; c != -1; c = cycNext[c]) {
            int z = 1;
            for (int k = cycBot[c]; k != cycTop[c]; k = parentOf[k]) z = mul(z, escapeProb[k]);
            cycleZ[c] = z;
            for (int h = deg; h >= 0; h--) polyBuf[h + 1] = add(polyBuf[h + 1], mul(polyBuf[h], z));
            deg++;
        }
        int base = 0; // 最终选中任意一条"指定的单条边"的概率
        for (int h = 0; h <= K; h++) base = add(base, mul(polyBuf[h], weightBuf[h]));
        if (onCycEdge[v]) escapeProb[v] = base;
        for (int j = succHead[v]; j != 0; j = succNext[j])
            if (!onCycEdge[j]) visitProb[j] = base; // 桥儿子：选中那条桥即可到达
        // 环上的后继：还要按方向累乘路径上各点的逃逸概率
        for (int c = firstCycOf[v]; c != -1; c = cycNext[c]) {
            int z = cycleZ[c];
            // 除以 (1 + z x)：倒着做一次乘法的逆运算，O(K) 得到"不含本环"的生成函数
            for (int h = 1; h <= K; h++) polyBuf[h] = sub(polyBuf[h], mul(polyBuf[h - 1], z));
            int y = 0;
            for (int h = 0; h <= K; h++) y = add(y, mul(polyBuf[h], weightBuf[h]));
            for (int h = K; h >= 1; h--) polyBuf[h] = add(polyBuf[h], mul(polyBuf[h - 1], z));
            int m = 0;
            for (int k = cycBot[c]; k != cycTop[c]; k = parentOf[k]) chainBuf[m++] = k;
            // 方向一：从环底经非树边进入环，chainBuf[0] 最近
            int acc = y;
            for (int h = 0; h < m; h++) {
                visitProb[chainBuf[h]] = add(visitProb[chainBuf[h]], acc);
                acc = mul(acc, escapeProb[chainBuf[h]]);
            }
            // 方向二：从环顶的儿子沿树边进入环，chainBuf[m-1] 最近
            acc = y;
            for (int h = m - 1; h >= 0; h--) {
                visitProb[chainBuf[h]] = add(visitProb[chainBuf[h]], acc);
                acc = mul(acc, escapeProb[chainBuf[h]]);
            }
        }
    }
}

// 自顶向下：传递到达概率，并统计"恰好停在 v"的概率
void calcTopDown() {
    reachProb[1] = 1;
    for (int idx = 0; idx < N; idx++) {
        int v = ordByDepth[idx];
        // 停在 v ⇔ 不在 v 用掉逃逸边，也不会走某个后继后永远困在它的子仙人掌里
        int x = sub(1, escapeProb[v]);
        for (int j = succHead[v]; j != 0; j = succNext[j])
            x = sub(x, mul(visitProb[j], sub(1, escapeProb[j])));
        answer[v] = mul(reachProb[v], x);
        for (int j = succHead[v]; j != 0; j = succNext[j])
            reachProb[j] = mul(reachProb[v], visitProb[j]);
    }
}

void solveCase() {
    scanf("%d%d", &N, &M);
    for (int i = 1; i <= N; i++) {
        int p;
        scanf("%d", &p);
        contProb[i] = sub(1, p);
    }
    for (int i = 1; i <= N; i++) degCnt[i] = 0;
    for (int e = 0; e < M; e++) {
        scanf("%d%d", &rawU[e], &rawV[e]);
        degCnt[rawU[e]]++;
        degCnt[rawV[e]]++;
    }
    startPos[1] = 0;
    for (int i = 1; i <= N; i++) startPos[i + 1] = startPos[i] + degCnt[i];
    for (int i = 1; i <= N; i++) fillPos[i] = startPos[i];
    for (int e = 0; e < M; e++) {
        adjTo[fillPos[rawU[e]]++] = rawV[e];
        adjTo[fillPos[rawV[e]]++] = rawU[e];
    }
    for (int i = 1; i <= N; i++) {
        depthOf[i] = 0;
        onCycEdge[i] = 0;
        cycTopCnt[i] = 0;
        firstCycOf[i] = -1;
        succHead[i] = 0;
        bridgeCnt[i] = 0;
        escapeProb[i] = 0;
        visitProb[i] = 0;
        reachProb[i] = 0;
    }
    cycN = 0;
    buildTree();
    findCycles();
    buildSuccessorList();
    orderByDepth();
    calcBottomUp();
    calcTopDown();
    for (int i = 1; i <= N; i++) printf("%d%c", answer[i], i == N ? '\n' : ' ');
}

int main() {
    // inv[0] 留成 0：当 d-2c 变成 0 时已经无路可走，"再选一条边"的概率必然是 0
    inv[0] = 0;
    inv[1] = 1;
    for (int i = 2; i < MAXN; i++) inv[i] = (int)((long long)(MOD - MOD / i) * inv[MOD % i] % MOD);
    int T;
    scanf("%d", &T);
    while (T--) solveCase();
    return 0;
}
