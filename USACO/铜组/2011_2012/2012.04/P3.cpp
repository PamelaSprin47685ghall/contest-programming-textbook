#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

// 一个位置及其高度.
struct Position {
    int height;
    int index;
};

int main() {
    int n;
    scanf("%d", &n);
    vector<Position> positions(n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &positions[i].height);
        positions[i].index = i;
    }

    // 从高到低排序, 水从低处开始淹没, 逐高度处理.
    auto higher = [](const Position& a, const Position& b) {
        return b.height < a.height;  // 高度大的排前面
    };
    sort(positions.begin(), positions.end(), higher);

    // visited[i] 表示位置 i 是否已被水淹没 (左右各留一个哨兵).
    vector<char> visited(n + 2, false);
    int islands = 0;  // 当前水面下的岛屿段数
    int answer = 0;

    // 淹没位置 index 后更新段数: 左右邻居都淹没则段数减一, 都未淹没则段数加一.
    auto flood = [&](int index) {
        visited[index + 1] = true;
        islands -= visited[index] + visited[index + 2] - 1;
    };

    // 相同高度的位置必须同时淹没, 淹没完一批再统计段数.
    for (int i = 0; i < n; ) {
        int j = i;
        while (j < n && positions[j].height == positions[i].height) ++j;
        for (int k = i; k < j; ++k) flood(positions[k].index);
        answer = max(answer, islands);
        i = j;
    }

    printf("%d\n", answer);
    return 0;
}