#include <cstdio>

using namespace std;

const int MAXN = 25;
const int NUM_BOARDS = 19683;   // 棋盘共 3^9 种状态，每位数字 0=空、1='M'、2='O'

int n;
int kind[MAXN][MAXN];           // 0=墙，1=空地，2=写有走法的格子
int moveIdx[MAXN][MAXN];        // 走法作用的棋盘格子编号 0..8（按行优先）
int movePiece[MAXN][MAXN];      // 走法落下的棋子：1='M'，2='O'
int pow3[10];

char seen[MAXN][MAXN][NUM_BOARDS];  // 状态 (位置, 棋盘) 是否访问过
char winBoard[NUM_BOARDS];          // 是否已经生成了该获胜棋盘
int queueState[MAXN * MAXN * NUM_BOARDS];   // BFS 队列，把 (位置, 棋盘) 打包成一个整数

const int lineCell[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8},    // 三行
    {0, 3, 6}, {1, 4, 7}, {2, 5, 8},    // 三列
    {0, 4, 8}, {2, 4, 6}                // 两条对角线
};

// 判断棋盘上是否已经拼出 'MOO'（正着反着都算）
bool isWin(int board) {
    int cell[9];
    for (int i = 0; i < 9; ++i) {
        cell[i] = board % 3;
        board /= 3;
    }
    for (int t = 0; t < 8; ++t) {
        int a = cell[lineCell[t][0]], b = cell[lineCell[t][1]], c = cell[lineCell[t][2]];
        if (a == 1 && b == 2 && c == 2) return true;    // MOO
        if (a == 2 && b == 2 && c == 1) return true;    // OOM
    }
    return false;
}

int main() {
    pow3[0] = 1;
    for (int i = 1; i <= 9; ++i) pow3[i] = pow3[i - 1] * 3;

    scanf("%d", &n);
    char row[3 * MAXN + 1];
    int startR = 0, startC = 0;
    for (int i = 0; i < n; ++i) {
        scanf("%s", row);
        for (int j = 0; j < n; ++j) {
            char a = row[3 * j], b = row[3 * j + 1], c = row[3 * j + 2];
            if (a == '#') {
                kind[i][j] = 0;
            } else if (a == '.') {
                kind[i][j] = 1;
            } else if (a == 'B') {
                kind[i][j] = 1;
                startR = i;
                startC = j;
            } else {
                kind[i][j] = 2;
                movePiece[i][j] = (a == 'M' ? 1 : 2);
                moveIdx[i][j] = (b - '1') * 3 + (c - '1');
            }
        }
    }

    // 状态 = 贝茜站在某格、且该格纸上的走法已经生效之后的棋盘
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};
    int head = 0, tail = 0;
    seen[startR][startC][0] = 1;
    queueState[tail++] = (startR * MAXN + startC) * NUM_BOARDS;
    while (head < tail) {
        int state = queueState[head++];
        int board = state % NUM_BOARDS;
        int pos = state / NUM_BOARDS;
        int r = pos / MAXN, c = pos % MAXN;
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;   // 边界全是干草堆
            if (kind[nr][nc] == 0) continue;
            int nb = board;
            if (kind[nr][nc] == 2) {
                int idx = moveIdx[nr][nc];
                // 目标格已有棋子时该走法无效，但仍可以走进这个格子
                if ((board / pow3[idx]) % 3 == 0) {
                    nb = board + movePiece[nr][nc] * pow3[idx];
                    if (isWin(nb)) {
                        // 一旦获胜贝茜就停止游戏，不能再从这一步继续扩展
                        winBoard[nb] = 1;
                        continue;
                    }
                }
            }
            if (seen[nr][nc][nb]) continue;
            seen[nr][nc][nb] = 1;
            queueState[tail++] = (nr * MAXN + nc) * NUM_BOARDS + nb;
        }
    }

    int answer = 0;
    for (int b = 0; b < NUM_BOARDS; ++b) {
        if (winBoard[b]) ++answer;
    }
    printf("%d\n", answer);
    return 0;
}
