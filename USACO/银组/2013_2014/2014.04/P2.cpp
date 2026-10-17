#include <cstdio>
#include <queue>
#include <vector>

using namespace std;

const int MAXN = 10005;
const int MAXM = 50005;

// 从家到农场最多经过 N-1 条路, 每条路的时间不超过 10^5,
// 所以任何最短路距离都严格小于 10^9, 可以用它当"无穷大"
const int INF = 1000000000;

struct Arc {
    int target;  // 这条弧指向的节点
    int weight;  // 走这条弧的代价
};

struct QueueNode {
    int distance;
    int node;

    // priority_queue 默认弹出"最大"的元素; 让距离小的算作"更大",
    // 这样弹出的就是当前距离最小的节点
    bool operator<(const QueueNode &other) const {
        return distance > other.distance;
    }
};

int nodeCount, edgeCount;
int edgeFrom[MAXM];
int edgeTo[MAXM];
int firstTime[MAXM];   // 第一个 GPS 认为的通行时间
int secondTime[MAXM];  // 第二个 GPS 认为的通行时间

// 反向图: 在反向图里从 N 出发跑最短路, 得到的就是原图中每个点到 N 的最短时间
vector<Arc> firstReverse[MAXN];
vector<Arc> secondReverse[MAXN];
// 抱怨图: 每条路的代价是走它会被抱怨几次
vector<Arc> complaintGraph[MAXN];

int firstDistance[MAXN];
int secondDistance[MAXN];
int answerDistance[MAXN];

void RunDijkstra(vector<Arc> graph[], int source, int distance[]) {
    for (int i = 1; i <= nodeCount; i++) {
        distance[i] = INF;
    }
    distance[source] = 0;
    priority_queue<QueueNode> heap;
    QueueNode start;
    start.distance = 0;
    start.node = source;
    heap.push(start);
    while (!heap.empty()) {
        QueueNode current = heap.top();
        heap.pop();
        if (current.distance != distance[current.node]) {
            continue;  // 这是一条过期记录: 该节点后来找到了更短的路线
        }
        for (int k = 0; k < (int)graph[current.node].size(); k++) {
            Arc arc = graph[current.node][k];
            int through = current.distance + arc.weight;
            if (through < distance[arc.target]) {
                distance[arc.target] = through;
                QueueNode next;
                next.distance = through;
                next.node = arc.target;
                heap.push(next);
            }
        }
    }
}

int main() {
    scanf("%d %d", &nodeCount, &edgeCount);
    for (int i = 0; i < edgeCount; i++) {
        scanf("%d %d %d %d", &edgeFrom[i], &edgeTo[i], &firstTime[i], &secondTime[i]);
        Arc arc;
        arc.target = edgeFrom[i];
        arc.weight = firstTime[i];
        firstReverse[edgeTo[i]].push_back(arc);
        arc.weight = secondTime[i];
        secondReverse[edgeTo[i]].push_back(arc);
    }

    RunDijkstra(firstReverse, nodeCount, firstDistance);
    RunDijkstra(secondReverse, nodeCount, secondDistance);

    // 从 X 通向 Y 的路若满足 "这条路的时间 + 从 Y 到农场的最短时间 == 从 X 到农场的最短时间",
    // 它就在 X 到农场的一条最短路上, 该 GPS 不抱怨; 否则这个 GPS 抱怨一次
    for (int i = 0; i < edgeCount; i++) {
        int complaints = 0;
        if (firstDistance[edgeTo[i]] + firstTime[i] != firstDistance[edgeFrom[i]]) {
            complaints++;
        }
        if (secondDistance[edgeTo[i]] + secondTime[i] != secondDistance[edgeFrom[i]]) {
            complaints++;
        }
        Arc arc;
        arc.target = edgeTo[i];
        arc.weight = complaints;
        complaintGraph[edgeFrom[i]].push_back(arc);
    }

    // 把抱怨次数当路长, 从家到农场的最短路就是最少抱怨次数
    RunDijkstra(complaintGraph, 1, answerDistance);
    printf("%d\n", answerDistance[nodeCount]);
    return 0;
}
