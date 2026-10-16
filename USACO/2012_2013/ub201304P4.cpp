#include <cstdlib>
#include <cstdio>
#include <limits>
#include <utility>
#include <vector>

// 图: 只保留编号更小的邻居, 便于按编号顺序放置时增量计算代价.
struct Graph {
    std::vector<std::vector<int>> out;

    void Read() {
        int n;
        scanf("%d", &n);
        out.resize(n);
        for (int i = 0; i < n; ++i) {
            int v0, v1, v2;
            scanf("%d%d%d", &v0, &v1, &v2);
            if (--v0 < i) out[i].push_back(v0);
            if (--v1 < i) out[i].push_back(v1);
            if (--v2 < i) out[i].push_back(v2);
        }
    }
};

// 枚举牛的排列 (每头牛在围栏里的顺序), 用对称性和最优性剪枝.
struct DFS {
    const Graph* graph;
    int answer = std::numeric_limits<int>::max();
    std::vector<int> position;  // position[i] = 第 i 头牛在排列中的位置.

    void Compute(int no, int cost) {
        if (answer <= cost) return;
        if (no == (int)graph->out.size()) {
            answer = cost;
            return;
        }
        for (int i = no; i < (int)graph->out.size(); ++i) {
            std::swap(position[no], position[i]);
            int newCost = cost;
            // 与已放置的邻居连线: 每根线穿过 (距离-1) 头牛.
            for (int j : graph->out[no]) {
                newCost += std::abs(position[j] - position[no]) - 1;
            }
            // 左右对称只搜一半: 固定第一头牛在排列的前半.
            if (position[0] * 2 < (int)graph->out.size()) {
                Compute(no + 1, newCost);
            }
            std::swap(position[no], position[i]);
        }
    }
};

int main() {
    Graph graph;
    graph.Read();
    DFS dfs;
    dfs.graph = &graph;
    dfs.position.resize(graph.out.size());
    for (int i = 0; i < (int)graph.out.size(); ++i) dfs.position[i] = i;
    dfs.Compute(0, 0);
    // DFS 里每条边只算了 (距离-1), 即穿过其他牛的线数;
    // 每条边本身还有 1 根线, 每头牛有 3 个朋友, 共 3n/2 条, 故补回 n/2*3.
    printf("%d\n", dfs.answer + (int)graph.out.size() / 2 * 3);
    return 0;
}