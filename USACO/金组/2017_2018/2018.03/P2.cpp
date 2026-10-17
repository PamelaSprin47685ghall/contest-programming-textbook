#include <cstdio>
#include <queue>

using namespace std;

const int MAXN = 100005; // 奶牛数上限
const int MAXM = 50005;  // 观察数上限
const int MAXE = 200005; // 所有观察列出的奶牛总数上限

int n, m;
int obsStart[MAXM];  // 第 i 条观察在 obsList 中的起始位置
int obsLen[MAXM];    // 第 i 条观察的长度
int obsList[MAXE];   // 按输入顺序依次存放所有观察里列出的奶牛编号

// 邻接表用链式前向星：每次重建图只需清空指针数组，不必反复申请内存
int head[MAXN];
int edgeTo[MAXE];
int edgeNext[MAXE];
int edgeCount;
int indeg[MAXN];
int result[MAXN];

// 只使用前 k 条观察建图，求字典序最小的拓扑序；成功时写入 result 并返回 true
bool TopoSort(int k) {
    for (int i = 1; i <= n; i++) {
        head[i] = 0;
        indeg[i] = 0;
    }
    edgeCount = 0;
    for (int i = 1; i <= k; i++) {
        for (int j = 1; j < obsLen[i]; j++) {
            int u = obsList[obsStart[i] + j - 1];
            int v = obsList[obsStart[i] + j];
            edgeCount++;
            edgeTo[edgeCount] = v;
            edgeNext[edgeCount] = head[u];
            head[u] = edgeCount;
            indeg[v]++;
        }
    }

    // 小根堆用存负数的方式实现：每次取当前可以挤奶的奶牛中编号最小的那头
    priority_queue<int> q;
    for (int i = 1; i <= n; i++)
        if (indeg[i] == 0) q.push(-i);

    for (int i = 0; i < n; i++) {
        if (q.empty()) return false; // 还有奶牛没排进序列，说明约束成环，前 k 条观察无法同时满足
        int u = -q.top();
        q.pop();
        result[i] = u;
        for (int e = head[u]; e != 0; e = edgeNext[e]) {
            int v = edgeTo[e];
            if (--indeg[v] == 0) q.push(-v);
        }
    }
    return true;
}

int main() {
    scanf("%d %d", &n, &m);
    int pos = 0;
    for (int i = 1; i <= m; i++) {
        int cnt;
        scanf("%d", &cnt);
        obsStart[i] = pos;
        obsLen[i] = cnt;
        for (int j = 0; j < cnt; j++) {
            int cow;
            scanf("%d", &cow);
            obsList[pos++] = cow;
        }
    }

    // "前 X 条观察可行"关于 X 单调（去掉约束只会更容易满足），于是可以二分
    // 不变式：lo 可行，hi 不可行（m+1 是必然无解的哨兵）
    int lo = 0, hi = m + 1;
    while (hi > lo + 1) {
        int mid = (lo + hi) / 2;
        if (TopoSort(mid)) lo = mid;
        else hi = mid;
    }
    // 对最终可行的 X 再跑一次，取得字典序最小的方案
    TopoSort(lo);
    for (int i = 0; i < n; i++)
        printf("%d%c", result[i], i + 1 == n ? '\n' : ' ');
    return 0;
}
