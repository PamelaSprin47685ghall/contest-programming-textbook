#include <algorithm>
#include <cstdio>
#include <queue>
#include <vector>

using namespace std;

const int MAXN = 100000;                     // 检查点数量上限
const int MAXK = 100000;                     // 票的数量上限
const long long INF = 1000000000000000000LL; // 1e18：比任何可行答案都大，两份相加也不会溢出

// 一张票：在 buyAt 号检查点花 price 的价格买下，之后可以进入 [left, right] 中的一切检查点
struct Ticket {
    int buyAt;
    int price;
    int left;
    int right;
};

int n, k;
Ticket tickets[MAXK]; // 按 left 升序排列后的票
int treeSize;         // 线段树叶子数，取不小于 k 的 2 的幂
int maxRight[4 * MAXK]; // 线段树结点存该区间内所有票 right 的最大值，已用掉的票记 -1

int removedTickets[MAXK]; // 一次查询里被"用掉"的票的下标
int removedCount;

bool ticketLess(const Ticket& x, const Ticket& y) {
    return x.left < y.left;
}

void buildTree(int node, int left, int right) {
    if (left == right) {
        // 叶子按 left 升序摆放，多出来的叶子没有票，记 -1 表示永远不会被选中
        maxRight[node] = (left < k) ? tickets[left].right : -1;
        return;
    }
    int middle = (left + right) / 2;
    buildTree(node * 2, left, middle);
    buildTree(node * 2 + 1, middle + 1, right);
    maxRight[node] = max(maxRight[node * 2], maxRight[node * 2 + 1]);
}

// 找出所有满足 left <= position <= right 且还没用掉的票，并把它们从树里删掉
void collectTickets(int node, int left, int right, int position) {
    // 叶子按 left 升序：tickets[left].left 已是本子树中最小的 left，
    // 它都超过 position，整棵子树就没有票覆盖 position；
    // maxRight 比 position 还小，则本子树里也没有票的区间到达 position。
    if (left >= k || tickets[left].left > position || maxRight[node] < position) return;
    if (left == right) {
        maxRight[node] = -1;
        removedTickets[removedCount++] = left;
        return;
    }
    int middle = (left + right) / 2;
    collectTickets(node * 2, left, middle, position);
    collectTickets(node * 2 + 1, middle + 1, right, position);
    maxRight[node] = max(maxRight[node * 2], maxRight[node * 2 + 1]);
}

struct QueueNode {
    long long distance;
    int checkpoint;
    // 优先队列默认是大根堆，把比较反过来就得到按距离从小到大取出的堆
    bool operator<(const QueueNode& other) const {
        return distance > other.distance;
    }
};

// dist 的下标 0..n-1 是检查点，n..n+k-1 是票。
// dist[v] 的含义：从 v 出发，一路买票换取通行权，最后能取到的最小代价。
// 初始给出的 dist 就是能直接取到的代价，本过程让每个结点取到它经任意次买票后
// 能达到的最小代价。
void dijkstra(long long dist[]) {
    buildTree(1, 0, treeSize - 1);
    priority_queue<QueueNode> queue;
    // 票结点上的初值表示"站在票所覆盖的某个检查点上"的代价，
    // 折算过去就是在它的售卖点花 price 买下这张票
    for (int i = n; i < n + k; ++i) {
        int target = tickets[i - n].buyAt;
        long long candidate = dist[i] + tickets[i - n].price;
        if (candidate < dist[target]) dist[target] = candidate;
    }
    for (int i = 0; i < n; ++i) {
        if (dist[i] < INF) {
            QueueNode start;
            start.distance = dist[i];
            start.checkpoint = i;
            queue.push(start);
        }
    }
    while (!queue.empty()) {
        QueueNode current = queue.top();
        queue.pop();
        if (current.distance > dist[current.checkpoint]) continue; // 堆里的旧记录
        removedCount = 0;
        collectTickets(1, 0, treeSize - 1, current.checkpoint);
        // 这些票覆盖当前检查点；当前弹出的距离是覆盖区间里最小的，
        // 所以票结点能取到 current.distance，在 buyAt 买下它们能得到
        // current.distance + price 的代价。
        for (int t = 0; t < removedCount; ++t) {
            int index = removedTickets[t];
            if (dist[n + index] > current.distance) {
                dist[n + index] = current.distance;
                int target = tickets[index].buyAt;
                long long candidate = current.distance + tickets[index].price;
                if (candidate < dist[target]) {
                    dist[target] = candidate;
                    QueueNode fresh;
                    fresh.distance = candidate;
                    fresh.checkpoint = target;
                    queue.push(fresh);
                }
            }
        }
    }
}

int main() {
    scanf("%d%d", &n, &k);
    for (int i = 0; i < k; ++i) {
        int c, p, a, b;
        scanf("%d%d%d%d", &c, &p, &a, &b);
        tickets[i].buyAt = c - 1;
        tickets[i].price = p;
        tickets[i].left = a - 1;
        tickets[i].right = b - 1;
    }
    sort(tickets, tickets + k, ticketLess);
    treeSize = 1;
    while (treeSize < k) treeSize *= 2;

    // 从 1 号检查点出发倒着买单：算出每个结点"让 1 号可达"的最小花费
    vector<long long> distLeft(n + k, INF);
    distLeft[0] = 0;
    dijkstra(&distLeft[0]);

    // 同理算出每个结点"让 N 号可达"的最小花费
    vector<long long> distRight(n + k, INF);
    distRight[n - 1] = 0;
    dijkstra(&distRight[0]);

    // 最优方案一定可以拆成：先从起点 i 走到某个结点 z（检查点或已买下的票），
    // 再从 z 分别把 1 号和 N 号纳入可达集合（两条路线不再共用票）。
    // 于是把每个 z 的两部分花销先加起来，再做一次同样的最短路。
    vector<long long> dist(n + k);
    for (int i = 0; i < n + k; ++i) {
        dist[i] = distLeft[i] + distRight[i];
    }
    dijkstra(&dist[0]);

    for (int i = 0; i < n; ++i) {
        if (dist[i] < INF) printf("%lld\n", dist[i]);
        else printf("-1\n");
    }
    return 0;
}
