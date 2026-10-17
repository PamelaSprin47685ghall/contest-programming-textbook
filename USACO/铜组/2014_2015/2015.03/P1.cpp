#include <algorithm>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

const int DX[8] = {1, 1, 0, -1, -1, -1, 0, 1};
const int DY[8] = {0, 1, 1, 1, 0, -1, -1, -1};

char buffer[80];

struct Board {
    int n;
    int m;
    vector<string> data;

    void read() {
        scanf("%d%d", &n, &m);
        data.resize(n);
        for (int i = 0; i < n; ++i) {
            scanf("%s", buffer);
            data[i] = buffer;
        }
    }
};

int main() {
    Board board;
    board.read();
    // 统计每种 (第一字母, 第二字母) 组合作为连续三格出现的次数。
    unordered_map<string, int> counts;
    auto inside = [&](int x, int y) {
        return 0 <= x && x < board.n && 0 <= y && y < board.m;
    };
    for (int i = 0; i < board.n; ++i) {
        for (int j = 0; j < board.m; ++j) {
            for (int k = 0; k < 8; ++k) {
                int x1 = i + DX[k], y1 = j + DY[k];
                if (!inside(x1, y1)) continue;
                if (board.data[i][j] == board.data[x1][y1]) continue;
                int x2 = x1 + DX[k], y2 = y1 + DY[k];
                if (!inside(x2, y2)) continue;
                if (board.data[x1][y1] != board.data[x2][y2]) continue;
                // M 与 O 不能是同一字母，否则不构成 "MOO"。
                if (board.data[i][j] == 'M' || board.data[x1][y1] == 'O') continue;
                string key;
                key += board.data[i][j];
                key += board.data[x1][y1];
                ++counts[key];
            }
        }
    }
    int answer = 0;
    for (const auto& entry : counts) answer = max(answer, entry.second);
    printf("%d\n", answer);
    return 0;
}
