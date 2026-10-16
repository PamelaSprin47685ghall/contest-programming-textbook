#include <algorithm>
#include <cstdio>
#include <vector>

// 传球关系图. 每头牛只把球传给最近的牛(平局传左边).
// 用 Out 记录每头牛把球传给谁; 传球过程中把已传过的边置 -1 表示已处理.
struct Graph {
    std::vector<int> Out;
    std::vector<int> InDegree;

    void ComputeInDegree() {
        InDegree.assign(Out.size(), 0);
        for (int from : Out) ++InDegree[from];
    }

    // 从 no 出发沿传球链走到底, 返回这条链是否真的传了球.
    int Push(int no) {
        if (Out[no] == -1) return 0;
        while (Out[no] != -1) {
            int next = Out[no];
            Out[no] = -1;  // 标记已处理, 避免重复走同一条链.
            no = next;
        }
        return 1;
    }

    int PushAll() {
        int balls = 0;
        // 先给没有入度的牛发球: 它必须自己拿一个球.
        for (int i = 0; i < (int)Out.size(); ++i) {
            if (InDegree[i] == 0) balls += Push(i);
        }
        // 剩下的每个连通分支是一个环, 每个环还需要一个球.
        for (int i = 0; i < (int)Out.size(); ++i) balls += Push(i);
        return balls;
    }
};

int main() {
    int n;
    scanf("%d", &n);
    std::vector<int> position(n);
    for (int i = 0; i < n; ++i) scanf("%d", &position[i]);
    std::sort(position.begin(), position.end());
    // 判断位置 no 的牛把球传给右边还是左边.
    auto passRight = [&](int no) {
        if (no == 0) return true;
        if (no == n - 1) return false;
        return position[no + 1] - position[no] < position[no] - position[no - 1];
    };
    Graph graph;
    graph.Out.resize(n);
    for (int i = 0; i < n; ++i) {
        graph.Out[i] = passRight(i) ? i + 1 : i - 1;
    }
    graph.ComputeInDegree();
    printf("%d\n", graph.PushAll());
    return 0;
}