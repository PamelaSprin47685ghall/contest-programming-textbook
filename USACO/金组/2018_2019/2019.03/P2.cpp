#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 7505;
const long long MOD = 2019201997LL;   // 题目给出的模数

int n, k;
bool used[MAXN];
long long dist[MAXN];     // 每个点连到当前已建成的树的最小边权
long long picked[MAXN];   // 每个点被并入生成树时所用的那条边权(树根用 MOD 占位)

// 奶牛 x 与奶牛 y (x < y) 愿意走的英里数
long long willing(int x, int y) {
    return (2019201913LL * x + 2019201949LL * y) % MOD;
}

int main() {
    scanf("%d %d", &n, &k);
    for (int i = 1; i <= n; i++) dist[i] = MOD;

    // Jarnik(Prim) 算法求最小生成树, 邻接矩阵隐含在 willing() 里, 不需要真的存图
    for (int iter = 0; iter < n; iter++) {
        int pick = 0;
        for (int i = 1; i <= n; i++)
            if (!used[i] && (pick == 0 || dist[i] < dist[pick])) pick = i;
        used[pick] = true;
        picked[iter] = dist[pick];
        for (int j = 1; j <= n; j++) {
            if (used[j]) continue;
            long long w = (j < pick) ? willing(j, pick) : willing(pick, j);
            if (w < dist[j]) dist[j] = w;
        }
    }

    // 按 Kruskal 顺序加边, 当连通块数量正好降到 K 个时, 下一条要加的边就是各组之间
    // 的最小距离, 因此答案 = 生成树中第 K-1 大的边权 = picked 中第 K 大的值
    sort(picked, picked + n);
    printf("%lld\n", picked[n - k]);
    return 0;
}
