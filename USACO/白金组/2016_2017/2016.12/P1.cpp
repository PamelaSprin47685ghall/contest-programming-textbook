#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 300; // 树的棵数上限

struct Point {
    long long x;
    long long y;
};

Point tree[MAXN];      // 每棵树的位置
int sorted[MAXN];      // 按 (x, y) 字典序排好的树的编号
int belowInSlab[MAXN][MAXN]; // belowInSlab[a][b]：横向位置夹在 a、b 之间，且在直线 a→b 下方的树的棵数
int aboveInSlab[MAXN][MAXN]; // 同上，但在直线 a→b 上方的棵数
long long lotCount[MAXN];    // lotCount[v]：内部恰好有 v 棵树的三角形土地数量

// 叉积 (ax, ay) × (bx, by)。正数说明从第一段转向第二段是逆时针
long long cross(long long ax, long long ay, long long bx, long long by) {
    return ax * by - ay * bx;
}

// 字典序：先比 x 再比 y。等价于沿方向 (1, 极小正数) 投影后排序，
// 这样任意两棵树都有一个不同的"横向位置"，后面谈"夹在中间"不会遇到并列
bool lexLess(int a, int b) {
    if (tree[a].x != tree[b].x) return tree[a].x < tree[b].x;
    return tree[a].y < tree[b].y;
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%lld%lld", &tree[i].x, &tree[i].y);
    }

    for (int i = 0; i < n; ++i) sorted[i] = i;
    sort(sorted, sorted + n, lexLess);

    // 预处理每条有向直线：数出横向位置严格夹在两端点之间、且落在直线下方的点。
    // 后面每个三角形只要沿它的三条边做一次加加减减就能得到内部点数，
    // 这样把"逐三角形再扫一遍所有点"的 O(n^4) 降成 O(n^3)。
    for (int ra = 0; ra < n; ++ra) {
        int a = sorted[ra];
        for (int rb = ra + 1; rb < n; ++rb) {
            int b = sorted[rb];
            for (int rc = ra + 1; rc < rb; ++rc) {
                int c = sorted[rc];
                long long side = cross(tree[b].x - tree[a].x, tree[b].y - tree[a].y,
                                       tree[c].x - tree[a].x, tree[c].y - tree[a].y);
                if (side < 0) {
                    belowInSlab[a][b]++;
                } else {
                    aboveInSlab[a][b]++;
                }
            }
        }
    }

    // 枚举三角形：按字典序取 first、middle、last 三棵树。
    // 若 middle 在底边 first→last 上方，三角形内部 = 左半段折线下方 + 右半段折线下方 - 底边下方：
    // 对横向位置在底边区间内的每个点，"落在折线下方"减去"落在底边下方"恰好等于"夹在上下边界之间"；
    // middle 不在底边上时，两条斜线各自都盖过底边，所以按坐标直接加减就能消掉区间外的点。
    // middle 在底边下方时上下颠倒，改用上方计数，道理相同。
    for (int ra = 0; ra < n; ++ra) {
        int first = sorted[ra];
        for (int rb = ra + 1; rb < n; ++rb) {
            int middle = sorted[rb];
            for (int rc = rb + 1; rc < n; ++rc) {
                int last = sorted[rc];
                long long sideOfMiddle = cross(tree[last].x - tree[first].x, tree[last].y - tree[first].y,
                                               tree[middle].x - tree[first].x, tree[middle].y - tree[first].y);
                long long inside;
                if (sideOfMiddle > 0) {
                    inside = belowInSlab[first][middle] + belowInSlab[middle][last]
                             - belowInSlab[first][last];
                } else {
                    inside = aboveInSlab[first][middle] + aboveInSlab[middle][last]
                             - aboveInSlab[first][last];
                }
                lotCount[inside]++;
            }
        }
    }

    for (int v = 0; v <= n - 3; ++v) {
        printf("%lld\n", lotCount[v]);
    }
    return 0;
}
