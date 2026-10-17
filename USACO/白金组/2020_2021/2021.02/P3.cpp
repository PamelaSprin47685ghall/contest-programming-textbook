#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 320;             // 顶点数上限（题目保证 N <= 100，这里留出余量）
const int MAXSIDE = 2 * MAXN + 5; // 两种最短长度都不会超过 2N
const int MOD = 1000000007;
const int INF = 1000000000;       // 表示"走不到"

int n, m;
vector<int> adjacency[MAXN];

// distEven[v]：从 1 号顶点出发、长度为偶数并到达 v 的最短行走长度
// distOdd[v] ：从 1 号顶点出发、长度为奇数并到达 v 的最短行走长度
int distEven[MAXN];
int distOdd[MAXN];

int stateVertex[2 * MAXN];  // 分层 BFS 的队列，一个状态 = (顶点, 长度奇偶)
int stateParity[2 * MAXN];

// layerCount[a][b] = 集合 s(a, b) 的大小，即 h(i) = (a, b) 的顶点个数
int layerCount[MAXSIDE][MAXSIDE];
int vertexA[MAXN];  // 每个顶点的 h(i) 的较小坐标
int vertexB[MAXN];  // 每个顶点的 h(i) 的较大坐标

long long binomial[MAXN + 5][MAXN + 5];           // binomial[x][y] = (x 选 y)
long long powerOfTwo[MAXN * (MAXN + 1) / 2 + 5];  // 2 的幂

long long powMod(long long base, long long exponent) {
    long long result = 1;
    base %= MOD;
    while (exponent > 0) {
        if (exponent & 1) result = result * base % MOD;
        base = base * base % MOD;
        exponent >>= 1;
    }
    return result;
}

// 在"顶点 + 长度奇偶"组成的状态图上 BFS：走一条边恰好翻转长度奇偶，
// 所以每个状态只需要入队一次，第一次入队时的长度就是它的最短长度
void bfsDistances() {
    for (int v = 0; v < n; ++v) {
        distEven[v] = INF;
        distOdd[v] = INF;
    }
    distEven[0] = 0;
    int head = 0, tail = 0;
    stateVertex[tail] = 0;
    stateParity[tail] = 0;
    ++tail;
    while (head < tail) {
        int v = stateVertex[head];
        int parity = stateParity[head];
        ++head;
        int reached = parity ? distOdd[v] : distEven[v];
        for (int i = 0; i < (int)adjacency[v].size(); ++i) {
            int u = adjacency[v][i];
            if (parity == 0) {
                if (distOdd[u] == INF) {
                    distOdd[u] = reached + 1;
                    stateVertex[tail] = u;
                    stateParity[tail] = 1;
                    ++tail;
                }
            } else {
                if (distEven[u] == INF) {
                    distEven[u] = reached + 1;
                    stateVertex[tail] = u;
                    stateParity[tail] = 0;
                    ++tail;
                }
            }
        }
    }
}

// 二分图：每个顶点只有一种奇偶长度的行走能到它，所以等价图里
// 距离为 d 的顶点必须连向距离 d - 1 的顶点（且至少连一条），别无选择
long long countBipartite() {
    int countAtDistance[MAXN];
    for (int d = 0; d < n; ++d) countAtDistance[d] = 0;
    for (int v = 0; v < n; ++v) {
        int distance = distEven[v] < distOdd[v] ? distEven[v] : distOdd[v];
        ++countAtDistance[distance];
    }
    long long answer = 1;
    for (int d = 1; d < n; ++d) {
        if (countAtDistance[d] == 0) break;
        // 上一层每个顶点都可以被本层顶点自由选择，但不能一个都不选
        long long choices = (powMod(2, countAtDistance[d - 1]) - 1 + MOD) % MOD;
        answer = answer * powMod(choices, countAtDistance[d]) % MOD;
    }
    return answer;
}

void fillLayerCount() {
    for (int v = 0; v < n; ++v) {
        int a = distEven[v] < distOdd[v] ? distEven[v] : distOdd[v];
        int b = distEven[v] < distOdd[v] ? distOdd[v] : distEven[v];
        vertexA[v] = a;
        vertexB[v] = b;
        ++layerCount[a][b];
    }
}

void clearLayerCount() {
    for (int v = 0; v < n; ++v) layerCount[vertexA[v]][vertexB[v]] = 0;
}

