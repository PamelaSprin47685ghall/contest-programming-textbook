#include <cstdio>
#include <utility>
#include <vector>

struct Edge {
    int to;
    char relation;  // 'S' 或 'D'.
};

struct Graph {
    int n;
    std::vector<std::vector<Edge>> out;
    std::vector<bool> hasDegree;  // 是否出现在任何约束里.

    void Read() {
        int k;
        scanf("%d%d", &n, &k);
        out.resize(n);
        hasDegree.assign(n, false);
        for (int i = 0; i < k; ++i) {
            char relation;
            int v0, v1;
            scanf(" %c%d%d", &relation, &v0, &v1);
            // 约束只挂在编号较大的牛身上, 保证按牛编号顺序决策即可检查.
            if (v1 < v0) std::swap(v0, v1);
            out[v1 - 1].push_back(Edge{v0 - 1, relation});
            hasDegree[v0 - 1] = true;
            hasDegree[v1 - 1] = true;
        }
    }
};

// 按编号顺序给每头牛定品种 (0/1/2), 遇到约束时检查是否满足.
struct DFS {
    const Graph* graph;
    std::vector<int> color;

    int Put(int no) {
        if (no == graph->n) return 1;
        if (!graph->hasDegree[no]) {
            // 无约束的牛有 3 种选择, 且互不影响, 乘 3 即可.
            return Put(no + 1) * 3;
        }
        int answer = 0;
        for (int c = 0; c < 3; ++c) answer += PutWithColor(no, c);
        return answer;
    }

    int PutWithColor(int no, int c) {
        // 检查所有指向更小编号的约束: 同色必须 S, 异色必须 D.
        for (const Edge& e : graph->out[no]) {
            char required = (color[e.to] == c) ? 'S' : 'D';
            if (e.relation != required) return 0;
        }
        color[no] = c;
        return Put(no + 1);
    }
};

int main() {
    Graph graph;
    graph.Read();
    DFS dfs;
    dfs.graph = &graph;
    dfs.color.resize(graph.n);
    printf("%d\n", dfs.Put(0));
    return 0;
}