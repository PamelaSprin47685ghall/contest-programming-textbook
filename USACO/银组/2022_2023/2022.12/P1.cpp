#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 200005;

int n;
long long hay[MAXN];       // 每个仓库当前的干草数
vector<int> adj[MAXN];     // 邻接表
int parent[MAXN];          // 以 1 号仓库为根时的父结点
int bfsOrder[MAXN];        // 宽度优先遍历序
long long surplus[MAXN];   // 子树内的干草数减去子树应有的干草数

// 一条搬运命令：把 amount 捆干草从 from 号仓库搬到 to 号仓库
struct Order {
    int from, to;
    long long amount;
};
vector<Order> orders;

// 用显式栈模拟官方题解里的递归过程（链状树深度可达 2*10^5，递归会爆栈）：
// type == 0 表示"处理某棵子树"，type == 1 表示"执行一次搬运"。
struct Task {
    int type;
    int u, v;
    long long amount;
};
vector<Task> stk;

int main() {
    scanf("%d", &n);
    long long total = 0;
    for (int i = 1; i <= n; ++i) {
        scanf("%lld", &hay[i]);
        total += hay[i];
    }
    long long avg = total / n;
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        scanf("%d %d", &u, &v);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // 迭代式 BFS 定根，避免链状树上递归过深
    int head = 0, tail = 0;
    bfsOrder[tail++] = 1;
    parent[1] = 0;
    while (head < tail) {
        int x = bfsOrder[head++];
        for (size_t k = 0; k < adj[x].size(); ++k) {
            int y = adj[x][k];
            if (y == parent[x]) continue;
            parent[y] = x;
            bfsOrder[tail++] = y;
        }
    }

    // 逆 BFS 序求子树盈亏：叶子先算，父亲后算
    for (int i = n - 1; i >= 0; --i) {
        int x = bfsOrder[i];
        surplus[x] = hay[x] - avg;
        for (size_t k = 0; k < adj[x].size(); ++k) {
            int y = adj[x][k];
            if (y == parent[x]) continue;
            surplus[x] += surplus[y];
        }
    }

    // 一条边被用到当且仅当它两侧某一边的盈亏不为零，因此用到的边数就是答案
    int usedEdges = 0;
    for (int i = 1; i <= n; ++i) {
        if (i != 1 && surplus[i] != 0) ++usedEdges;
    }

    // 官方贪心：先让所有"盈余"子树把多余的干草送上来，再把干草下发给"亏欠"子树
    Task start;
    start.type = 0;
    start.u = 1;
    start.v = 0;
    start.amount = 0;
    stk.push_back(start);
    while (!stk.empty()) {
        Task cur = stk.back();
        stk.pop_back();
        if (cur.type == 1) {
            Order o;
            o.from = cur.u;
            o.to = cur.v;
            o.amount = cur.amount;
            orders.push_back(o);
            continue;
        }
        int x = cur.u;
        // 逆序压栈即可让出栈顺序与题解一致：先处理盈余孩子，再处理亏欠孩子
        for (size_t k = adj[x].size(); k > 0; --k) {
            int y = adj[x][k - 1];
            if (y == parent[x] || surplus[y] >= 0) continue;
            Task t;
            t.type = 0;
            t.u = y;
            t.v = 0;
            t.amount = 0;
            stk.push_back(t);              // 稍后处理这棵亏欠子树
            t.type = 1;
            t.u = x;
            t.v = y;
            t.amount = -surplus[y];        // 把干草从 x 下发到 y
            stk.push_back(t);
        }
        for (size_t k = adj[x].size(); k > 0; --k) {
            int y = adj[x][k - 1];
            if (y == parent[x] || surplus[y] < 0) continue;
            Task t;
            if (surplus[y] > 0) {
                t.type = 1;
                t.u = y;
                t.v = x;
                t.amount = surplus[y];     // 盈余孩子把多余的干草送给 x
                stk.push_back(t);
            }
            t.type = 0;
            t.u = y;
            t.v = 0;
            t.amount = 0;
            stk.push_back(t);
        }
    }

    printf("%d\n", usedEdges);
    for (size_t i = 0; i < orders.size(); ++i) {
        printf("%d %d %lld\n", orders[i].from, orders[i].to, orders[i].amount);
    }
    return 0;
}
