#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

struct Move {
    int type;
    int a;
    int b;
    int c;
};

int rowCount, columnCount;
vector<string> row;
vector<vector<int> > has;
vector<Move> moves;

void applyMove(int type, int a, int b, int c) {
    Move move;
    move.type = type;
    move.a = a + 1;
    move.b = b + 1;
    move.c = c + 1;
    moves.push_back(move);

    if (type == 1) {
        swap(row[a][b], row[a][c]);
    } else {
        has[a][row[a][c] - 'a']--;
        has[b][row[b][c] - 'a']--;
        swap(row[a][c], row[b][c]);
        has[a][row[a][c] - 'a']++;
        has[b][row[b][c] - 'a']++;
    }
}

void solve() {
    scanf("%d %d", &rowCount, &columnCount);
    char buffer[1005];
    scanf("%s", buffer);
    string target = buffer;

    row.assign(rowCount, "");
    for (int i = 0; i < rowCount; ++i) {
        scanf("%s", buffer);
        row[i] = buffer;
    }

    has.assign(rowCount, vector<int>(26, 0));
    for (int i = 0; i < rowCount; ++i) {
        for (int j = 0; j < columnCount; ++j) ++has[i][row[i][j] - 'a'];
    }
    moves.clear();

    // 先在第一行内部对调，能对上的位置就地解决。
    for (int i = 0; i < columnCount; ++i) {
        if (row[0][i] == target[i]) continue;
        for (int j = 0; j < columnCount; ++j) {
            if (row[0][j] == target[i] && row[0][j] != target[j]) {
                applyMove(1, 0, i, j);
                break;
            }
        }
    }

    // 第一行缺的字母，从别的行把对应列换进来。
    for (int i = 0; i < columnCount; ++i) {
        if (row[0][i] == target[i]) continue;
        for (int j = 1; j < rowCount; ++j) {
            if (!has[j][target[i] - 'a']) continue;
            int index = (int)(find(row[j].begin(), row[j].end(), target[i]) - row[j].begin());
            applyMove(1, j, i, index);
            applyMove(2, 0, j, i);
            break;
        }
    }

    printf("%d\n", (int)moves.size());
    for (int i = 0; i < (int)moves.size(); ++i) {
        printf("%d %d %d %d\n", moves[i].type, moves[i].a, moves[i].b, moves[i].c);
    }
}

int main() {
    int testCount;
    scanf("%d", &testCount);
    while (testCount--) solve();
    return 0;
}
