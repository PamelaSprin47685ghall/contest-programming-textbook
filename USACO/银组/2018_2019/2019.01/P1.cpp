#include <cstdio>

using namespace std;

const int MAXN = 100000;

int n;
int degree[MAXN + 1];

int main() {
    scanf("%d", &n);
    for (int i = 1; i < n; ++i) {
        int a, b;
        scanf("%d%d", &a, &b);
        ++degree[a];
        ++degree[b];
    }

    // 度数最大的点 v 有 D 个邻居, v 与它们两两相邻或经由 v 相隔 2 条边,
    // 所以这 D+1 个点必须两两异色, 下界是 D+1.
    // 上界: 任取根染 1 号色, 每个非根点 u 已连向父节点, 故儿子数不超过 D-1,
    // 而可用颜色有 D-1 种 (去掉 u 与它父亲的颜), 于是逐层 BFS 总能给儿子们
    // 分配互不相同的颜色. 兄弟异色保证距离 2 的点同色冲突不会出现, 距离 2 的
    // 另一种情形是祖孙, 由"避开父节点颜色"排除. 所以答案是 D+1.
    int maxDegree = 0;
    for (int i = 1; i <= n; ++i) {
        if (degree[i] > maxDegree) {
            maxDegree = degree[i];
        }
    }
    printf("%d\n", maxDegree + 1);
    return 0;
}
