#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 100000;

int n, m;
char breed[MAXN + 1];       // 每座农场的奶牛品种：'G' 或 'H'
vector<int> adj[MAXN + 1];  // 树的邻接表
int comp[MAXN + 1];         // 所属的"同品种极大连通块"编号
int stk[MAXN + 1];          // 手写栈，避免链状树上的深递归爆栈
char answer[MAXN + 2];

int main() {
    scanf("%d%d", &n, &m);

    char buf[MAXN + 2];
    scanf("%s", buf);
    for (int i = 1; i <= n; ++i) breed[i] = buf[i - 1];

    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // 只沿同品种的边搜索，把每条极大同品种连通链压缩成一个连通块：
    // 两端点在同一块内 ⇔ 路径上所有农场品种相同。
    int compCnt = 0;
    for (int start = 1; start <= n; ++start) {
        if (comp[start]) continue;
        ++compCnt;
        int top = 0;
        stk[top++] = start;
        comp[start] = compCnt;
        while (top > 0) {
            int u = stk[--top];
            for (size_t k = 0; k < adj[u].size(); ++k) {
                int v = adj[u][k];
                // 入栈时就打标记，保证每个点只进栈一次，栈容量不超过 N。
                if (!comp[v] && breed[v] == breed[u]) {
                    comp[v] = compCnt;
                    stk[top++] = v;
                }
            }
        }
    }

    for (int i = 0; i < m; ++i) {
        int a, b;
        char c;
        scanf("%d%d %c", &a, &b, &c);
        // 若两端点不在同一块内，路径上必然两种品种都出现，朋友一定满意；
        // 否则路径上所有奶牛都与 A 品种相同，只需比较 A 的品种与朋友的偏好。
        answer[i] = (breed[a] == c || comp[a] != comp[b]) ? '1' : '0';
    }
    answer[m] = '\0';

    printf("%s\n", answer);
    return 0;
}
