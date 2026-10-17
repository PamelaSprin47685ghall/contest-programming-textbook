#include <cstdio>

using namespace std;

const int MAXN = 500005;
const int INF = 1000000000;

int nextFarm[MAXN];
int farmerHere[MAXN];
int head[MAXN], toFarm[MAXN], nextEdge[MAXN];
int state[MAXN];
int farmerDistance[MAXN];
int depthOf[MAXN];
int cycleSlot[MAXN];
int answer[MAXN];
int cycle[MAXN];
int treeNode[MAXN];
int orderInTree[MAXN];
int goodAt[MAXN];
int skipToGood[MAXN];
int stackNode[MAXN];
int stackEdge[MAXN];

int main() {
    int n, farmerCount;
    scanf("%d %d", &n, &farmerCount);
    for (int farm = 1; farm <= n; ++farm) {
        scanf("%d", &nextFarm[farm]);
        head[farm] = -1;
        farmerDistance[farm] = INF;
    }
    for (int farm = 1; farm <= n; ++farm) {
        int parent = nextFarm[farm];
        toFarm[farm] = farm;
        nextEdge[farm] = head[parent];
        head[parent] = farm;
    }
    for (int i = 0; i < farmerCount; ++i) {
        int farm;
        scanf("%d", &farm);
        farmerHere[farm] = 1;
        farmerDistance[farm] = 0;
    }

    for (int start = 1; start <= n; ++start) {
        if (state[start]) continue;

        int cursor = start;
        while (state[cursor] == 0) {
            state[cursor] = 1;
            cursor = nextFarm[cursor];
        }
        int cycleSize = 0;
        if (state[cursor] == 1) {
            int onCycle = cursor;
            do {
                cycle[cycleSize++] = onCycle;
                state[onCycle] = 2;
                onCycle = nextFarm[onCycle];
            } while (onCycle != cursor);
        }
        cursor = start;
        while (state[cursor] == 1) {
            state[cursor] = 3;
            cursor = nextFarm[cursor];
        }
        if (cycleSize == 0) continue;

        for (int i = 0; i < cycleSize; ++i) goodAt[i] = 1;

        int treeCount = 0;
        for (int slot = 0; slot < cycleSize; ++slot) {
            int root = cycle[slot];
            int top = 0;
            stackNode[0] = root;
            stackEdge[0] = head[root];
            depthOf[root] = 0;
            cycleSlot[root] = slot;
            treeNode[treeCount++] = root;

            while (top >= 0) {
                int node = stackNode[top];
                int edge = stackEdge[top];
                if (edge == -1) {
                    --top;
                    continue;
                }
                stackEdge[top] = nextEdge[edge];
                int child = toFarm[edge];
                // 环上前一个点指向根，不能把它再当成树上的孩子。
                if (node == root && child == cycle[(slot + cycleSize - 1) % cycleSize]) continue;
                if (state[child] == 2) continue;
                depthOf[child] = depthOf[node] + 1;
                cycleSlot[child] = slot;
                state[child] = 3;
                treeNode[treeCount++] = child;
                ++top;
                stackNode[top] = child;
                stackEdge[top] = head[child];
            }
        }

        // 后序：先子后父，农夫距离才能从叶子传到根。
        int orderCount = 0;
        for (int slot = 0; slot < cycleSize; ++slot) {
            int root = cycle[slot];
            int top = 0;
            stackNode[0] = root;
            stackEdge[0] = head[root];
            while (top >= 0) {
                int node = stackNode[top];
                int edge = stackEdge[top];
                if (edge == -1) {
                    orderInTree[orderCount++] = node;
                    --top;
                    continue;
                }
                stackEdge[top] = nextEdge[edge];
                int child = toFarm[edge];
                if (node == root && child == cycle[(slot + cycleSize - 1) % cycleSize]) continue;
                if (depthOf[child] != depthOf[node] + 1 || cycleSlot[child] != slot) continue;
                ++top;
                stackNode[top] = child;
                stackEdge[top] = head[child];
            }
        }
        for (int i = 0; i < orderCount; ++i) {
            int node = orderInTree[i];
            for (int edge = head[node]; edge != -1; edge = nextEdge[edge]) {
                int child = toFarm[edge];
                if (depthOf[child] != depthOf[node] + 1 || cycleSlot[child] != cycleSlot[node]) continue;
                if (farmerDistance[child] + 1 < farmerDistance[node]) {
                    farmerDistance[node] = farmerDistance[child] + 1;
                }
            }
            if (farmerHere[node] && farmerDistance[node] == 0) {
                int place = (cycleSlot[node] - depthOf[node] % cycleSize + cycleSize) % cycleSize;
                goodAt[place] = 0;
            }
        }

        // 环上的农夫还能再往回追一圈，缩短别的环点的安全休息时间。
        for (int step = 0; step < 2 * cycleSize; ++step) {
            int slot = step % cycleSize;
            int previous = (slot + cycleSize - 1) % cycleSize;
            if (farmerDistance[cycle[previous]] + 1 < farmerDistance[cycle[slot]]) {
                farmerDistance[cycle[slot]] = farmerDistance[cycle[previous]] + 1;
            }
        }

        for (int i = 0; i < cycleSize; ++i) skipToGood[i] = INF;
        for (int i = 2 * cycleSize - 1; i >= 0; --i) {
            int place = i % cycleSize;
            if (goodAt[place]) skipToGood[place] = 0;
            else if (skipToGood[(place + 1) % cycleSize] < INF) {
                skipToGood[place] = skipToGood[(place + 1) % cycleSize] + 1;
            }
        }

        for (int i = 0; i < treeCount; ++i) {
            int node = treeNode[i];
            if (farmerDistance[node] >= INF / 2) {
                answer[node] = -2;
                continue;
            }
            int restLimit = farmerDistance[node] - 1;
            if (restLimit < 0) {
                answer[node] = -1;
                continue;
            }
            int arrival = (cycleSlot[node] - (restLimit + depthOf[node]) % cycleSize + cycleSize) % cycleSize;
            if (skipToGood[arrival] >= INF) {
                answer[node] = -1;
                continue;
            }
            int best = restLimit - skipToGood[arrival];
            answer[node] = best >= 0 ? best : -1;
        }
    }

    for (int farm = 1; farm <= n; ++farm) printf("%d\n", answer[farm]);
    return 0;
}
