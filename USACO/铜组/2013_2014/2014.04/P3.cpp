#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

struct Graph {
    vector<vector<int>> out;

    void Read() {
        int n, m;
        scanf("%d%d", &n, &m);
        out.resize(n);
        for (int i = 0; i < m; ++i) {
            int a, b;
            scanf("%d%d", &a, &b);
            out[a - 1].push_back(b - 1);
            out[b - 1].push_back(a - 1);
        }
    }
};

// 对每个连通分量做二染色. 若某条边两端同色则无法二染色(返回 -1),
// 否则取该分量中两种颜色数量较多者, 作为能装饰的牛数.
struct Bipartite {
    const Graph* graph;
    vector<int> color; // 0=未染色, 1/2=两种颜色
    int count[2];

    bool TryColor(int node, int c) {
        if (color[node] == c) return true;
        if (color[node] != 0) return false;
        color[node] = c;
        ++count[c - 1];
        for (int next : graph->out[node]) {
            if (!TryColor(next, 3 - c)) return false;
        }
        return true;
    }

    int Solve() {
        color.assign(graph->out.size(), 0);
        int answer = 0;
        for (int i = 0; i < (int)graph->out.size(); ++i) {
            count[0] = count[1] = 0;
            if (color[i] == 0 && !TryColor(i, 1)) return -1;
            answer += max(count[0], count[1]);
        }
        return answer;
    }
};

int main() {
    Graph graph;
    graph.Read();
    Bipartite solver;
    solver.graph = &graph;
    printf("%d\n", solver.Solve());
    return 0;
}