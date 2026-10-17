#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 200005;

int N, T;
vector<int> children[MAXN];
int subSize[MAXN];             // 子树大小
int maxDepth[MAXN];            // 子树内以 u 为端点的最长向下路径（边数）
long long rateSum[MAXN];       // 子树内生长速率之和（结点 1 自身的速率无关紧要）
long long dp0[MAXN];           // 从 u 出发、遍历完子树并回到 u 的最小肥料量
long long dp1[MAXN];           // 从 u 出发、遍历完子树并停在子树最深点的最小肥料量
long long preSize[MAXN];       // 排序后排在每个儿子之前的兄弟子树大小之和
long long suffixRate[MAXN + 1];

// 交换论证：把 b 排在 c 前面当且仅当 subSize[b] / rateSum[b] <= subSize[c] / rateSum[c]，
// 即（交叉相乘避免除法）subSize[b] * rateSum[c] <= subSize[c] * rateSum[b]
bool cmpChild(int b, int c) {
    return subSize[b] * rateSum[c] < subSize[c] * rateSum[b];
}

int main() {
    scanf("%d %d", &N, &T);
    for (int i = 2; i <= N; i++) {
        int p;
        long long a;
        scanf("%d %lld", &p, &a);
        children[p].push_back(i);
        rateSum[i] = a;
    }

    // 保证 p_i < i，所以儿子编号总比父亲大，从大到小遍历即是自底向上
    for (int u = N; u >= 1; u--) {
        subSize[u] = 1;
        for (int i = 0; i < (int)children[u].size(); i++) {
            int v = children[u][i];
            subSize[u] += subSize[v];
            rateSum[u] += rateSum[v];
            if (maxDepth[v] + 1 > maxDepth[u]) maxDepth[u] = maxDepth[v] + 1;
        }
        sort(children[u].begin(), children[u].end(), cmpChild);

        int k = (int)children[u].size();
        long long prefixSize = 0;
        for (int i = 0; i < k; i++) {
            preSize[i] = prefixSize;
            prefixSize += subSize[children[u][i]];
        }
        suffixRate[k] = 0;
        for (int i = k - 1; i >= 0; i--) {
            suffixRate[i] = suffixRate[i + 1] + rateSum[children[u][i]];
        }

        // 第 i 个儿子 v 的贡献为 dp0[v] + rateSum[v] * (1 + 2 * preSize[i])：
        // 访问它的子树要花 2 * subSize[v] 秒，其中每一秒 v 子树里的草都在长，
        // 而 v 子树访问结束的位置正好是各兄弟之间的分界点
        dp0[u] = 0;
        for (int i = 0; i < k; i++) {
            int v = children[u][i];
            dp0[u] += dp0[v] + rateSum[v] * (1 + 2 * preSize[i]);
        }

        // T = 1 时要停在小树里最深的位置，于是最后访问的儿子必须是深度最大的儿子之一。
        // 把它从原位置挪到最后：它之前的儿子贡献不变，它之后的儿子因为少等 2 * subSize[v] 秒
        // 而各自少 rateSum * 2 * subSize[v]，它自己从「回到自身」变成「停在最深点」
        dp1[u] = 0;
        if (k > 0) {
            long long best = -1;
            for (int i = 0; i < k; i++) {
                int v = children[u][i];
                if (maxDepth[v] + 1 != maxDepth[u]) continue;
                long long cand = dp0[u] - dp0[v] - rateSum[v] * (1 + 2 * preSize[i])
                               + dp1[v] + rateSum[v] * (1 + 2 * (prefixSize - subSize[v]))
                               - 2 * subSize[v] * suffixRate[i + 1];
                if (best < 0 || cand < best) best = cand;
            }
            dp1[u] = best;
        }
    }

    if (T == 0) {
        printf("%d %lld\n", 2 * (N - 1), dp0[1]);
    } else {
        printf("%d %lld\n", 2 * (N - 1) - maxDepth[1], dp1[1]);
    }
    return 0;
}
