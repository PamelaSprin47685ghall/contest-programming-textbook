#include <cstdio>
#include <vector>

using namespace std;

const int MOD = 1000000007;
const int MAXK = 50000;    // 图的张数上限
const int MAXN2 = 200000;  // 单张图翻倍后的点数上限：2·N_i，且所有 N_i 之和不超过 1e5
const int MAXD = 200002;   // 最短距离的上界，也是因子数组的长度

int graphCount;        // 图的张数 K
int nodeCount[MAXK];   // 每张图的点数 N_i

// 把每张图复制成两份（"偶数份"和"奇数份"）后合并成的邻接表：
// 从偶数份沿一条边走到奇数份，表示在原图里多走了一步，于是点的份号就是步数的奇偶性
vector<int> adjacency[MAXN2];
int bfsDistance[MAXN2];
int bfsQueue[MAXN2];

// 每张图对三种统计口径各自贡献一组"最短距离"
vector<int> evenValues[MAXK];  // 能通过偶数步走到时，最短的偶数步数
vector<int> oddValues[MAXK];   // 能通过奇数步走到时，最短的奇数步数
vector<int> bothValues[MAXK];  // 两种步数都能走到时取较大者，用来扣掉重复统计的部分

long long smallFactor[MAXD];  // 距离 x 上，"2·N_i > x 的那批图"贡献的因子
long long largeFactor[MAXD];  // 每张图把"距离超过上界后不再变化的因子"记在 2·N_i 位上，再做前缀积
int valueCount[MAXD];         // 当前这张图里，最短距离恰好等于 x 的点数

// 求所有合法元组"最大值之和"：
// 记 less[x] = 最大值不超过 x 的元组数，则所求为 Σ x·(less[x] − less[x−1])
long long sumOfMaxima(vector<int>* lists) {
    for (int x = 0; x < MAXD; ++x) {
        smallFactor[x] = 1;
        largeFactor[x] = 1;
    }
    int largestDistance = 0;
    for (int i = 0; i < graphCount; ++i) {
        int doubled = 2 * nodeCount[i];
        if (doubled > largestDistance) largestDistance = doubled;
        for (int x = 0; x < doubled; ++x) valueCount[x] = 0;
        for (int k = 0; k < (int)lists[i].size(); ++k) valueCount[lists[i][k]] += 1;
        // 累加成"距离不超过 x 的点数"，也就是这张图在 x 处的计数因子
        for (int x = 1; x < doubled; ++x) valueCount[x] += valueCount[x - 1];
        for (int x = 0; x < doubled; ++x) {
            smallFactor[x] = smallFactor[x] * valueCount[x] % MOD;
        }
        // 距离一旦到 doubled−1 就把这张图的点全部数完，更大的 x 因子不再变化；
        // 把这份固定的因子记为 x = doubled 处的后缀因子，就省掉了逐位相乘
        largeFactor[doubled] = largeFactor[doubled] * valueCount[doubled - 1] % MOD;
    }
    for (int x = 1; x < MAXD; ++x) {
        largeFactor[x] = largeFactor[x] * largeFactor[x - 1] % MOD;
    }
    long long result = 0;
    long long previous = smallFactor[0] * largeFactor[0] % MOD;
    for (int x = 1; x <= largestDistance; ++x) {
        long long current = smallFactor[x] * largeFactor[x] % MOD;
        long long tuples = (current - previous + MOD) % MOD;  // 最大值恰好为 x 的元组数
        result = (result + tuples * x) % MOD;
        previous = current;
    }
    return result;
}

int main() {
    scanf("%d", &graphCount);
    for (int i = 0; i < graphCount; ++i) {
        int n, m;
        scanf("%d%d", &n, &m);
        nodeCount[i] = n;
        int doubled = 2 * n;
        for (int v = 0; v < doubled; ++v) adjacency[v].clear();
        for (int e = 0; e < m; ++e) {
            int a, b;
            scanf("%d%d", &a, &b);
            --a;
            --b;
            // 每条边都要连"偶数份→奇数份"和"奇数份→偶数份"两个方向
            adjacency[a].push_back(n + b);
            adjacency[b].push_back(n + a);
            adjacency[n + a].push_back(b);
            adjacency[n + b].push_back(a);
        }
        for (int v = 0; v < doubled; ++v) bfsDistance[v] = -1;
        bfsDistance[0] = 0;
        int head = 0, tail = 0;
        bfsQueue[tail++] = 0;
        while (head < tail) {
            int current = bfsQueue[head++];
            for (int k = 0; k < (int)adjacency[current].size(); ++k) {
                int next = adjacency[current][k];
                if (bfsDistance[next] == -1) {
                    bfsDistance[next] = bfsDistance[current] + 1;
                    bfsQueue[tail++] = next;
                }
            }
        }
        evenValues[i].clear();
        oddValues[i].clear();
        bothValues[i].clear();
        for (int v = 0; v < n; ++v) {
            // 偶数份里的距离就是最短偶数步数，奇数份里的距离就是最短奇数步数
            if (bfsDistance[v] != -1) evenValues[i].push_back(bfsDistance[v]);
            if (bfsDistance[n + v] != -1) oddValues[i].push_back(bfsDistance[n + v]);
            if (bfsDistance[v] != -1 && bfsDistance[n + v] != -1) {
                int longer = bfsDistance[v] > bfsDistance[n + v] ? bfsDistance[v] : bfsDistance[n + v];
                bothValues[i].push_back(longer);
            }
        }
    }
    // 先假设每个元组都按偶数步数计一次、按奇数步数再计一次；
    // 两种步数都能用的元组被多算了一次"较大的那个"，正好用 both 口径减掉
    long long answer = (sumOfMaxima(evenValues) + sumOfMaxima(oddValues)) % MOD;
    answer = (answer - sumOfMaxima(bothValues) + MOD) % MOD;
    printf("%lld\n", answer);
    return 0;
}
