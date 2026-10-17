#include <cstdio>

using namespace std;

const int MOD = 1000000007;
const int MAXV = 400005; // 原图最多 2e5 个点，每次合并再新建一个虚点，总点数不到 4e5

int pow10[MAXV]; // 10 的幂，下标是序列长度
int dsuParent[MAXV];
// 每个点到它在并查集中的父亲所代表的那一段序列（长度、值）。
// 沿父亲一路把段首尾相接，就得到这个点当前的 h。
int partLen[MAXV];
int partRes[MAXV];
int pathBuf[MAXV]; // findRoot 沿途记下的路径，便于自顶向下补全

struct Value {
    int len; // 序列长度（也等于所在连通分量点数减一）
    int res; // 序列的值模 1e9+7
};

// 把另一段序列接在当前点已存的段后面（当前段在前）
void appendAfter(int x, int otherLen, int otherRes) {
    partRes[x] = (int)(((long long)pow10[otherLen] * partRes[x] + otherRes) % MOD);
    partLen[x] += otherLen;
}

// 带路径压缩的并查集：顺带把沿途各点到父亲的那一段依次拼成到根的一整段。
// 写成迭代是为了防住"合并顺序恶意"时递归深度达到 O(N) 的爆栈风险。
int findRoot(int x) {
    int len = 0;
    int cur = x;
    while (dsuParent[cur] != cur) {
        pathBuf[len++] = cur;
        cur = dsuParent[cur];
    }
    int r = cur;
    // 自顶向下补段：父亲那一段先补全，自己再把它接到后面
    for (int i = len - 1; i >= 0; --i) {
        int node = pathBuf[i];
        int p = dsuParent[node];
        if (p != r) { // 父亲不是根，说明它的段还没并进来过
            appendAfter(node, partLen[p], partRes[p]);
            dsuParent[node] = r;
        }
    }
    return r;
}

// 求点 x 当前的 h：先压缩，让 part[x] 正好是 x 到根的那一整段
Value currentValue(int x) {
    int r = findRoot(x);
    Value out;
    if (x == r) {
        out.len = partLen[x];
        out.res = partRes[x];
    } else {
        out.len = partLen[x] + partLen[r];
        out.res = (int)(((long long)pow10[partLen[r]] * partRes[x] + partRes[r]) % MOD);
    }
    return out;
}

int main() {
    // 10 的幂预先算好，拼接两段序列时是 O(1)
    pow10[0] = 1;
    for (int i = 1; i < MAXV; ++i) pow10[i] = (int)(10LL * pow10[i - 1] % MOD);
    for (int v = 0; v < MAXV; ++v) dsuParent[v] = v;

    int n, m;
    scanf("%d%d", &n, &m);

    int nextVtx = n; // 每合并一次就新建一个虚点，作为合并后分量的根
    for (int e = 1; e <= m; ++e) {
        int a, b;
        scanf("%d%d", &a, &b);
        --a;
        --b;
        int ra = findRoot(a), rb = findRoot(b);
        if (ra == rb) continue; // 只有 Kruskal 会选中的边才有用
        Value ha = currentValue(a), hb = currentValue(b);
        // 合并后：a 所在分量里每个点的序列接上 [e] 再紧跟 h_b，b 一侧对称。
        // 这两段挂在两个旧根上，等路径压缩走到旧根时自动加给分量里的每个点。
        int segLenA = 1 + hb.len;
        int segResA = (int)((pow10[hb.len] * (long long)e + hb.res) % MOD);
        int segLenB = 1 + ha.len;
        int segResB = (int)((pow10[ha.len] * (long long)e + ha.res) % MOD);
        appendAfter(ra, segLenA, segResA);
        appendAfter(rb, segLenB, segResB);
        dsuParent[ra] = nextVtx;
        dsuParent[rb] = nextVtx;
        ++nextVtx;
    }

    for (int v = 0; v < n; ++v) printf("%d\n", currentValue(v).res);
    return 0;
}
