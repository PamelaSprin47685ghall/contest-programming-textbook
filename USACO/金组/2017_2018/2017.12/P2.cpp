#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 100000;
const long long MOD = 1000000007;

int n, k;
vector<int> neighbor[MAXN + 1];  // 谷仓之间的通路
int parent[MAXN + 1];            // 以 1 号谷仓为根时的父结点
int painted[MAXN + 1];           // 已经刷好的颜色, 0 表示还没刷
long long ways[MAXN + 1][3];     // ways[v][c]: v 刷成颜色 c+1 时, v 的子树有多少种合法刷法
int order[MAXN + 1];             // 自根向下的遍历序

int main() {
    scanf("%d%d", &n, &k);
    for (int i = 1; i < n; i++) {
        int x, y;
        scanf("%d%d", &x, &y);
        neighbor[x].push_back(y);
        neighbor[y].push_back(x);
    }
    for (int i = 0; i < k; i++) {
        int barn, color;
        scanf("%d%d", &barn, &color);
        painted[barn] = color;
    }

    // 通路构成一棵树, 从 1 号谷仓出发就能定下父子关系.
    // 树可能是一条 10^5 长的链, 递归会爆栈, 所以用数组当队列手动遍历.
    int head = 0, tail = 0;
    order[tail++] = 1;
    parent[1] = 0;
    while (head < tail) {
        int v = order[head++];
        for (int i = 0; i < (int)neighbor[v].size(); i++) {
            int u = neighbor[v][i];
            if (u == parent[v]) continue;  // 树里除了父结点, 其余邻居都是儿子
            parent[u] = v;
            order[tail++] = u;
        }
    }

    // 按遍历序的逆序处理: 处理 v 时它所有儿子的答案都已经算好
    for (int i = tail - 1; i >= 0; i--) {
        int v = order[i];
        for (int c = 0; c < 3; c++) {
            ways[v][c] = (painted[v] == 0 || painted[v] == c + 1) ? 1 : 0;
        }
        for (int j = 0; j < (int)neighbor[v].size(); j++) {
            int u = neighbor[v][j];
            if (u == parent[v]) continue;
            for (int c = 0; c < 3; c++) {
                // 相邻谷仓不能同色, 所以儿子 u 的三种颜色里要扣掉 c 这一种
                long long notSame = (ways[u][0] + ways[u][1] + ways[u][2] - ways[u][c]) % MOD;
                if (notSame < 0) notSame += MOD;
                ways[v][c] = ways[v][c] * notSame % MOD;
            }
        }
    }

    printf("%lld\n", (ways[1][0] + ways[1][1] + ways[1][2]) % MOD);
    return 0;
}
