#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 200005;

// 反向边：存储 “某条 a -> b 的反向信息”，用于从终点 b 回推起点 a
struct BackEdge {
    int from;
    long long label;
};

// 候选走法：从当前点先走一条边，再走那个后继点的最佳路径
struct Choice {
    int negLen;         // 后继点最长行程长度的相反数，统一成“越小越好”
    long long label;    // 这条边的标签
    int rank;           // 后继点最佳路径在本层中的排名
    long long sum;      // 这条边标签 + 后继点最佳路径的标签和
};

vector<BackEdge> backEdge[MAXN];
vector<Choice> choice[MAXN];

int outDegree[MAXN];    // 还有多少个后继点没被处理
int rankOf[MAXN];       // 同层（行程长度相同）之间的最佳路径排名
int tripLen[MAXN];
long long tripSum[MAXN];

// 先比行程长度（越长越好），再比第一条边标签，最后比后继路径的排名
bool choiceLess(const Choice &p, const Choice &q) {
    if (p.negLen != q.negLen) {
        return p.negLen < q.negLen;
    }
    if (p.label != q.label) {
        return p.label < q.label;
    }
    return p.rank < q.rank;
}

// 本层结点内部比较：先比第一条边标签，再比后继路径排名
struct RankKey {
    long long label;
    int rank;
    int node;
};

bool rankKeyLess(const RankKey &p, const RankKey &q) {
    if (p.label != q.label) {
        return p.label < q.label;
    }
    if (p.rank != q.rank) {
        return p.rank < q.rank;
    }
    return p.node < q.node;
}

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int e = 0; e < m; ++e) {
        int a, b;
        long long l;
        scanf("%d%d%lld", &a, &b, &l);
        backEdge[b].push_back(BackEdge{a, l});
        ++outDegree[a];
    }

    // 出度为 0 的城镇行程长度为 0，它们没有任何后继，可以最先定下来
    vector<int> layer;
    for (int i = 1; i <= n; ++i) {
        if (outDegree[i] == 0) {
            layer.push_back(i);
            rankOf[i] = 0;
        }
    }

    // 每一层内所有城镇的行程长度相同；层的顺序就是行程长度递增的顺序，
    // 因此处理第 k 层时，所有行程更长的后继都还没轮到，不会被误用
    while (!layer.empty()) {
        vector<int> nextLayer;
        for (int idx = 0; idx < (int)layer.size(); ++idx) {
            int to = layer[idx];
            for (int t = 0; t < (int)backEdge[to].size(); ++t) {
                int from = backEdge[to][t].from;
                // 候选记录里直接带上后继的最佳路径信息，稍后只需比较这三项
                Choice c;
                c.negLen = -tripLen[to];
                c.label = backEdge[to][t].label;
                c.rank = rankOf[to];
                c.sum = tripSum[to] + backEdge[to][t].label;
                choice[from].push_back(c);
                if (--outDegree[from] == 0) {
                    // 所有后继都已处理完；最后处理的这一层行程最长，
                    // 所以 from 的最长行程就是它加 1
                    tripLen[from] = tripLen[to] + 1;
                    nextLayer.push_back(from);
                }
            }
        }

        // 本层每个城镇挑出最佳走法
        vector<RankKey> keys;
        for (int idx = 0; idx < (int)nextLayer.size(); ++idx) {
            int node = nextLayer[idx];
            sort(choice[node].begin(), choice[node].end(), choiceLess);
            tripSum[node] = choice[node][0].sum;
            RankKey key;
            key.label = choice[node][0].label;
            key.rank = choice[node][0].rank;
            key.node = node;
            keys.push_back(key);
        }

        // 给本层排名：排名越小，最佳路径的标签序列字典序越小。
        // 由于本层所有城镇行程长度相同，只比较 (第一条边标签, 后继排名) 就够；
        // 字典序完全相同的两个城镇排在谁前谁后都不影响答案。
        sort(keys.begin(), keys.end(), rankKeyLess);
        for (int idx = 0; idx < (int)keys.size(); ++idx) {
            rankOf[keys[idx].node] = idx;
        }

        layer = nextLayer;
    }

    for (int i = 1; i <= n; ++i) {
        printf("%d %lld\n", tripLen[i], tripSum[i]);
    }
    return 0;
}
