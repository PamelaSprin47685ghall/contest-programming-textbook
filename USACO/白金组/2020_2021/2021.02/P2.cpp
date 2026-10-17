#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 100005;     // 单个测试点的顶点数上限
const int INF = 1000000000;  // 表示"走不到"

int n, m;
vector<int> adjacency[MAXN];

// distEven[v]：从 1 号顶点出发、长度为偶数并到达 v 的最短行走长度
// distOdd[v] ：从 1 号顶点出发、长度为奇数并到达 v 的最短行走长度
int distEven[MAXN];
int distOdd[MAXN];

int stateVertex[2 * MAXN];  // 分层 BFS 的队列，一个状态 = (顶点, 长度奇偶)
int stateParity[2 * MAXN];

// 每个顶点的 h(i) = (a, b)：a、b 分别是上面两个距离的较小、较大者
int vertexLayer[MAXN];  // a + b，也就是顶点所在的层
int vertexLower[MAXN];  // a，层内按它从小到大处理
int sortedVertex[MAXN];

// 把顶点按 (层, a) 压缩成组：第 i 组就是集合 s(lower, layer - lower)，
// 组里顶点个数存在 groupSize 里
int groupLayer[MAXN];
int groupLower[MAXN];
int groupSize[MAXN];
int groupNum;

bool compareVertexGroup(int x, int y) {
    if (vertexLayer[x] != vertexLayer[y]) return vertexLayer[x] < vertexLayer[y];
    return vertexLower[x] < vertexLower[y];
}

// 在"顶点 + 长度奇偶"组成的状态图上 BFS：走一条边恰好翻转长度奇偶，
// 所以每个状态只需要入队一次，第一次入队时的长度就是它的最短长度
void bfsDistances() {
    for (int v = 0; v < n; ++v) {
        distEven[v] = INF;
        distOdd[v] = INF;
    }
    distEven[0] = 0;
    int head = 0, tail = 0;
    stateVertex[tail] = 0;
    stateParity[tail] = 0;
    ++tail;
    while (head < tail) {
        int v = stateVertex[head];
        int parity = stateParity[head];
        ++head;
        int reached = parity ? distOdd[v] : distEven[v];
        for (int i = 0; i < (int)adjacency[v].size(); ++i) {
            int u = adjacency[v][i];
            if (parity == 0) {
                if (distOdd[u] == INF) {
                    distOdd[u] = reached + 1;
                    stateVertex[tail] = u;
                    stateParity[tail] = 1;
                    ++tail;
                }
            } else {
                if (distEven[u] == INF) {
                    distEven[u] = reached + 1;
                    stateVertex[tail] = u;
                    stateParity[tail] = 0;
                    ++tail;
                }
            }
        }
    }
}

// 图连通时，只要不是二分图，两类长度就都能到达每个顶点，于是 h(i) 两个坐标都有限
void buildGroups() {
    for (int v = 0; v < n; ++v) {
        int smaller = distEven[v] < distOdd[v] ? distEven[v] : distOdd[v];
        int larger = distEven[v] < distOdd[v] ? distOdd[v] : distEven[v];
        vertexLower[v] = smaller;
        vertexLayer[v] = smaller + larger;
        sortedVertex[v] = v;
    }
    sort(sortedVertex, sortedVertex + n, compareVertexGroup);
    groupNum = 0;
    for (int i = 0; i < n; ++i) {
        int v = sortedVertex[i];
        if (groupNum > 0 && groupLayer[groupNum - 1] == vertexLayer[v] &&
            groupLower[groupNum - 1] == vertexLower[v]) {
            ++groupSize[groupNum - 1];
        } else {
            groupLayer[groupNum] = vertexLayer[v];
            groupLower[groupNum] = vertexLower[v];
            groupSize[groupNum] = 1;
            ++groupNum;
        }
    }
}

