#include <cstdio>
#include <string>
#include <unordered_set>
#include <vector>

char buffer[80];

struct Board {
    int n;
    std::vector<std::string> data;

    void read() {
        scanf("%d", &n);
        data.resize(n);
        for (int i = 0; i < n; ++i) {
            scanf("%s", buffer);
            data[i] = buffer;
        }
    }
};

struct Paths {
    const Board* board;
    int step;  // 每次移动的步长，正着走为 1，倒着走为 -1。
    // U[i]：走到第 i 行（另一维随之确定）时能形成的所有字符串。
    std::vector<std::unordered_set<std::string>> strings;
    std::string current;
};

// 从 (x, y) 出发，每次只能向右或向下走一步，枚举所有路径上的字符串。
void enumerate(Paths& p, int remaining, int x, int y) {
    p.current += p.board->data[x][y];
    if (remaining == 1) {
        p.strings[x].insert(p.current);
    } else {
        enumerate(p, remaining - 1, x + p.step, y);
        enumerate(p, remaining - 1, x, y + p.step);
    }
    p.current.pop_back();
}

Paths search(const Board& board, int step, int x, int y) {
    Paths p;
    p.board = &board;
    p.step = step;
    p.strings.resize(board.n);
    enumerate(p, board.n, x, y);
    return p;
}

int main() {
    Board board;
    board.read();
    // 正着从左上角走，倒着从右下角走；两条路径在同一行相遇时字符串相同，
    // 说明整条路径是回文。
    Paths forward = search(board, 1, 0, 0);
    Paths backward = search(board, -1, board.n - 1, board.n - 1);
    std::unordered_set<std::string> palindromes;
    for (int i = 0; i < board.n; ++i) {
        for (const std::string& s : forward.strings[i]) {
            if (backward.strings[i].count(s)) palindromes.insert(s);
        }
    }
    printf("%d\n", (int)palindromes.size());
    return 0;
}
