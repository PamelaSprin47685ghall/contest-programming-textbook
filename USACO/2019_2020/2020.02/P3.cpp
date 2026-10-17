#include <algorithm>
#include <cstdio>
#include <vector>

int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    int a1, a2, b1, b2;
    scanf("%d%d%d%d", &a1, &a2, &b1, &b2);

    // 1. 求出单轮操作后, 处于位置 i 的牛是哪头牛
    std::vector<int> perm(n + 1);
    for (int i = 1; i <= n; ++i) perm[i] = i;

    std::reverse(perm.begin() + a1, perm.begin() + a2 + 1);
    std::reverse(perm.begin() + b1, perm.begin() + b2 + 1);

    // targetPos[c]: 编号为 c 的牛在 1 轮后所去往的新位置
    std::vector<int> targetPos(n + 1);
    for (int i = 1; i <= n; ++i) {
        targetPos[perm[i]] = i;
    }

    // 2. 将排列分解为置换环, 对每个环利用周期对 k 取模
    std::vector<int> answer(n + 1);
    std::vector<bool> visited(n + 1, false);

    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            std::vector<int> cycle;
            int curr = i;
            while (!visited[curr]) {
                visited[curr] = true;
                cycle.push_back(curr);
                curr = targetPos[curr];
            }

            int cycleLen = cycle.size();
            int shift = k % cycleLen;
            for (int idx = 0; idx < cycleLen; ++idx) {
                int finalPos = cycle[(idx + shift) % cycleLen];
                answer[finalPos] = cycle[idx];
            }
        }
    }

    for (int i = 1; i <= n; ++i) {
        printf("%d\n", answer[i]);
    }
    return 0;
}
