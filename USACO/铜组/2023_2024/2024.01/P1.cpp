#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

void Solve() {
    int n;
    scanf("%d", &n);
    vector<int> h(n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &h[i]);
    }

    // 某种干草能传遍全局的充要条件:
    // 存在两个相同的干草品种相邻 (距离 1), 或者中间隔恰好 1 种草 (距离 2).
    // 此时它可以在局部区间 [i, i+1] 或 [i, i+2] 中占据严格多数 (2/2 或 2/3),
    // 随后不断向外扩展覆盖整个数组!
    vector<int> answers;
    vector<bool> found(n + 1, false);

    for (int i = 0; i < n; ++i) {
        if (i + 1 < n && h[i] == h[i + 1]) {
            if (!found[h[i]]) {
                found[h[i]] = true;
                answers.push_back(h[i]);
            }
        }
        if (i + 2 < n && h[i] == h[i + 2]) {
            if (!found[h[i]]) {
                found[h[i]] = true;
                answers.push_back(h[i]);
            }
        }
    }

    if (answers.empty()) {
        printf("-1\n");
    } else {
        sort(answers.begin(), answers.end());
        for (size_t i = 0; i < answers.size(); ++i) {
            printf("%d%c", answers[i], i + 1 == answers.size() ? '\n' : ' ');
        }
    }
}

int main() {
    int t;
    if (scanf("%d", &t) == 1) {
        while (t--) {
            Solve();
        }
    }
    return 0;
}
