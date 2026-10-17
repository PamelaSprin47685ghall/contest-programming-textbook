#include <cstdio>
#include <string>
#include <vector>

using namespace std;

char buffer[80];

int main() {
    int r, c;
    scanf("%d%d", &r, &c);
    vector<string> board(r);
    for (int i = 0; i < r; ++i) {
        scanf("%s", buffer);
        board[i] = buffer;
    }
    // ways[i][j]：从 (i,j) 到右下角的跳法数，从右下角向左上递推。
    vector<vector<int>> ways(r);
    for (int i = 0; i < r; ++i) ways[i].resize(c);
    for (int i = r - 1; i >= 0; --i) {
        for (int j = c - 1; j >= 0; --j) {
            if (i == r - 1 && j == c - 1) ways[i][j] = 1;
            // 下一步只能跳到右下方向、且字母不同的格子。
            for (int i0 = i + 1; i0 < r; ++i0) {
                for (int j0 = j + 1; j0 < c; ++j0) {
                    if (board[i0][j0] != board[i][j]) {
                        ways[i][j] += ways[i0][j0];
                    }
                }
            }
        }
    }
    printf("%d\n", ways[0][0]);
    return 0;
}
