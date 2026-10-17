#include <algorithm>
#include <cstdio>
#include <limits>
#include <vector>

using namespace std;

// 12 头牛分 4 组, 每组 3 头. 用排列 0,0,0,1,1,1,2,2,2,3,3,3
// 表示每头牛的分组, 枚举所有排列, 计算各组能力和的极差.
int TeamDifference(const vector<int>& ability,
                   const vector<int>& team) {
    vector<int> teamSum(4);
    for (int i = 0; i < 12; ++i) teamSum[team[i]] += ability[i];
    int strongest = *max_element(teamSum.begin(), teamSum.end());
    int weakest = *min_element(teamSum.begin(), teamSum.end());
    return strongest - weakest;
}

int main() {
    vector<int> ability(12);
    for (int& a : ability) scanf("%d", &a);

    vector<int> team{0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3};
    int answer = numeric_limits<int>::max();
    do {
        answer = min(answer, TeamDifference(ability, team));
    } while (next_permutation(team.begin(), team.end()));
    printf("%d\n", answer);
    return 0;
}