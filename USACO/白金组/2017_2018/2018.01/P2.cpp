#include <cstdio>

using namespace std;

const int MAXN = 70005;     // 谷仓数上限
const int MAXE = 2 * MAXN;  // 无向边存两份
const int INF = 1000000000;

int N;
int head[MAXN], nxt[MAXE], to[MAXE], ecnt;  // 前向星邻接表
int deg[MAXN];

void addEdge(int u, int v) {
    to[ecnt] = v;
    nxt[ecnt] = head[u];
    head[u] = ecnt++;
}

int leafDist[MAXN];  // 每个谷仓到最近出口（度为 1 的点）的距离
int weight[MAXN];    // 2 - deg，用来把 S_u 的连通块个数写成“点和”的形式
int qSum[MAXN];      // qSum[u] = Σ_{v: dist(u,v) < L(v)} w(v)，答案就是 2 - qSum[u]

// 遍历用的全局数组（同一时刻只服务于一层调用）
int bfsOrder[MAXN], bfsPar[MAXN], bfsDist[MAXN];
bool done[MAXN];    // 已经在重心分解里被当作重心的点
int sz[MAXN];
int cntKey[MAXN];   // key 一定落在 [0, N] 内

// 从 start 出发遍历它所在的连通块（跳过已经处理掉的点），返回点数并写入 bfsOrder
int collect(int start) {
    int cnt = 0;
    bfsOrder[cnt] = start;
    bfsPar[start] = -1;
    bfsDist[start] = 0;
    ++cnt;
    for (int i = 0; i < cnt; ++i) {
        int x = bfsOrder[i];
        for (int ei = head[x]; ei != -1; ei = nxt[ei]) {
            int y = to[ei];
            if (y == bfsPar[x] || done[y]) continue;  // 是父亲或者已经被当作重心删掉
            bfsPar[y] = x;
            bfsDist[y] = bfsDist[x] + 1;
            bfsOrder[cnt++] = y;
        }
    }
    return cnt;
}

void decompose(int start) {
    int cnt = collect(start);
    // 倒着扫 BFS 序，就能保证先算完儿子再累加到父亲
    for (int i = 0; i < cnt; ++i) sz[bfsOrder[i]] = 1;
    for (int i = cnt - 1; i >= 1; --i) sz[bfsPar[bfsOrder[i]]] += sz[bfsOrder[i]];
    // 重心：把它删掉后，剩下每个部分的大小都不超过 cnt/2
    int center = -1, bestPart = cnt + 1;
    for (int i = 0; i < cnt; ++i) {
        int x = bfsOrder[i];
        int maxPart = cnt - sz[x];  // 父亲那一侧
        for (int ei = head[x]; ei != -1; ei = nxt[ei]) {
            int y = to[ei];
            if (done[y] || y == bfsPar[x]) continue;
            if (sz[y] > maxPart) maxPart = sz[y];
        }
        if (maxPart < bestPart) {
            bestPart = maxPart;
            center = x;
        }
    }
    done[center] = true;

    // 重心分解的每一层：把“路径经过 center”的点对在这里一次性结算。
    // v 被算进 u 的条件是 dist(u,v) = dist(u,center) + dist(v,center) <= L(v)-1，
    // 于是把 key(v) = L(v)-1-dist(v,center) 塞进桶里，问 u 时取 key >= dist(u,center) 的和。
    int cc = collect(center);
    int maxKey = -1;
    for (int i = 0; i < cc; ++i) {
        int x = bfsOrder[i];
        int key = leafDist[x] - 1 - bfsDist[x];
        if (key >= 0) {
            cntKey[key] += weight[x];
            if (key > maxKey) maxKey = key;
        }
    }
    for (int k = maxKey - 1; k >= 0; --k) cntKey[k] += cntKey[k + 1];  // 后缀和
    for (int i = 0; i < cc; ++i) {
        int x = bfsOrder[i];
        if (bfsDist[x] <= maxKey) qSum[x] += cntKey[bfsDist[x]];
    }
    for (int k = 0; k <= maxKey; ++k) cntKey[k] = 0;

    // 同一个儿子子树里的点对，路径不经过 center，这一层是“算多了”的，要减掉，
    // 交给下一层递归去算。
    for (int ei = head[center]; ei != -1; ei = nxt[ei]) {
        int ch = to[ei];
        if (done[ch]) continue;
        int cs = collect(ch);
        int mKey = -1;
        for (int i = 0; i < cs; ++i) {
            int x = bfsOrder[i];
            int dC = bfsDist[x] + 1;  // 到 center 的距离
            int key = leafDist[x] - 1 - dC;
            if (key >= 0) {
                cntKey[key] += weight[x];
                if (key > mKey) mKey = key;
            }
        }
        for (int k = mKey - 1; k >= 0; --k) cntKey[k] += cntKey[k + 1];
        for (int i = 0; i < cs; ++i) {
            int x = bfsOrder[i];
            int dC = bfsDist[x] + 1;
            if (dC <= mKey) qSum[x] -= cntKey[dC];
        }
        for (int k = 0; k <= mKey; ++k) cntKey[k] = 0;
        decompose(ch);
    }
}

int main() {
    scanf("%d", &N);
    for (int i = 0; i < N; ++i) {
        head[i] = -1;
        deg[i] = 0;
        leafDist[i] = INF;
    }
    ecnt = 0;
    for (int i = 0; i + 1 < N; ++i) {
        int a, b;
        scanf("%d %d", &a, &b);
        --a;
        --b;
        addEdge(a, b);
        addEdge(b, a);
        ++deg[a];
        ++deg[b];
    }

    // 两遍遍历求“到最近出口的距离”：先自底向上，再用父亲的信息自顶向下。
    // 用 BFS 序代替递归，链状的树也不会爆栈。
    int cnt = 0;
    bfsOrder[cnt] = 0;
    bfsPar[0] = -1;
    ++cnt;
    for (int i = 0; i < cnt; ++i) {
        int x = bfsOrder[i];
        for (int ei = head[x]; ei != -1; ei = nxt[ei]) {
            int y = to[ei];
            if (y == bfsPar[x]) continue;
            bfsPar[y] = x;
            bfsOrder[cnt++] = y;
        }
    }
    for (int i = cnt - 1; i >= 0; --i) {
        int x = bfsOrder[i];
        if (deg[x] == 1) leafDist[x] = 0;  // 出口自己到最近出口的距离是 0
        if (bfsPar[x] != -1 && leafDist[x] + 1 < leafDist[bfsPar[x]])
            leafDist[bfsPar[x]] = leafDist[x] + 1;
    }
    for (int i = 1; i < cnt; ++i) {
        int x = bfsOrder[i];
        if (leafDist[bfsPar[x]] + 1 < leafDist[x]) leafDist[x] = leafDist[bfsPar[x]] + 1;
    }
    for (int i = 0; i < N; ++i) weight[i] = 2 - deg[i];

    decompose(0);

    for (int i = 0; i < N; ++i) {
        if (deg[i] == 1) {
            printf("1\n");  // 就在出口上冒头，1 个农夫足够
        } else {
            // S_i = {v : L(v) <= dist(i,v)} 是若干棵子树之并，每棵子树贡献 Σ(2-deg) = 1，
            // 所以连通块个数 = Σ_{v∈S_i} w(v) = (Σ_all w) - Σ_{v∉S_i} w = 2 - qSum[i]
            printf("%d\n", 2 - qSum[i]);
        }
    }
    return 0;
}