// 判断层号为 layer、a 为 lower 的组 s(lower, layer - lower) 是否非空
bool groupExists(int layer, int lower) {
    int low = 0, high = groupNum - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (groupLayer[mid] == layer && groupLower[mid] == lower) return true;
        bool before = groupLayer[mid] < layer ||
                      (groupLayer[mid] == layer && groupLower[mid] < lower);
        if (before) low = mid + 1;
        else high = mid - 1;
    }
    return false;
}

// 贪心加边。层内按 a 从小到大处理：每个顶点的边只有三种合法去处，
// 而 type A 边只影响本层顶点、type B/C 边只连本层内的两组，
// 所以每层可以独立决定（层号 a + b 相同即同一层）
int minimumEdges() {
    int answer = 0;
    int i = 0;
    while (i < groupNum) {
        int layer = groupLayer[i];
        int previousLower = -1;   // 本层内前一个组的 a
        int previousAmount = 0;   // 前一个组连到本组的边数，也就是本组现成的"向上边"条数
        while (i < groupNum && groupLayer[i] == layer) {
            int a = groupLower[i];
            int b = layer - a;
            int size = groupSize[i];
            // 只有 a - 1 这一组（它是本层紧邻的前一组）连过来的边才属于本组
            int previous = (previousLower == a - 1) ? previousAmount : 0;
            if (b == a + 1) {
                // 本组顶点的第二条出路是在本组内部连一条边（可以连自己），
                // 全组都走这条路需要用 ceil(size / 2) 条组内边
                int allInternal = (size + 1) / 2;
                if (a == 0) {
                    // 只剩 1 号顶点且 distOdd = 1，只能给自己加一个自环
                    answer += allInternal;
                } else if (groupExists(layer - 2, a - 1)) {
                    // type A 边可用：已有向上边的 previous 个顶点只需再补一条组内边，
                    // 其余顶点各用一条 type A 边；把它和"全用组内边"取更优
                    int withPrevious = (size - previous) + (previous + 1) / 2;
                    answer += withPrevious > allInternal ? withPrevious : allInternal;
                } else {
                    // type A 边不可用，整组都必须靠组内边完成，缺的向上边要补
                    if (previous < size) answer += size - previous;
                    answer += allInternal;
                }
                previousAmount = 0;  // 本组下面已经没有同层的组了
            } else {
                // b > a + 1：先给本组每个顶点连一条边到下一组 s(a + 1, b - 1)，
                // 这条边会成为下一组的"向上边"，一共 size 条
                answer += size;
                if (a == 0) {
                    // 1 号顶点连一条边到 s(1, b - 1)，同样是给下一组当向上边
                    previousAmount = size;
                } else if (groupExists(layer - 2, a - 1)) {
                    // type A 边可用：有向上边的顶点改走 type B 更划算（边数不变，
                    // 还能多给下面一组一条边），所以把向上边尽量用掉
                    previousAmount = size < previous ? size : previous;
                } else {
                    // type A 边不可用：每个顶点都必须有向上边，缺的补上
                    if (previous < size) answer += size - previous;
                    previousAmount = size;
                }
            }
            previousLower = a;
            ++i;
        }
    }
    return answer;
}

int main() {
    int testCount;
    scanf("%d", &testCount);
    for (int t = 0; t < testCount; ++t) {
        scanf("%d%d", &n, &m);
        for (int v = 0; v < n; ++v) adjacency[v].clear();
        for (int e = 0; e < m; ++e) {
            int x, y;
            scanf("%d%d", &x, &y);
            --x;
            --y;
            adjacency[x].push_back(y);
            adjacency[y].push_back(x);
        }
        bfsDistances();
        if (distOdd[0] == INF) {
            // 二分图：任何等价的图都不能有奇环，最少就是一棵生成树
            printf("%d\n", n - 1);
            continue;
        }
        buildGroups();
        printf("%d\n", minimumEdges());
    }
    return 0;
}
