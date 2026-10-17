#include <cstdio>

using namespace std;

const int MAXN = 100000;      // 谷仓数上限
const int FAR = MAXN + 1;     // 当作"无穷远"

int n, startBarn;
int degree[MAXN + 1];

// 邻接表用数组模拟: head[u] 是 u 的第一条边, nextEdge 串起同一起点的边
int head[MAXN + 1];
int to[2 * MAXN];
int nextEdge[2 * MAXN];
int edgeCount = 0;

int depth[MAXN + 1];     // 到 Bessie 起点的距离
int parentOf[MAXN + 1];  // 以起点为根时的父亲
int distLeaf[MAXN + 1];  // 到最近的出口(叶)的距离
int queue[MAXN + 1];

void AddEdge(int u, int v) {
    edgeCount++;
    to[edgeCount] = v;
    nextEdge[edgeCount] = head[u];
    head[u] = edgeCount;
}

int main() {
    scanf("%d %d", &n, &startBarn);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        AddEdge(u, v);
        AddEdge(v, u);
        degree[u]++;
        degree[v]++;
    }

    // 起点自己就是出口时, Bessie 一露面就脱身, 一个农夫就够(官方结论)
    if (degree[startBarn] == 1) {
        printf("1\n");
        return 0;
    }

    // 以起点为根, 求每个谷仓到根的距离和父亲
    for (int i = 1; i <= n; i++) {
        depth[i] = FAR;
    }
    depth[startBarn] = 0;
    parentOf[startBarn] = 0;
    int qHead = 0, qTail = 0;
    queue[qTail++] = startBarn;
    while (qHead < qTail) {
        int u = queue[qHead++];
        for (int e = head[u]; e != 0; e = nextEdge[e]) {
            int v = to[e];
            if (v == parentOf[u]) {
                continue;
            }
            parentOf[v] = u;
            depth[v] = depth[u] + 1;
            queue[qTail++] = v;
        }
    }

    // 从所有出口同时出发做 BFS, 得到每个谷仓到最近出口的距离.
    // 农夫朝根走最优, 所以"某点是否被守住"只看它离出口有多远.
    for (int i = 1; i <= n; i++) {
        distLeaf[i] = FAR;
    }
    qHead = 0;
    qTail = 0;
    for (int i = 1; i <= n; i++) {
        if (degree[i] == 1) {
            distLeaf[i] = 0;
            queue[qTail++] = i;
        }
    }
    while (qHead < qTail) {
        int u = queue[qHead++];
        for (int e = head[u]; e != 0; e = nextEdge[e]) {
            int v = to[e];
            if (distLeaf[v] > distLeaf[u] + 1) {
                distLeaf[v] = distLeaf[u] + 1;
                queue[qTail++] = v;
            }
        }
    }

    // 计数: 父亲还"比出口更近"(Bessie 能安全到达) 而自己已经"被出口追上"的谷仓.
    // 每个这样的谷仓对应一个必须派农夫的子树, 且不会重复计数.
    int farmers = 0;
    for (int i = 1; i <= n; i++) {
        if (i == startBarn) {
            continue;
        }
        if (distLeaf[i] <= depth[i] && distLeaf[parentOf[i]] > depth[parentOf[i]]) {
            farmers++;
        }
    }
    printf("%d\n", farmers);
    return 0;
}
