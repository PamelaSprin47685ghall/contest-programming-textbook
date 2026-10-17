#include <cstdio>
#include <cstring>
using namespace std;

const int MAXN = 100005;  // N <= 100000
const int MAXM = 100005;  // M < N
const long long MOD = 1000000007LL;

// 邻接表用数组链式前向星，避免引入 vector
int head[MAXN], to[2 * MAXM], nxt[2 * MAXM];
int edgeCnt = 0;
bool visited[MAXN];
int stackArr[MAXN];

void addEdge(int u, int v) {
    to[edgeCnt] = v;
    nxt[edgeCnt] = head[u];
    head[u] = edgeCnt++;
}

int main() {
    int N, M;
    scanf("%d %d", &N, &M);
    memset(head, -1, sizeof(head));
    for (int i = 0; i < M; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        addEdge(u, v);
        addEdge(v, u);
    }

    long long ans = 1;
    for (int s = 1; s <= N; s++) {
        if (visited[s] || head[s] == -1) continue;  // 孤立农场不贡献方案数
        // 迭代 DFS 求出该连通块的顶点数 verts 与边数（度数和的一半）
        int top = 0;
        stackArr[top++] = s;
        visited[s] = true;
        int verts = 0, degSum = 0;
        while (top > 0) {
            int u = stackArr[--top];
            verts++;
            for (int e = head[u]; e != -1; e = nxt[e]) {
                degSum++;
                int v = to[e];
                if (!visited[v]) {
                    visited[v] = true;
                    stackArr[top++] = v;
                }
            }
        }
        int edges = degSum / 2;
        if (edges == verts - 1) {
            ans = ans * verts % MOD;      // 树：任选一个农场作为没建路的那个
        } else if (edges == verts) {
            ans = ans * 2 % MOD;          // 恰含一圈：环上两个方向
        } else {
            ans = 0;                      // 边数超过点数，必有一个农场要建两条路
        }
    }
    printf("%lld\n", ans);
    return 0;
}
