#include <cstdio>

using namespace std;

const int MAXN = 105;             // 节点数上限
const long long MOD = 1000000007;

int n;
char typeStr[MAXN + 5];           // 一行节点类型：S / R / .
bool isSender[MAXN], isReceiver[MAXN];
bool edge[MAXN][MAXN];
int inDeg[MAXN], outDeg[MAXN], deg[MAXN];
int backStart[2], backEnd[2];     // 至多两条 i > j 的"回边"

long long factorial[MAXN];

long long powMod(long long base, long long exp) {
    long long result = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp & 1) result = result * base % MOD;
        base = base * base % MOD;
        exp >>= 1;
    }
    return result;
}

// 返回 sum over 从 en 沿正向边走到 st 的路径，路径上每个顶点（含首尾）的度数倒数之积。
// 含义：把每个顶点的 in/out 边两两配对时，强制让这条路径与回边 st->en 组成一个环，
// 环上每个顶点只剩 (deg-1)! 种配法，于是方案数正好是所有配对数的 prod(1/deg(v)) 倍。
long long pathSum(int st, int en) {
    if (st == 0 || en == 0 || en > st) return 0;
    long long dp[MAXN];
    for (int i = 0; i <= n; ++i) dp[i] = 0;
    dp[en] = 1;
    for (int i = en; i <= st; ++i) {
        if (deg[i] > 0) dp[i] = dp[i] * powMod(deg[i], MOD - 2) % MOD;
        for (int j = i + 1; j <= st; ++j)
            if (edge[i][j]) dp[j] = (dp[j] + dp[i]) % MOD;
    }
    return dp[st];
}

void solve() {
    int k;
    scanf("%d%d", &n, &k);
    scanf("%s", typeStr);
    for (int i = 1; i <= n; ++i) {
        isSender[i] = (typeStr[i - 1] == 'S');
        isReceiver[i] = (typeStr[i - 1] == 'R');
        inDeg[i] = outDeg[i] = 0;
        for (int j = 1; j <= n; ++j) edge[i][j] = false;
    }

    int backCnt = 0;
    for (int i = 1; i <= n; ++i) {
        scanf("%s", typeStr);
        for (int j = 1; j <= n; ++j) {
            edge[i][j] = (typeStr[j - 1] == '1');
            if (edge[i][j]) {
                ++outDeg[i];
                ++inDeg[j];
                if (i > j) {  // 记录回边，题目保证最多两条
                    backStart[backCnt] = i;
                    backEnd[backCnt] = j;
                    ++backCnt;
                }
            }
        }
    }
    // 补成两条"空"回边，这样 K = 0 / 1 也能套用同一个容斥公式（空边的路径和为 0）
    while (backCnt < 2) {
        backStart[backCnt] = 0;
        backEnd[backCnt] = 0;
        ++backCnt;
    }

    // 加上虚拟起点到每个 sender、每个 receiver 到虚拟终点的边之后，
    // 每个点的入度都等于出度，记为 deg[i]
    for (int i = 1; i <= n; ++i) deg[i] = inDeg[i] + (isSender[i] ? 1 : 0);

    factorial[0] = 1;
    for (int i = 1; i <= n; ++i) factorial[i] = factorial[i - 1] * i % MOD;
    long long prodDeg = 1;
    for (int i = 1; i <= n; ++i) prodDeg = prodDeg * factorial[deg[i]] % MOD;

    // 所有正向边都从小标号指向大标号，所以环必须经过回边。
    // 四条路径和：两条"回边自成环"，两条交叉（一条环串起两条回边）。
    long long self0 = pathSum(backStart[0], backEnd[0]);
    long long self1 = pathSum(backStart[1], backEnd[1]);
    long long cross01 = pathSum(backStart[0], backEnd[1]);
    long long cross10 = pathSum(backStart[1], backEnd[0]);

    // 容斥：1 - 环0 - 环1 + 环0环1（两个环同时出现）- 只用两条回边串成一个大环
    long long valid = 1;
    valid = (valid - self0 + MOD) % MOD;
    valid = (valid - self1 + MOD) % MOD;
    valid = (valid + self0 * self1) % MOD;
    valid = (valid - cross01 * cross10 % MOD + MOD) % MOD;

    printf("%lld\n", valid * prodDeg % MOD);
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) solve();
    return 0;
}
