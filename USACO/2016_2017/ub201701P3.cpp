#include <cstdio>
#include <string>
#include <vector>

char Buffer[80];

struct Board {
    int size;
    std::vector<std::string> grid;

    Board& Read() {
        scanf("%d", &size);
        grid.resize(size);
        for (std::string& row : grid) {
            scanf("%s", Buffer);
            row = Buffer;
        }
        return *this;
    }

    // 翻转到 (x, y) 左上角整块, 因为每次操作固定从左上角开始.
    void Flip(int x, int y) {
        for (int i = 0; i <= x; ++i) {
            for (int j = 0; j <= y; ++j) {
                grid[i][j] = (grid[i][j] == '0') ? '1' : '0';
            }
        }
    }

    // 从右下往左上处理, 每头倒下的牛必须由以它结尾的翻转救起,
    // 而它右下方的格子已定型, 不会再有操作碰到, 所以贪心正确.
    int CountFlips() {
        int answer = 0;
        for (int i = size - 1; i >= 0; --i) {
            for (int j = size - 1; j >= 0; --j) {
                if (grid[i][j] != '0') {
                    Flip(i, j);
                    ++answer;
                }
            }
        }
        return answer;
    }
};

int main() {
    printf("%d\n", Board{}.Read().CountFlips());
    return 0;
}
