#include <algorithm>
#include <cstdio>
#include <vector>

const int MAX_TIME = 1000;

int main() {
    int n;
    scanf("%d", &n);
    std::vector<int> begin(n), end(n);
    for (int i = 0; i < n; ++i) scanf("%d%d", &begin[i], &end[i]);
    // 差分数组: 在 begin 处加一, 在 end 处减一.
    std::vector<int> cover(MAX_TIME + 1, 0);
    for (int i = 0; i < n; ++i) {
        ++cover[begin[i]];
        --cover[end[i]];
    }
    // 前缀和还原每个时刻被几个救生员覆盖.
    for (int t = 1; t <= MAX_TIME; ++t) cover[t] += cover[t - 1];
    // s1[t]: 前 t 时刻中恰好被 1 人覆盖的时长(去掉此人会丢).
    // s2[t]: 前 t 时刻中至少被 2 人覆盖的时长(去掉任何一人都不丢).
    // 注意 cover[t-1] 表示时刻 t 的覆盖数, 因为差分端点错开一位.
    std::vector<int> single(MAX_TIME + 1, 0), doubleCover(MAX_TIME + 1, 0);
    for (int t = 1; t <= MAX_TIME; ++t) {
        single[t] = single[t - 1] + (cover[t - 1] == 1);
        doubleCover[t] = doubleCover[t - 1] + (2 <= cover[t - 1]);
    }
    // 全部在岗时的总覆盖时长.
    int total = single[MAX_TIME] + doubleCover[MAX_TIME];
    // 开除某人会损失其独享区间, 找损失最小的人.
    int minLost = total;
    for (int i = 0; i < n; ++i) {
        minLost = std::min(minLost, single[end[i]] - single[begin[i]]);
    }
    printf("%d\n", total - minLost);
    return 0;
}