#include <cstdio>

using namespace std;

const int MAXN = 5000;

// 树的邻接表用“链式前向星”：head[v] 是从视频 v 出发的第一条边编号，
// 每条边再记下同起点的下一条边。N 只有 5000，这样做省内存也够快。
struct Edge {
    int to;
    int weight;
    int next;
};

int head[MAXN + 1];
Edge edge[2 * MAXN];
int edgeCount = 0;

void addEdge(int u, int v, int weight) {
    edge[edgeCount].to = v;
    edge[edgeCount].weight = weight;
    edge[edgeCount].next = head[u];
    head[u] = edgeCount;
    edgeCount++;
}

int bfsQueue[MAXN + 1];
bool visited[MAXN + 1];

int main() {
    int n, queryCount;
    scanf("%d %d", &n, &queryCount);
    for (int i = 1; i <= n; i++) head[i] = -1;
    for (int i = 0; i < n - 1; i++) {
        int u, v, relevance;
        scanf("%d %d %d", &u, &v, &relevance);
        addEdge(u, v, relevance);
        addEdge(v, u, relevance);
    }

    // 两个视频之间的相关度是路径上的最小边权，所以“与 v 的相关度不小于 k”
    // 当且仅当存在一条从 v 出发、全程只走权值不小于 k 的边的路径。
    // 每次询问做一遍 BFS，遇到权值小于 k 的边就不走，数能到达多少个别的视频。
    for (int query = 0; query < queryCount; query++) {
        int k, v;
        scanf("%d %d", &k, &v);
        for (int i = 1; i <= n; i++) visited[i] = false;
        int front = 0, back = 0;
        bfsQueue[back++] = v;
        visited[v] = true;
        int answer = 0;
        while (front < back) {
            int current = bfsQueue[front++];
            for (int e = head[current]; e != -1; e = edge[e].next) {
                int next = edge[e].to;
                if (!visited[next] && edge[e].weight >= k) {
                    visited[next] = true;
                    bfsQueue[back++] = next;
                    answer++;
                }
            }
        }
        printf("%d\n", answer);
    }
    return 0;
}
