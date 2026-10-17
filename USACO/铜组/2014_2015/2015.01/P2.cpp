#include <algorithm>
#include <cstdio>
#include <limits>
#include <unordered_set>
#include <vector>

using namespace std;

// 两条航线接力：第一条上出现 a 之后的所有城市，第二条上在 b 之前出现即可。
bool canReach(int a, int b, const vector<int>& route0,
              const vector<int>& route1) {
    unordered_set<int> afterA;
    int k0 = 0;
    while (k0 < (int)route0.size() && route0[k0] != a) ++k0;
    while (k0 < (int)route0.size()) afterA.insert(route0[k0++]);
    int k1 = (int)route1.size() - 1;
    while (k1 >= 0 && route1[k1] != b) --k1;
    while (k1 >= 0) {
        if (afterA.count(route1[k1--])) return true;
    }
    return false;
}

int main() {
    int a, b, n;
    scanf("%d%d%d", &a, &b, &n);
    vector<vector<int>> route(n);
    vector<int> cost(n);
    for (int i = 0; i < n; ++i) {
        int k;
        scanf("%d%d", &cost[i], &k);
        route[i].resize(k);
        for (int j = 0; j < k; ++j) scanf("%d", &route[i][j]);
    }
    int answer = numeric_limits<int>::max();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (canReach(a, b, route[i], route[j])) {
                int c = (i == j) ? cost[i] : cost[i] + cost[j];
                answer = min(answer, c);
            }
        }
    }
    if (answer == numeric_limits<int>::max()) {
        printf("-1\n");
    } else {
        printf("%d\n", answer);
    }
    return 0;
}
