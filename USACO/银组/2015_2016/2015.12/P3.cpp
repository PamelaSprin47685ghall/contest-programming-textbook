#include <cstdio>

using namespace std;

// 前缀和数组：prefix[k][i] 表示前 i 头牛中品种 k 的头数。
// 3 个品种共用 4 行，行号 0 留空方便按品种编号直接索引。
const int MAXN = 100005;

int N, Q;
int prefix[4][MAXN];

int main() {
    scanf("%d%d", &N, &Q);
    for (int i = 1; i <= N; ++i) {
        int breed;
        scanf("%d", &breed);
        // 先把 i-1 的计数整体搬到 i，再给当前品种加一
        for (int k = 1; k <= 3; ++k) prefix[k][i] = prefix[k][i - 1];
        prefix[breed][i] += 1;
    }

    // 区间 [a,b] 的答案 = 前 b 头的数量减去前 a-1 头的数量
    for (int i = 0; i < Q; ++i) {
        int a, b;
        scanf("%d%d", &a, &b);
        printf("%d %d %d\n",
               prefix[1][b] - prefix[1][a - 1],
               prefix[2][b] - prefix[2][a - 1],
               prefix[3][b] - prefix[3][a - 1]);
    }
    return 0;
}
