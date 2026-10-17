#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 400005;
const int MAXM = 400005;

int leftBound[MAXN], rightBound[MAXN];
int head[MAXN], toNode[2 * MAXM], edgeSum[2 * MAXM], nextEdge[2 * MAXM];
int edgeCount;
int signOf[MAXN];
long long offsetOf[MAXN];
int visited[MAXN];
int componentNode[MAXN];
long long eventValue[2 * MAXN];
int eventDelta[2 * MAXN];
int eventOrder[2 * MAXN];

void addEdge(int from, int to, int sum) {
    toNode[edgeCount] = to;
    edgeSum[edgeCount] = sum;
    nextEdge[edgeCount] = head[from];
    head[from] = edgeCount;
    ++edgeCount;
}

bool eventLess(int a, int b) {
    if (eventValue[a] != eventValue[b]) return eventValue[a] < eventValue[b];
    // 同一点上先减后加：区间右端的失效事件和下一区间的开始事件重合时，不能算作同时覆盖。
    return eventDelta[a] < eventDelta[b];
}

void solve() {
    int n, m;
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; ++i) scanf("%d", &leftBound[i]);
    for (int i = 0; i < n; ++i) scanf("%d", &rightBound[i]);

    edgeCount = 0;
    for (int i = 0; i < n; ++i) {
        head[i] = -1;
        visited[i] = 0;
    }
    for (int i = 0; i < m; ++i) {
        int x, y, z;
        scanf("%d %d %d", &x, &y, &z);
        --x;
        --y;
        addEdge(x, y, z);
        addEdge(y, x, z);
    }

    long long answer = 0;
    bool impossible = false;
    for (int start = 0; start < n && !impossible; ++start) {
        if (visited[start]) continue;

        // 连通块里每根棒的能量都能写成 sign * a_start + offset。
        // 环上同一根棒出现两种写法时，就得到关于 a_start 的一次方程。
        int componentSize = 0;
        int stackNode[MAXN];
        int stackSign[MAXN];
        long long stackOffset[MAXN];
        int stackTop = 0;
        stackNode[0] = start;
        stackSign[0] = 1;
        stackOffset[0] = 0;
        visited[start] = 1;
        signOf[start] = 1;
        offsetOf[start] = 0;
        componentNode[componentSize++] = start;

        bool fixed = false;
        long long fixedValue = 0;
        while (stackTop >= 0 && !impossible) {
            int node = stackNode[stackTop];
            int sign = stackSign[stackTop];
            long long offset = stackOffset[stackTop];
            --stackTop;
            for (int edge = head[node]; edge != -1; edge = nextEdge[edge]) {
                int next = toNode[edge];
                int nextSign = -sign;
                long long nextOffset = edgeSum[edge] - offset;
                if (!visited[next]) {
                    visited[next] = 1;
                    signOf[next] = nextSign;
                    offsetOf[next] = nextOffset;
                    componentNode[componentSize++] = next;
                    ++stackTop;
                    stackNode[stackTop] = next;
                    stackSign[stackTop] = nextSign;
                    stackOffset[stackTop] = nextOffset;
                } else if (signOf[next] == nextSign) {
                    if (offsetOf[next] != nextOffset) impossible = true;
                } else {
                    // sign * x + offset = -sign * x + nextOffset，解出唯一的 x。
                    long long numerator = nextOffset - offsetOf[next];
                    if (numerator % 2 != 0) impossible = true;
                    else {
                        long long value = numerator / 2 * signOf[next];
                        if (!fixed) {
                            fixed = true;
                            fixedValue = value;
                        } else if (fixedValue != value) {
                            impossible = true;
                        }
                    }
                }
            }
        }
        if (impossible) break;

        int eventCount = 0;
        for (int i = 0; i < componentSize; ++i) {
            int node = componentNode[i];
            long long low, high;
            if (signOf[node] == 1) {
                low = leftBound[node] - offsetOf[node];
                high = rightBound[node] - offsetOf[node];
            } else {
                low = offsetOf[node] - rightBound[node];
                high = offsetOf[node] - leftBound[node];
            }
            eventValue[eventCount] = low;
            eventDelta[eventCount] = 1;
            eventOrder[eventCount] = eventCount;
            ++eventCount;
            eventValue[eventCount] = high + 1;
            eventDelta[eventCount] = -1;
            eventOrder[eventCount] = eventCount;
            ++eventCount;
        }

        if (fixed) {
            int generating = 0;
            for (int i = 0; i < eventCount; ++i) {
                if (eventValue[i] <= fixedValue) generating += eventDelta[i];
            }
            answer += generating;
        } else {
            sort(eventOrder, eventOrder + eventCount, eventLess);
            int current = 0;
            int best = 0;
            for (int i = 0; i < eventCount; ++i) {
                current += eventDelta[eventOrder[i]];
                if (current > best) best = current;
            }
            answer += best;
        }
    }

    if (impossible) printf("-1\n");
    else printf("%lld\n", answer);
}

int main() {
    int testCount;
    scanf("%d", &testCount);
    while (testCount--) solve();
    return 0;
}
