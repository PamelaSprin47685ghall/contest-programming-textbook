#include <algorithm>
#include <cstdio>
#include <unordered_map>
#include <vector>

using namespace std;

struct Point {
    int id, x, y;
};

// 按 y 分组, 每组内按 x 排序, 得到每个点向右走会到达的点.
// rightOf[i] = i 号点向右走到的点; 若右边没有点则为 -1.
struct RightNeighbors {
    vector<int> rightOf;

    void Compute(const vector<Point>& points,
                 const unordered_map<int, vector<Point>>& byRow) {
        rightOf.resize(points.size(), -1);
        for (const auto& row : byRow) {
            const vector<Point>& sameRow = row.second;
            vector<Point> sorted = sameRow;
            sort(sorted.begin(), sorted.end(),
                 [](const Point& a, const Point& b) { return a.x < b.x; });
            for (int i = 0; i + 1 < (int)sorted.size(); ++i) {
                rightOf[sorted[i].id] = sorted[i + 1].id;
            }
        }
    }
};

// 判断给定配对下, 从任意点出发沿"向右走, 再经虫洞跳回"是否陷入环.
// 用状态 1(正在访问) 2(已结束) 做环检测.
struct LoopChecker {
    const vector<int>* rightOf;
    const vector<int>* pair;
    vector<int> state;

    bool HasCycle(int node) {
        if (state[node] == 1) return true;
        if (state[node] == 2) return false;
        state[node] = 1;
        int right = (*rightOf)[(*pair)[node]]; // 跳到配对的点, 再向右
        if (right != -1 && HasCycle(right)) return true;
        state[node] = 2;
        return false;
    }

    bool Check() {
        state.assign(pair->size(), 0);
        for (int i = 0; i < (int)pair->size(); ++i) {
            if (HasCycle(i)) return true;
        }
        return false;
    }
};

// 枚举所有两两配对, 统计其中会形成环的配对数.
struct PairSearch {
    const vector<int>* rightOf;
    vector<int> pair;

    int Count() {
        pair.assign(rightOf->size(), -1);
        return Dfs(0);
    }

    int Dfs(int node) {
        if (node == (int)pair.size()) {
            LoopChecker checker;
            checker.rightOf = rightOf;
            checker.pair = &pair;
            return checker.Check() ? 1 : 0;
        }
        if (pair[node] != -1) return Dfs(node + 1);
        int answer = 0;
        for (int other = node + 1; other < (int)pair.size(); ++other) {
            if (pair[other] != -1) continue;
            pair[node] = other; // 把 node 和 other 配成一对虫洞
            pair[other] = node;
            answer += Dfs(node + 1);
            pair[other] = -1;
            pair[node] = -1;
        }
        return answer;
    }
};

int main() {
    int n;
    scanf("%d", &n);
    vector<Point> points(n);
    unordered_map<int, vector<Point>> byRow;
    for (int i = 0; i < n; ++i) {
        points[i].id = i;
        scanf("%d%d", &points[i].x, &points[i].y);
        byRow[points[i].y].push_back(points[i]);
    }

    RightNeighbors right;
    right.Compute(points, byRow);

    PairSearch search;
    search.rightOf = &right.rightOf;
    printf("%d\n", search.Count());
    return 0;
}