#include <cstdio>

using namespace std;

// 顶点按 0..n-1 编号。a[i][j] 为改动边 (i,j) 的代价：
// a[i][j] > 0 表示这条边当前不存在，加边要花 a[i][j]；a[i][j] < 0 表示这条边存在，删边要花 -a[i][j]。
const int MAXN = 750;
const int INF = 1000000000;

int n;
int a[MAXN + 1][MAXN + 1];
// p[i][j] = 区间 [i,j] 内部所有"当前存在的边"的删除代价之和。
// 于是切断 [l,m-1] 与 [m,r] 之间所有边的代价 = p[l][r] - p[l][m-1] - p[m][r]（容斥）。
int p[MAXN + 1][MAXN + 1];
// dp[i][j] = 让区间 [i,j] 成为以 i 为根的合法 DFS 子树的最小代价（i 的若干孩子分别是若干连续段的段首）。
int dp[MAXN + 1][MAXN + 1];
// 上面两个数组的转置。内层循环固定 (i,j) 遍历 k 时，dp[k+1][j] 与 p[k+1][j] 是"跨列"访问，
// 转置后变成顺序访问同一行，避免 cache miss 拖慢 O(N^3) 主循环。
int pT[MAXN + 1][MAXN + 1];
int dpT[MAXN + 1][MAXN + 1];

int main() {
    scanf("%d", &n);
    // 输入第 j 行（j 从 1 开始）给出 a[0][j], a[1][j], ..., a[j-1][j]。
    for (int j = 1; j < n; ++j) {
        for (int i = 0; i < j; ++i) {
            scanf("%d", &a[i][j]);
            a[j][i] = a[i][j]; // 对称存一份，方便按行访问
        }
    }

    for (int i = n - 1; i >= 0; --i) {
        int *pi = p[i + 1];   // p[i+1][*]，常量列访问靠它
        int *pRow = p[i];
        int *ai = a[i];
        int *dpRow = dp[i];
        for (int j = i + 1; j < n; ++j) {
            // 二维前缀和：先把 (i,j) 这条边本身的删除代价并入。
            int own = ai[j] < 0 ? -ai[j] : 0;
            int cur = pi[j] + pRow[j - 1] - pi[j - 1] + own;
            pRow[j] = cur;
            pT[j][i] = cur;

            int *pTj = pT[j];     // pT[j][m] = p[m][j]
            int *dpTj = dpT[j];   // dpT[j][m] = dp[m][j]
            int base = pi[j];     // cut(i+1, k+1, j) = pi[j] - pi[k] - p[k+1][j]
            int best = INF;
            for (int k = i; k < j; ++k) {
                // k 是 i 的最后一个孩子段的最右端：左段 [i,k] 已经整理好，
                // 右段 [k+1,j] 作为新孩子挂到 i 上，必须加边 (i,k+1)（若已存在则免费），
                // 同时必须删掉 [i+1,k] 与 [k+1,j] 之间的所有边，否则 DFS 会提前钻进后面的段。
                int add = ai[k + 1] > 0 ? ai[k + 1] : 0;
                int cut = base - pi[k] - pTj[k + 1];
                int cand = dpRow[k] + dpTj[k + 1] + add + cut;
                if (cand < best) best = cand;
            }
            dpRow[j] = best;
            dpT[j][i] = best; // dp[j][j] 等空闲位置保持 0，正好表示单点子树代价为 0
        }
    }

    printf("%d\n", dp[0][n - 1]);
    return 0;
}
