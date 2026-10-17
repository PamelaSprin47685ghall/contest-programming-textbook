#include <cstdio>
#include <utility>
#include <vector>

using namespace std;

const int MAXN = 1500; // 草地数上限
const int MAXY = 2500; // 长度下界 Y 的上限，也是 DP 表的大小
const int MOD = 1000000007;

struct State { // 一个"封顶总长度"下的汇总量
    int cnt;   // 方案数（模 MOD）
    int sum;   // 这些方案的真实总长度之和（模 MOD）
};

int N, M, X, Y, K;
vector<pair<int, int> > adj[MAXN + 1]; // 邻接表：{邻点, 路长}
bool visited[MAXN + 1];

int compNodes[MAXN + 1]; // 当前农场的结点列表
int compSize;
vector<int> dists; // 当前农场内全部无序点对的距离

// 按"封顶长度" min(距离, Y) 分组后的 (条数, 长度和)，同样存模 MOD
int groupCnt[MAXY + 1], groupSum[MAXY + 1];
bool groupUsed[MAXY + 1];
vector<int> groupKeys;

// DP：下标是封顶后的总长度，res[Y] 表示"总长度 >= Y"
State res[MAXY + 1], tmp[MAXY + 1];
int seenToken[MAXY + 1]; // 与 token 配合，给 tmp[·] 做惰性清零
vector<int> reachable, nxtReach;
int token;

// 找出 u 所在的整棵农场（连通块）
void dfsCollect(int u) {
    visited[u] = true;
    compNodes[compSize++] = u;
    for (int i = 0; i < (int)adj[u].size(); ++i) {
        int v = adj[u][i].first;
        if (!visited[v]) dfsCollect(v);
    }
}

// 从 ori 出发遍历整棵树，只记录终点编号大于 ori 的点对，
// 于是每个无序点对恰好被记录一次（树内两点路径唯一，距离即 d）。
void dfsDist(int u, int parent, int ori, int d) {
    for (int i = 0; i < (int)adj[u].size(); ++i) {
        int v = adj[u][i].first;
        int w = adj[u][i].second;
        if (v == parent) continue;
        if (ori < v) dists.push_back(d + w);
        dfsDist(v, u, ori, d + w);
    }
}

int main() {
    scanf("%d %d %d %d", &N, &M, &X, &Y);
    for (int i = 0; i < M; ++i) {
        int a, b, d;
        scanf("%d %d %d", &a, &b, &d);
        adj[a].push_back(make_pair(b, d));
        adj[b].push_back(make_pair(a, d));
    }
    K = N - M; // 森林里树的棵数就是农场数

    // 先把连接 K 个农场的 K 条道路（每条长 X）计入总长度。
    // 下标一旦达到 Y 就压到 Y：之后再拼上任何路径，下标都停在 Y，
    // 而 sum 里记的仍是被压缩前的真实长度，所以封顶不丢任何后续需要的信息。
    int start = (K * X < Y) ? K * X : Y;
    res[start].cnt = 1;
    res[start].sum = K * X % MOD;
    reachable.push_back(start);

    for (int root = 1; root <= N; ++root) {
        if (visited[root]) continue;

        compSize = 0;
        dfsCollect(root);

        // 农场内部两点路径唯一，从每个点各 DFS 一遍就拿到全部点对距离：
        // 一棵大小为 s 的树共 s(s-1)/2 个点对，所有农场加起来是 O(N^2)。
        dists.clear();
        for (int i = 0; i < compSize; ++i) dfsDist(compNodes[i], -1, compNodes[i], 0);

        // 按封顶长度分组：长度 >= Y 的路径全部并到关键字 Y。
        // 因为此后无论再拼上多长的路径，总和都已经 >= Y，它们彼此等价；
        // 但等价只体现在"是否达标"上，长度和仍要按原值累加。
        groupKeys.clear();
        for (int i = 0; i < (int)dists.size(); ++i) {
            int d = dists[i];
            int key = (d < Y) ? d : Y;
            if (!groupUsed[key]) {
                groupUsed[key] = true;
                groupCnt[key] = 0;
                groupSum[key] = 0;
                groupKeys.push_back(key);
            }
            groupCnt[key] = (groupCnt[key] + 1) % MOD;
            groupSum[key] = (groupSum[key] + d) % MOD;
        }

        // 合并这一棵树：先前的 res[j] 表示"封顶总长度为 j"的 cnt 种选法、
        // 长度和为 sum；本树提供 groupCnt 条总长 groupSum 的路径。
        // 条数相乘；长度和是所有配对的 (a+b) 之和，等于 sumA*cntB + cntA*sumB。
        ++token;
        nxtReach.clear();
        for (int gi = 0; gi < (int)groupKeys.size(); ++gi) {
            int key = groupKeys[gi];
            int gCnt = groupCnt[key], gSum = groupSum[key];
            for (int ri = 0; ri < (int)reachable.size(); ++ri) {
                int j = reachable[ri];
                int j2 = (j + key < Y) ? j + key : Y;
                if (seenToken[j2] != token) { // 本轮第一次写到 j2，先清空
                    seenToken[j2] = token;
                    tmp[j2].cnt = 0;
                    tmp[j2].sum = 0;
                    nxtReach.push_back(j2);
                }
                tmp[j2].sum = (tmp[j2].sum + (long long)res[j].sum * gCnt +
                               (long long)res[j].cnt * gSum) % MOD;
                tmp[j2].cnt = (tmp[j2].cnt + (long long)res[j].cnt * gCnt) % MOD;
            }
        }
        for (int i = 0; i < (int)nxtReach.size(); ++i) {
            int j = nxtReach[i];
            res[j] = tmp[j];
        }
        reachable.swap(nxtReach); // 只有这些下标是"活的"，其余位置不必再看
        for (int gi = 0; gi < (int)groupKeys.size(); ++gi) groupUsed[groupKeys[gi]] = false;
    }

    // 若没有任何选法的总长度能达到 Y，本轮的 reachable 里就没有下标 Y，
    // 那里留着旧值，必须按 0 处理。
    if (seenToken[Y] != token) res[Y].sum = 0;

    // 一组 K 条路径配上 2K 个"缺省连接点"共 K! * 2^K 种带标号的排法；
    // 每个真实赛道会被数 2K 次（K 个起点 × 2 个方向），
    // 所以赛道数正好是 K! * 2^K / (2K) = (K-1)! * 2^(K-1)。
    long long trackCount = 1;
    for (int i = 1; i <= K - 1; ++i) trackCount = trackCount * i % MOD;
    for (int i = 0; i < K - 1; ++i) trackCount = trackCount * 2 % MOD;

    long long answer = trackCount * res[Y].sum % MOD;
    printf("%lld\n", answer);
    return 0;
}
