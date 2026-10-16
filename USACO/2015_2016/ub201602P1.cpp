#include <cstdio>
#include <vector>

// 完全背包：每种桶可以反复使用，can[j] 表示能否恰好装 j 加仑。
// 从大到小不行，这里从小到大遍历，允许同一桶用多次。
int main() {
    int pail0, pail1, m;
    scanf("%d%d%d", &pail0, &pail1, &m);
    int pails[2] = {pail0, pail1};

    std::vector<char> can(m + 1);
    can[0] = true;
    for (int i = 0; i < 2; ++i) {
        for (int j = pails[i]; j <= m; ++j) {
            can[j] = can[j] || can[j - pails[i]];
        }
    }

    // 从 m 往下找第一个能装出的量
    int answer = m;
    while (!can[answer]) {
        --answer;
    }
    printf("%d\n", answer);
    return 0;
}