// 非二分图：按层（a + b 相同）从小到大、层内按 a 从小到大做 DP，
// 每层独立统计方案数
long long countNonBipartite() {
    fillLayerCount();
    long long answer = 1;
    for (int layer = 1; layer < 4 * n; layer += 2) {
        // dp[x]：已经处理完本层中 a 更小的组时，本组还有 x 个顶点没有边
        vector<long long> dp(layerCount[0][layer] + 1, 0);
        dp[layerCount[0][layer]] = 1;
        for (int a = 1; a <= layer / 2; ++a) {
            int b = layer - a;
            int size = layerCount[a][b];
            int upperSize = layerCount[a - 1][b + 1];
            // 第一步：在 s(a-1, b+1) 与 s(a, b) 之间连 type B 边。
            // middle[x] 表示本组还剩 x 个顶点一条边都没有的方案数
            vector<long long> middle(size + 1, 0);
            for (int x = 0; x <= size; ++x) {
                long long total = 0;
                for (int y = 0; y <= upperSize; ++y) {
                    long long ways = 0;
                    for (int k = 0; k <= y; ++k) {
                        // 容斥：上面那组 y 个"还需要边"的顶点必须都拿到边，
                        // 先减去假定其中 k 个拿不到的方案
                        long long usable = (powMod(2, upperSize - k) - 1 + MOD) % MOD;
                        long long term = binomial[y][k] * powMod(usable, size - x) % MOD;
                        if (k & 1) ways = (ways - term + MOD) % MOD;
                        else ways = (ways + term) % MOD;
                    }
                    total = (total + ways * dp[y]) % MOD;
                }
                middle[x] = total * binomial[size][x] % MOD;  // 选出具体的 x 个顶点
            }
            // 第二步：在 s(a-1, b-1) 与 s(a, b) 之间连 type A 边。
            // 上一步一条边都没拿到的顶点必须靠 type A 边补救，其余顶点可以自由连
            int lowerSize = layerCount[a - 1][b - 1];
            long long typeAChoices = (powMod(2, lowerSize) - 1 + MOD) % MOD;
            vector<long long> nextDp(size + 1, 0);
            for (int x = 0; x <= size; ++x) {
                long long total = 0;
                for (int y = 0; x + y <= size; ++y) {
                    long long term = binomial[size - y][x] * powMod(typeAChoices, size - x) % MOD;
                    total = (total + term * middle[y]) % MOD;
                }
                nextDp[x] = total;
            }
            dp.swap(nextDp);
        }
        // 最上面的组没有更小的 a 可用，剩下没满足的顶点只能靠 type C 边，
        // 也就是在本组内部连边（连自己即自环）；同样用容斥数出方案数
        int topSize = layerCount[layer / 2][layer / 2 + 1];
        long long layerAnswer = 0;
        for (int y = 0; y <= topSize; ++y) {
            long long ways = 0;
            for (int k = 0; k <= y; ++k) {
                int available = topSize - k;
                long long term = binomial[y][k] * powerOfTwo[available * (available + 1) / 2] % MOD;
                if (k & 1) ways = (ways - term + MOD) % MOD;
                else ways = (ways + term) % MOD;
            }
            layerAnswer = (layerAnswer + ways * dp[y]) % MOD;
        }
        answer = answer * layerAnswer % MOD;
    }
    clearLayerCount();
    return answer;
}

int main() {
    binomial[0][0] = 1;
    for (int x = 1; x <= MAXN; ++x) {
        binomial[x][0] = 1;
        for (int y = 1; y <= x; ++y) {
            binomial[x][y] = (binomial[x - 1][y] + binomial[x - 1][y - 1]) % MOD;
        }
    }
    powerOfTwo[0] = 1;
    int powerLimit = MAXN * (MAXN + 1) / 2;
    for (int i = 1; i <= powerLimit; ++i) powerOfTwo[i] = powerOfTwo[i - 1] * 2 % MOD;

    int testCount;
    scanf("%d", &testCount);
    for (int t = 0; t < testCount; ++t) {
        scanf("%d%d", &n, &m);
        for (int v = 0; v < n; ++v) adjacency[v].clear();
        for (int e = 0; e < m; ++e) {
            int x, y;
            scanf("%d%d", &x, &y);
            --x;
            --y;
            adjacency[x].push_back(y);
            adjacency[y].push_back(x);
        }
        bfsDistances();
        if (distOdd[0] == INF) printf("%lld\n", countBipartite());
        else printf("%lld\n", countNonBipartite());
    }
    return 0;
}
