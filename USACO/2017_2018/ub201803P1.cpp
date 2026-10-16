#include <cstdio>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

// 棋盘 9 个格子的所有获胜连线(行、列、对角线).
std::vector<std::vector<int>> Lines{
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
    {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
    {0, 4, 8}, {2, 4, 6},
};

int main() {
    // 读入 9 个格子的棋子字符.
    char cell[9];
    for (int i = 0; i < 9; ++i) scanf(" %c", &cell[i]);
    // answer[1] 存单人获胜的棋子, answer[2] 存双人组获胜的(排序后的)棋子串.
    std::unordered_set<std::string> answer[3];
    for (const std::vector<int>& line : Lines) {
        std::set<char> players;
        for (int pos : line) players.insert(cell[pos]);
        // 一条线只有 1 或 2 种棋子才算获胜; 用排序后的字符串做去重键.
        if (players.size() == 1 || players.size() == 2) {
            std::string key(players.begin(), players.end());
            answer[players.size()].insert(key);
        }
    }
    printf("%d\n", (int)answer[1].size());
    printf("%d\n", (int)answer[2].size());
    return 0;
}