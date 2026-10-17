#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <vector>

using namespace std;

int main() {
    int n;
    scanf("%d", &n);
    vector<int> x(n), y(n), suffix(n);
    for (int i = 0; i < n; ++i) scanf("%d%d", &x[i], &y[i]);
    // 跳过第 i 个检查点时，i 之后各相邻点距离之和，从右往左递推。
    for (int i = n - 2; i >= 0; --i) {
        suffix[i] = abs(x[i] - x[i + 1]) + abs(y[i] - y[i + 1]) + suffix[i + 1];
    }
    int answer = numeric_limits<int>::max();
    int prefix = 0;  // i 之前各相邻点距离之和。
    for (int i = 1; i < n - 1; ++i) {
        // 跳过 i 时，用 i-1 与 i+1 的直连距离替代两段相邻距离。
        int candidate = prefix + abs(x[i - 1] - x[i + 1]) +
                        abs(y[i - 1] - y[i + 1]) + suffix[i + 1];
        answer = min(answer, candidate);
        prefix += abs(x[i - 1] - x[i]) + abs(y[i - 1] - y[i]);
    }
    printf("%d\n", answer);
    return 0;
}
