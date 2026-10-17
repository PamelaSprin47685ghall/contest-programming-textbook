#include <cstdio>
#include <vector>

using namespace std;

const int M = 102;  // 坐标范围 0..100, 留一圈边界便于外部连通.

const int DX[4] = {1, 0, -1, 0};
const int DY[4] = {0, 1, 0, -1};

// 网格: ' ' 空地, 'X' 图案, 'A' 已从外部访问到的空地.
struct Board {
    vector<vector<char>> data;

    void Read() {
        data.assign(M, vector<char>(M, ' '));
        int n;
        scanf("%d", &n);
        for (int i = 0; i < n; ++i) {
            int x, y;
            scanf("%d%d", &x, &y);
            data[x][y] = 'X';
        }
    }

    // 统计图案与外部空地的交界边数, 即图案的周长.
    int CountBorders() {
        int answer = 0;
        auto isDifferent = [](char a, char b) {
            // 只有 'A' (外部可达) 与 'X' (图案) 交界才算边界.
            return (a == 'A') != (b == 'A');
        };
        for (int x = 0; x < M; ++x) {
            for (int y = 0; y < M; ++y) {
                if (x != 0 && isDifferent(data[x - 1][y], data[x][y])) ++answer;
                if (y != 0 && isDifferent(data[x][y - 1], data[x][y])) ++answer;
            }
        }
        return answer;
    }
};

// 从边界出发标记所有外部可达的空地.
struct DFS {
    Board* board;

    void Compute(int x, int y) {
        if (x < 0 || M <= x) return;
        if (y < 0 || M <= y) return;
        if (board->data[x][y] != ' ') return;
        board->data[x][y] = 'A';
        for (int i = 0; i < 4; ++i) {
            Compute(x + DX[i], y + DY[i]);
        }
    }
};

int main() {
    Board board;
    board.Read();
    DFS dfs;
    dfs.board = &board;
    // 从四条边出发, 把所有能到达的外部空地标记出来.
    for (int i = 0; i < M; ++i) {
        dfs.Compute(0, i);
        dfs.Compute(M - 1, i);
        dfs.Compute(i, 0);
        dfs.Compute(i, M - 1);
    }
    printf("%d\n", board.CountBorders());
    return 0;
}