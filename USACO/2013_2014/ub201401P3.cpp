#include <algorithm>
#include <cstdio>
#include <limits>
#include <vector>

// 12 头牛分 4 组, 每组 3 头. 用排列 0,0,0,1,1,1,2,2,2,3,3,3
// 表示每头牛的分组, 枚举所有排列, 计算各组能力和的极差.
int TeamDifference(const std::vector<int>& ability,
                   const std::vector<int>& team) {
    std::vector<int> teamSum(4);
    for (int i = 0; i < 12; ++i) teamSum[team[i]] += ability[i];
    int strongest = *std::max_element(teamSum.begin(), teamSum.end());
    int weakest = *std::min_element(teamSum.begin(), teamSum.end());
    return strongest - weakest;
}

int main() {
    std::vector<int> ability(12);
    for (int& a : ability) scanf("%d", &a);

    std::vector<int> team{0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3};
    int answer = std::numeric_limits<int>::max();
    do {
        answer = std::min(answer, TeamDifference(ability, team));
    } while (std::next_permutation(team.begin(), team.end()));
    printf("%d\n", answer);
    return 0;
}