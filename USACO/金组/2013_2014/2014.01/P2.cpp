#include <cstdio>
#include <algorithm>
#include <queue>

using namespace std;

const int MAXDIM = 105;

int rows, cols;
char design[MAXDIM][MAXDIM];

// 判定"边长 B 是否可行"时用的工作数组
bool stamped[MAXDIM][MAXDIM];     // 该格子是否已被更晚的印章盖过(内容随意)
int prefFree[MAXDIM][MAXDIM + 1];  // 每行中"还没被盖过"的格子数前缀和
int prefFreeR[MAXDIM][MAXDIM + 1];  // 每行中"还没被盖过且要求是 R"的格子数前缀和
bool dirty[MAXDIM];               // 该行的前缀和是否过期
int freeLeft;                     // 还没被盖过的格子总数

void RebuildRow(int r) {
    prefFree[r][0] = 0;
    prefFreeR[r][0] = 0;
    for (int c = 0; c < cols; c++) {
        int isFree = stamped[r][c] ? 0 : 1;
        int isFreeR = (!stamped[r][c] && design[r][c] == 'R') ? 1 : 0;
        prefFree[r][c + 1] = prefFree[r][c] + isFree;
        prefFreeR[r][c + 1] = prefFreeR[r][c] + isFreeR;
    }
}

// 判断边长 B 的印章能否铺满整块地. 倒着想: 最后盖下去的那一方里每个格子都
// 不能被更晚的印章改掉, 所以它必须是同一颜色; 去掉这一方(里面的格子内容
// 从此随意), 再问有没有新的同色 B x B 方块, 反复直到全被盖过.
bool Feasible(int b) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) stamped[r][c] = false;
        dirty[r] = false;
        RebuildRow(r);
    }
    freeLeft = rows * cols;

    // 只有和刚盖下的方块相交的方块, 才可能从"盖不了"变成"能盖", 所以只要
    // 把这些方块排进队列重查即可, 不必每轮扫全图.
    queue<int> todo;
    for (int r = 0; r + b <= rows; r++) {
        for (int c = 0; c + b <= cols; c++) todo.push(r * cols + c);
    }
    while (!todo.empty()) {
        int pos = todo.front();
        todo.pop();
        int r = pos / cols;
        int c = pos % cols;
        for (int i = r; i < r + b; i++) {
            if (dirty[i]) {
                RebuildRow(i);
                dirty[i] = false;
            }
        }
        int freeCnt = 0;
        int freeR = 0;
        for (int i = r; i < r + b; i++) {
            freeCnt += prefFree[i][c + b] - prefFree[i][c];
            freeR += prefFreeR[i][c + b] - prefFreeR[i][c];
        }
        if (freeCnt == 0) continue;  // 整块早已被盖过, 再盖一次没有意义
        if (freeR != 0 && freeR != freeCnt) continue;  // R 和 S 混杂, 盖不出统一颜色

        // 这一块可以盖: 里面的格子变成"内容随意", 对两种颜色的印章都兼容.
        for (int i = r; i < r + b; i++) {
            for (int j = c; j < c + b; j++) {
                if (!stamped[i][j]) {
                    stamped[i][j] = true;
                    freeLeft--;
                    dirty[i] = true;
                }
            }
        }
        if (freeLeft == 0) return true;

        int rLow = max(0, r - b + 1);
        int rHigh = min(rows - b, r + b - 1);
        int cLow = max(0, c - b + 1);
        int cHigh = min(cols - b, c + b - 1);
        for (int rr = rLow; rr <= rHigh; rr++) {
            for (int cc = cLow; cc <= cHigh; cc++) todo.push(rr * cols + cc);
        }
    }
    return freeLeft == 0;
}

int main() {
    char buf[MAXDIM];
    scanf("%d %d", &rows, &cols);
    for (int r = 0; r < rows; r++) {
        scanf("%s", buf);
        for (int c = 0; c < cols; c++) design[r][c] = buf[c];
    }

    // B 可行则 B-1 也一定可行: 把每个 B x B 印章换成它内部的一小块 B-1 边长
    // 的印章, 覆盖范围只会变小不会变大, 而"颜色统一"这个条件仍然满足.
    // 单调性有了, 就可以对 B 二分.
    int lo = 1;
    int hi = min(rows, cols);
    while (lo < hi) {
        int mid = (lo + hi + 1) / 2;
        if (Feasible(mid)) lo = mid;
        else hi = mid - 1;
    }
    printf("%d\n", lo);
    return 0;
}
