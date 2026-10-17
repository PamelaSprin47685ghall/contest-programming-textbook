// USACO 2020 February Contest, Silver P1: Swapity Swapity Swap
#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100000;
const int MAXM = 100;

int n, m;
long long k;
int leftPos[MAXM], rightPos[MAXM];
int perm[MAXN + 1];      // perm[p]：执行一次完整流程后，停在位置 p 的奶牛编号
int answer[MAXN + 1];
bool visited[MAXN + 1];
int cycle[MAXN + 1];     // 当前环上的位置，按 perm 的前进方向排列

int main() {
    scanf("%d %d %lld", &n, &m, &k);
    for (int i = 0; i < m; ++i) {
        scanf("%d %d", &leftPos[i], &rightPos[i]);
    }

    // 一整轮流程只是把每个位置上的奶牛搬到固定的另一个位置，
    // 所以直接把这一轮流程作用在数组 [1,2,...,n] 上，
    // 得到的 perm[p] 就是"跑完一轮后停在位置 p 的奶牛编号"。
    for (int p = 1; p <= n; ++p) {
        perm[p] = p;
    }
    for (int i = 0; i < m; ++i) {
        for (int a = leftPos[i], b = rightPos[i]; a < b; ++a, --b) {
            swap(perm[a], perm[b]);
        }
    }

    // 每跑一轮，位置 p 上的编号就变成 perm[p]，于是 K 轮之后
    // 位置 p 上是 perm^K(p)。K 最大 10^9，不能逐轮模拟，
    // 但置换一定分解成若干互不相交的环，环内走 K 步等价于走 K % 环长 步。
    for (int start = 1; start <= n; ++start) {
        if (visited[start]) continue;
        int len = 0;
        int cur = start;
        while (!visited[cur]) {
            visited[cur] = true;
            cycle[len++] = cur;
            cur = perm[cur];
        }
        // cycle[t] 就是 perm^t(start)，所以 perm^K(cycle[t]) = cycle[(t + K) % len]
        for (int t = 0; t < len; ++t) {
            answer[cycle[t]] = cycle[(int)((t + k) % len)];
        }
    }

    for (int p = 1; p <= n; ++p) {
        printf("%d\n", answer[p]);
    }
    return 0;
}
