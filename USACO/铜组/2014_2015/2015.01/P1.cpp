#include <algorithm>
#include <cstdio>
#include <limits>
#include <vector>

using namespace std;

// 判断航线 route 上是否先出现 a 再出现 b（a 在 b 之前）。
bool canReach(int a, int b, const vector<int>& route) {
    int k = 0;
    while (k < (int)route.size() && route[k] != a) ++k;
    while (k < (int)route.size() && route[k] != b) ++k;
    return k < (int)route.size();
}

int main() {
    int a, b, n;
    scanf("%d%d%d", &a, &b, &n);
    int answer = numeric_limits<int>::max();
    for (int i = 0; i < n; ++i) {
        int cost, k;
        scanf("%d%d", &cost, &k);
        vector<int> route(k);
        for (int j = 0; j < k; ++j) scanf("%d", &route[j]);
        if (canReach(a, b, route)) answer = min(answer, cost);
    }
    if (answer == numeric_limits<int>::max()) {
        printf("-1\n");
    } else {
        printf("%d\n", answer);
    }
    return 0;
}
