#include <cstdio>
#include <vector>

using namespace std;

const int MAXM = 200000;  // 列数上限
const int MAXQ = 200000;  // 询问数上限
const int MAXPIECE = MAXM + 5;

int rowCount, columnCount;
long long downCost[MAXM + 1];  // c_y：在第 y 列向下走一格的代价（下标 0 基）

int queryCount;

struct Query {
    int row;    // 目标行
    int order;  // 原输入中的编号
};

// 询问按目标列分组：答案函数是从左到右逐列递推的，走到哪一列就顺手回答那一列的询问
vector<Query> queriesAt[MAXM + 1];
long long answer[MAXQ + 1];

// ans_y(x) 关于 x 是下凸的分段函数，栈里每段记三个数：
//   这一段的最优走法是"先在 startColumn 列一路下到第 x 行，再向右走到当前列"
//   于是它的取值是 (当前列−startColumn)·x² + c[startColumn]·x + base
int pieceRight[MAXPIECE];       // 这一段的右端点（行号）
int pieceStart[MAXPIECE];       // 这一段的起跳列（0 基）
long long pieceBase[MAXPIECE];  // 这一段的常数项
int pieceCount;
int currentColumn;  // 当前处理到第几列（0 基）

// 用"起跳列为 startColumn 的那条折线"求当前列下第 row 行的答案
long long evaluatePiece(long long row, int startColumn, long long base) {
    long long extraColumns = currentColumn - startColumn;
    return extraColumns * row * row + row * downCost[startColumn] + base;
}

// 在折线的一段内找出第一个行号 t，使"先在新列下一行（代价 c）再沿折线走"更划算。
// 找不到就返回 hi+1，表示这一段整段保留。
int firstBetterRow(int lo, int hi, int startColumn, long long base, long long columnCost) {
    if (hi < lo) return lo;
    ++hi;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        long long throughNewColumn = evaluatePiece(mid, startColumn, base) + columnCost;
        long long keepOldPiece = evaluatePiece(mid + 1, startColumn, base);
        if (throughNewColumn < keepOldPiece) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

// 求栈里第一个右端点不小于 row 的分段，也就是覆盖第 row 行的那一段
int findPiece(long long row) {
    int lo = 0, hi = pieceCount - 1;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (pieceRight[mid] >= row) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

int main() {
    scanf("%d%d", &rowCount, &columnCount);
    for (int column = 0; column < columnCount; ++column) {
        scanf("%lld", &downCost[column]);
    }
    scanf("%d", &queryCount);
    for (int i = 0; i < queryCount; ++i) {
        int row, column;
        scanf("%d%d", &row, &column);
        Query q;
        q.row = row;
        q.order = i;
        queriesAt[column - 1].push_back(q);
    }

    pieceCount = 0;
    for (int column = 0; column < columnCount; ++column) {
        currentColumn = column;
        if (column == 0) {
            // 第 1 列只能一路往下：ans_1(x) = (x−1)·c_1
            pieceCount = 0;
            pieceRight[0] = 1;
            pieceStart[0] = 0;
            pieceBase[0] = -downCost[0];
            ++pieceCount;
            pieceRight[1] = rowCount;
            pieceStart[1] = 0;
            pieceBase[1] = -downCost[0];
            ++pieceCount;
        } else {
            // 换了新列以后，每一段都相当于自动多加了一次 x²（见 evaluatePiece），
            // 还需要补上"从上一行在新列下一行再走过来"这条更便宜的可能。
            // 从栈顶往下检查：整段都不如新直线的段落直接弹掉。
            while (pieceCount > 1) {
                long long boundary = pieceRight[pieceCount - 2];
                int start = pieceStart[pieceCount - 1];
                long long base = pieceBase[pieceCount - 1];
                long long here = evaluatePiece(boundary, start, base);
                long long oneMoreRow = evaluatePiece(boundary + 1, start, base);
                if (here + downCost[column] < oneMoreRow) {
                    --pieceCount;
                    continue;
                }
                // 剩下的部分里，从某个行号起由"沿新列向下"接管，截断这一段
                int left = (int)boundary + 1;
                int right = pieceRight[pieceCount - 1] - 1;
                pieceRight[pieceCount - 1] = firstBetterRow(left, right, start, base, downCost[column]);
                break;
            }
            if (pieceRight[pieceCount - 1] < rowCount) {
                // 新列接管的尾部是一条斜率为 c[column] 的直线；
                // 让它在截断处与旧折线取值相同，就定出了常数项
                long long boundary = pieceRight[pieceCount - 1];
                long long value = evaluatePiece(boundary, pieceStart[pieceCount - 1], pieceBase[pieceCount - 1]);
                pieceRight[pieceCount] = rowCount;
                pieceStart[pieceCount] = column;
                pieceBase[pieceCount] = value - boundary * downCost[column];
                ++pieceCount;
            }
        }
        for (int k = 0; k < (int)queriesAt[column].size(); ++k) {
            int row = queriesAt[column][k].row;
            int index = findPiece(row);
            answer[queriesAt[column][k].order] =
                evaluatePiece(row, pieceStart[index], pieceBase[index]);
        }
    }

    for (int i = 0; i < queryCount; ++i) {
        printf("%lld\n", answer[i]);
    }
    return 0;
}
