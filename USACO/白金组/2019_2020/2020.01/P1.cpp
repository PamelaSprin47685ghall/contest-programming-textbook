#include <cstdio>

using namespace std;

const int MAXN = 1005;         // 网格边长上限
const int MAXNODE = 1000005;   // 等价类个数上限
const int MOD = 1000000007;

int N, M;
char grid[MAXN][MAXN];

int uf[MAXN][MAXN];      // 每一层内部的并查集, 把同层可达的空格合成等价类
int up[MAXN][MAXN];      // up[i][j]: 第 i 层代表元 j 的等价类向上接触到的列号
int idOf[MAXN][MAXN];    // 每个代表元在树上的节点编号
int parent[MAXNODE];     // 树上的父节点
int ways[MAXNODE];       // dp: 该等价类 (连同其下方所有等价类) 的填水方案数
int nodeCount = 1;       // 0 号是虚拟根, 代表 "没有上方等价类"

int Find(int row, int x) {
    while (uf[row][x] != x) x = uf[row][x] = uf[row][uf[row][x]];
    return x;
}

void Unite(int row, int a, int b) {
    a = Find(row, a);
    b = Find(row, b);
    if (a != b) uf[row][a] = b;   // 合并后 a 不再是代表元, 之后的 find 会跳过它
}

int main() {
    scanf("%d%d", &N, &M);
    for (int i = 0; i < N; i++) scanf("%s", grid[i]);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            uf[i][j] = j;
            up[i][j] = -1;
        }
    }

    // 自下而上扫描: 先把同一层里横向相邻的空格并成等价类, 再接入下方的等价类.
    for (int i = N - 1; i >= 0; i--) {
        for (int j = 0; j + 1 < M; j++) {
            if (grid[i][j] == '.' && grid[i][j + 1] == '.') Unite(i, j, j + 1);
        }
        if (i < N - 1) {
            for (int j = 0; j < M; j++) {
                if (grid[i][j] == '.' && grid[i + 1][j] == '.') {
                    int r = Find(i + 1, j);
                    // 同一层的这些列都会被合并, 所以只需记住第一个接触到的列
                    if (up[i + 1][r] == -1) up[i + 1][r] = j;
                    else Unite(i, j, up[i + 1][r]);
                }
            }
        }
    }

    // 自下而上编号: 保证下方 (孩子) 的编号一定小于上方 (父亲) 的编号.
    for (int i = N - 1; i >= 0; i--) {
        for (int j = 0; j < M; j++) {
            if (grid[i][j] == '.' && Find(i, j) == j) idOf[i][j] = nodeCount++;
        }
    }
    for (int i = N - 1; i >= 0; i--) {
        for (int j = 0; j < M; j++) {
            if (grid[i][j] == '.' && Find(i, j) == j) {
                int me = idOf[i][j];
                if (up[i][j] == -1) parent[me] = 0;   // 上方被岩石封住, 挂在虚拟根下
                else parent[me] = idOf[i - 1][Find(i - 1, up[i][j])];
            }
        }
    }

    // 每个等价类只有两种命运: 整个充满水, 或者完全不填; 填水时必须保持下方原样.
    for (int v = 0; v < nodeCount; v++) ways[v] = 1;
    for (int v = 1; v < nodeCount; v++) {
        int x = ways[v] + 1;      // +1 表示这个等价类自己充满水
        if (x >= MOD) x -= MOD;
        ways[v] = x;
        ways[parent[v]] = (long long)ways[parent[v]] * x % MOD;
    }
    // 虚拟根不能填水, 所以它的答案就是所有孩子的方案数之积
    printf("%d\n", ways[0]);
    return 0;
}
