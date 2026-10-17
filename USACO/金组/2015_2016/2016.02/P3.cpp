#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXF = 2005;             // 栅栏条数上限 2000, 加两条边界后间隔数上限 2001
const int MAXC = 2001 * 2001;      // 压缩后网格里的格子数上限

int A, B, n, m;
int colW[MAXF];    // 压缩后每一列的宽度, 升序
int rowH[MAXF];    // 压缩后每一行的高度, 升序
int parent[MAXC];
int subSize[MAXC];                 // 只能维护根处的大小

int find(int x) {
    if (parent[x] == x) return x;
    return parent[x] = find(parent[x]);   // 路径压缩
}

// 返回 true 表示这次真的把两个格子连通了, 于是这条栅栏必须被拆掉
bool unite(int x, int y) {
    int rx = find(x), ry = find(y);
    if (rx == ry) return false;
    if (subSize[rx] < subSize[ry]) swap(rx, ry);   // 按大小合并, 保证树高
    parent[ry] = rx;
    subSize[rx] += subSize[ry];
    return true;
}

int main() {
    scanf("%d %d %d %d", &A, &B, &n, &m);

    int pos[MAXF];
    for (int i = 0; i < n; i++) {
        scanf("%d", &pos[i]);
    }
    sort(pos, pos + n);
    // 竖直栅栏把矩形切成 n+1 列; 这些栅栏位置两两之差(连同两端的边界)就是列宽
    int last = 0;
    for (int i = 0; i < n; i++) {
        colW[i] = pos[i] - last;
        last = pos[i];
    }
    colW[n] = A - last;
    sort(colW, colW + n + 1);

    for (int i = 0; i < m; i++) {
        scanf("%d", &pos[i]);
    }
    sort(pos, pos + m);
    last = 0;
    for (int i = 0; i < m; i++) {
        rowH[i] = pos[i] - last;
        last = pos[i];
    }
    rowH[m] = B - last;
    sort(rowH, rowH + m + 1);

    int cols = n + 1, rows = m + 1;   // 列数、行数
    int cells = cols * rows;          // 压缩后总格子数, 最多约 4e6
    for (int i = 0; i < cells; i++) {
        parent[i] = i;
        subSize[i] = 1;
    }

    // 把每个格子看成点, 相邻两格之间的边权就是"拆掉的那段栅栏长度":
    // 上下相邻的一对格子之间的栅栏属于某一列, 长度恰好是那一列的宽度,
    // 左右相邻的一对格子之间的栅栏属于某一行, 长度恰好是那一行的高度。
    // 于是问题就是最小生成树, 按边的权值从小到大用并查集加边即可;
    // 同一列或同一行里所有这样的边的权值都相同, 按"候选长度"从小到大成批处理就行。
    long long ans = 0;
    int need = cells - 1;                  // 生成树还需要的边数
    int ci = 0, ri = 0;
    while (need > 0 && (ci < cols || ri < rows)) {
        if (ri == rows || (ci < cols && colW[ci] <= rowH[ri])) {
            int w = colW[ci];
            for (int r = 0; r + 1 < rows && need > 0; r++) {   // 这一列内部的竖直相邻对
                if (unite(ci * rows + r, ci * rows + r + 1)) {
                    ans += w;
                    need--;
                }
            }
            ci++;
        } else {
            int h = rowH[ri];
            for (int c = 0; c + 1 < cols && need > 0; c++) {   // 这一行内部的水平相邻对
                if (unite(c * rows + ri, (c + 1) * rows + ri)) {
                    ans += h;
                    need--;
                }
            }
            ri++;
        }
    }
    printf("%lld\n", ans);
    return 0;
}
