#include <cstdio>
#include <string>
#include <vector>

// 枚举两个碎片的编号（第一块在前）与各自的平移偏移量，
// 若两块并集恰好拼成目标图形就输出编号。题目保证答案唯一。
char buffer[80]; // 读一行棋盘

struct Board {
    std::vector<std::string> data;
};

void readBoard(Board& board, int n) {
    board.data.resize(n);
    for (std::string& row : board.data) {
        scanf("%s", buffer);
        row = buffer;
    }
}

bool inRange(int v, int n) {
    return 0 <= v && v < n;
}

// 把第 no 个碎片平移到 (x, y) 后画到 q 上；超出棋盘的部分忽略。
void placePiece(const std::vector<Board>& pieces, int no,
                int x, int y, int n, std::vector<std::string>& q) {
    q.assign(n, std::string(n, '.'));
    for (int px = 0; px < n; ++px) {
        for (int py = 0; py < n; ++py) {
            if (pieces[no].data[px][py] == '#' &&
                inRange(x + px, n) && inRange(y + py, n)) {
                q[x + px][y + py] = '#';
            }
        }
    }
}

// 两块碎片覆盖的格子必须与目标完全一致：不多、不少、不重叠。
bool matches(const Board& target, const std::vector<std::string>& q0,
             const std::vector<std::string>& q1, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int covered = (q0[i][j] == '#') + (q1[i][j] == '#');
            int wanted = (target.data[i][j] == '#');
            if (covered != wanted) return false;
        }
    }
    return true;
}

// 递归选两块碎片：depth 是第几块，begin 是可选编号的下界，
// 保证第二块编号严格大于第一块。
void compute(const Board& target, const std::vector<Board>& pieces, int n,
             int begin, int depth, int& no0, int& no1,
             std::vector<std::string>& q0, std::vector<std::string>& q1) {
    if (depth == 2) {
        if (matches(target, q0, q1, n)) {
            printf("%d %d\n", no0 + 1, no1 + 1);
        }
        return;
    }
    for (int no = begin; no < (int)pieces.size(); ++no) {
        if (depth == 0) {
            no0 = no;
        } else {
            no1 = no;
        }
        for (int dx = 1 - n; dx <= n - 1; ++dx) {
            for (int dy = 1 - n; dy <= n - 1; ++dy) {
                if (depth == 0) {
                    placePiece(pieces, no, dx, dy, n, q0);
                } else {
                    placePiece(pieces, no, dx, dy, n, q1);
                }
                compute(target, pieces, n, no + 1, depth + 1, no0, no1, q0, q1);
            }
        }
    }
}

int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    Board target;
    readBoard(target, n);
    std::vector<Board> pieces(k);
    for (Board& p : pieces) readBoard(p, n);

    int no0 = 0, no1 = 0;
    std::vector<std::string> q0, q1;
    compute(target, pieces, n, 0, 0, no0, no1, q0, q1);
    return 0;
}