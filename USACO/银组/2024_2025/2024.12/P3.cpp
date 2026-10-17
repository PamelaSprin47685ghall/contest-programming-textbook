#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

// 反向处理：先把 Q 天要建的传送带全部建好，再倒着逐天撤销（把该格变回 "?"）。
// 撤销只会让"可用格子"变多，所以每步只要从刚变回 "?" 的格子出发扩散一次，
// 每个格子最多被标记一次，总复杂度 O(N^2 + Q)。
const int MAXN = 1002;

const int DR[4] = {0, 0, -1, 1}; // 依次对应 L, R, U, D
const int DC[4] = {-1, 1, 0, 0};

int N, Q;
int direction[MAXN][MAXN]; // 0=L,1=R,2=U,3=D，-1 表示 "?"
bool usable[MAXN][MAXN];
int usableTotal;
int stackCells[MAXN * MAXN]; // 手写栈，避免递归过深

int updateRow[200005];
int updateCol[200005];
int updateType[200005];

// 判断 (row, col) 能不能被建成"放上去一定出得去"的格子
bool isUsable(int row, int col) {
    if (row < 1 || row > N || col < 1 || col > N) {
        return false; // 出界的位置不是格子
    }
    if (usable[row][col]) {
        return false;
    }
    if (direction[row][col] != -1) {
        // 已建好的传送带只能朝既定方向走，必须直接出界或走到另一个可用格子
        int d = direction[row][col];
        int nr = row + DR[d], nc = col + DC[d];
        return nr < 1 || nr > N || nc < 1 || nc > N || usable[nr][nc];
    }
    // "?" 可以任选方向：四个方向里有一个出界或通向可用格子就够了
    for (int d = 0; d < 4; ++d) {
        int nr = row + DR[d], nc = col + DC[d];
        if (nr < 1 || nr > N || nc < 1 || nc > N || usable[nr][nc]) {
            return true;
        }
    }
    return false;
}

void flood(int startRow, int startCol) {
    if (!isUsable(startRow, startCol)) {
        return;
    }
    int top = 0;
    usable[startRow][startCol] = true;
    ++usableTotal;
    stackCells[top++] = startRow * (N + 1) + startCol;
    while (top > 0) {
        int cell = stackCells[--top];
        int row = cell / (N + 1), col = cell % (N + 1);
        for (int d = 0; d < 4; ++d) {
            int nr = row + DR[d], nc = col + DC[d];
            if (isUsable(nr, nc)) {
                usable[nr][nc] = true;
                ++usableTotal;
                stackCells[top++] = nr * (N + 1) + nc;
            }
        }
    }
}

int main() {
    scanf("%d %d", &N, &Q);
    memset(direction, -1, sizeof(direction));
    memset(usable, 0, sizeof(usable));
    for (int i = 0; i < Q; ++i) {
        char buffer[4];
        int row, col;
        scanf("%d %d %s", &row, &col, buffer);
        int type = 0;
        if (buffer[0] == 'R') {
            type = 1;
        } else if (buffer[0] == 'U') {
            type = 2;
        } else if (buffer[0] == 'D') {
            type = 3;
        }
        updateRow[i] = row;
        updateCol[i] = col;
        updateType[i] = type;
        direction[row][col] = type;
    }

    // 第 Q 天结束时的网格：全部格子定下来后做一次可达性标记
    for (int row = 1; row <= N; ++row) {
        for (int col = 1; col <= N; ++col) {
            flood(row, col);
        }
    }

    vector<int> answer(Q);
    for (int i = Q - 1; i >= 0; --i) {
        answer[i] = N * N - usableTotal; // 撤销第 i 天之前的配置对应的答案
        direction[updateRow[i]][updateCol[i]] = -1;
        flood(updateRow[i], updateCol[i]); // 只有这里会新增可用格子
    }
    for (int i = 0; i < Q; ++i) {
        printf("%d\n", answer[i]);
    }
    return 0;
}
