#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 40005;
const int MAXM = 100005;
const int MOD = 1000000007;

struct Edge {
    int len;  // 长度
    int u;
    int v;
    bool operator<(const Edge& other) const {
        return len < other.len;
    }
};

Edge edges[MAXM];
int parent[MAXN];

// 主并查集的查找: 先找根, 再顺手把沿路的点都挂到根上
int find(int x) {
    int root = x;
    while (parent[root] != root) {
        root = parent[root];
    }
    while (parent[x] != root) {
        int next = parent[x];
        parent[x] = root;
        x = next;
    }
    return root;
}

// 局部小并查集的查找: 一次只处理至多 6 个分量, 不压缩路径也够快
int findLocal(int par[], int x) {
    while (par[x] != x) {
        x = par[x];
    }
    return x;
}

// 把分量代表元 r 登记到局部编号表 comp 里, 返回它的局部下标
int localIndex(int r, int comp[], int& compCnt) {
    for (int i = 0; i < compCnt; i++) {
        if (comp[i] == r) {
            return i;
        }
    }
    comp[compCnt] = r;
    return compCnt++;
}

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 0; i < m; i++) {
        scanf("%d%d%d", &edges[i].u, &edges[i].v, &edges[i].len);
    }
    sort(edges, edges + m);
    for (int i = 1; i <= n; i++) {
        parent[i] = i;
    }

    long long cost = 0;
    long long ways = 1;

    // Kruskal 按长度从小到大分组处理: 同一长度的边怎么选, 都不会影响
    // 下一组开始时图的分量划分, 所以每组的选择互相独立, 方案数可以相乘
    for (int i = 0; i < m;) {
        int j = i;
        while (j < m && edges[j].len == edges[i].len) {
            j++;
        }
        int num = j - i;  // 这个长度最多只有 3 条边

        // 把组内每条边的两个端点换成"当前分量"的局部编号
        int comp[6];
        int compCnt = 0;
        int locU[3], locV[3];
        for (int t = i; t < j; t++) {
            locU[t - i] = localIndex(find(edges[t].u), comp, compCnt);
            locV[t - i] = localIndex(find(edges[t].v), comp, compCnt);
        }

        // merges: 这组边最多能把分量合并多少次
        int par[6];
        for (int q = 0; q < compCnt; q++) {
            par[q] = q;
        }
        int merges = 0;
        for (int t = 0; t < num; t++) {
            int a = findLocal(par, locU[t]);
            int b = findLocal(par, locV[t]);
            if (a != b) {
                par[a] = b;
                merges++;
            }
        }

        // 组内至多 3 条边, 直接枚举所有子集: 无环且恰好用满 merges 条边
        // 的子集, 就是一棵覆盖该组所有合并的生成森林
        long long choices = 0;
        for (int mask = 0; mask < (1 << num); mask++) {
            int par2[6];
            for (int q = 0; q < compCnt; q++) {
                par2[q] = q;
            }
            bool acyclic = true;
            int used = 0;
            for (int t = 0; t < num; t++) {
                if (((mask >> t) & 1) == 0) {
                    continue;
                }
                int a = findLocal(par2, locU[t]);
                int b = findLocal(par2, locV[t]);
                if (a == b) {
                    acyclic = false;
                    break;
                }
                par2[a] = b;
                used++;
            }
            if (acyclic && used == merges) {
                choices++;
            }
        }
        ways = ways * choices % MOD;

        // 把这一组边真正并入主并查集, 顺带累计最小生成树的长度
        for (int t = i; t < j; t++) {
            int a = find(edges[t].u);
            int b = find(edges[t].v);
            if (a != b) {
                parent[a] = b;
                cost += edges[t].len;
            }
        }
        i = j;
    }

    printf("%lld %lld\n", cost, ways);
    return 0;
}
