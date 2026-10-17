#include <cstdio>
#include <queue>

using namespace std;

// 所有方块坐标都在 0..9，因此每个物体的包围盒不超过 10x10
const int GRID = 10;

// 搜索时把物体 1 固定在原地，只记录物体 2、3 相对各自初始位置的偏移。
// 偏移限制在 [-20, 20]：要把一个物体推到另一个物体旁边只需要移动十格上下，
// 留一倍余量足以覆盖所有有用的中间状态，同时让状态总数有限。
const int LIMIT = 20;
const int RANGE = 2 * LIMIT + 1;                    // 单个坐标维度的取值个数
const int STATES = RANGE * RANGE * RANGE * RANGE;   // 状态总数上界，约 2.8x10^6

int squareCount[3];
int boxWidth[3], boxHeight[3];
int boxX[3], boxY[3];                        // 各物体初始包围盒的左下角
bool filled[3][GRID][GRID];                  // 方块相对包围盒左下角的位置
int cellX[3][GRID * GRID], cellY[3][GRID * GRID];   // 方块坐标列表，供重叠检测遍历

bool visited[STATES];

// 把四个偏移编码成一个下标。visited 用一维数组，编码要保证不同状态下标不同。
int encode(int off2X, int off2Y, int off3X, int off3Y) {
    return (((off2X + LIMIT) * RANGE + (off2Y + LIMIT)) * RANGE + (off3X + LIMIT)) * RANGE
           + (off3Y + LIMIT);
}

// 判断物体 i、j 在当前位置下是否有单位方块重叠。
// 包围盒在某个方向上不重叠时，一定没有方块重叠，先做这个便宜的判断。
bool overlap(int i, int xI, int yI, int j, int xJ, int yJ) {
    if (xI + boxWidth[i] <= xJ || xJ + boxWidth[j] <= xI) return false;
    if (yI + boxHeight[i] <= yJ || yJ + boxHeight[j] <= yI) return false;
    // 逐格检查时遍历方块较少的那个物体，比较次数更少
    int small = i, large = j;
    if (squareCount[small] > squareCount[large]) {
        small = j;
        large = i;
    }
    int smallX = (small == i) ? xI : xJ;
    int smallY = (small == i) ? yI : yJ;
    int largeX = (large == i) ? xI : xJ;
    int largeY = (large == i) ? yI : yJ;
    for (int k = 0; k < squareCount[small]; ++k) {
        // 把方块换算到另一个物体的包围盒坐标里，直接查表判断有没有方块
        int relX = smallX + cellX[small][k] - largeX;
        int relY = smallY + cellY[small][k] - largeY;
        if (relX < 0 || relX >= GRID || relY < 0 || relY >= GRID) continue;
        if (filled[large][relX][relY]) return true;
    }
    return false;
}

// 滑动过程中物体之间不能有方块重叠（题目要求物体始终是三个实体）
bool valid(int x1, int y1, int x2, int y2, int x3, int y3) {
    return !overlap(0, x1, y1, 1, x2, y2) && !overlap(0, x1, y1, 2, x3, y3)
           && !overlap(1, x2, y2, 2, x3, y3);
}

// 包围盒两两不相交（允许边界相贴）就是题目要的“分离”
bool separated(int x1, int y1, int x2, int y2, int x3, int y3) {
    if (x1 + boxWidth[0] > x2 && x2 + boxWidth[1] > x1 && y1 + boxHeight[0] > y2
        && y2 + boxHeight[1] > y1)
        return false;
    if (x1 + boxWidth[0] > x3 && x3 + boxWidth[2] > x1 && y1 + boxHeight[0] > y3
        && y3 + boxHeight[2] > y1)
        return false;
    if (x2 + boxWidth[1] > x3 && x3 + boxWidth[2] > x2 && y2 + boxHeight[1] > y3
        && y3 + boxHeight[2] > y2)
        return false;
    return true;
}

// 生成一个候选状态：偏移越界或不合法就丢弃，没访问过就压入队列
void tryPush(int off2X, int off2Y, int off3X, int off3Y, queue<int>& frontier) {
    if (off2X < -LIMIT || off2X > LIMIT || off2Y < -LIMIT || off2Y > LIMIT) return;
    if (off3X < -LIMIT || off3X > LIMIT || off3Y < -LIMIT || off3Y > LIMIT) return;
    int state = encode(off2X, off2Y, off3X, off3Y);
    if (visited[state]) return;
    visited[state] = true;   // 状态是否合法只取决于偏移，访问过就不用再判断
    int x2 = boxX[1] + off2X, y2 = boxY[1] + off2Y;
    int x3 = boxX[2] + off3X, y3 = boxY[2] + off3Y;
    if (!valid(boxX[0], boxY[0], x2, y2, x3, y3)) return;
    frontier.push(state);
}

int main() {
    scanf("%d %d %d", &squareCount[0], &squareCount[1], &squareCount[2]);

    for (int i = 0; i < 3; ++i) {
        int inputX[GRID * GRID], inputY[GRID * GRID];
        int minX = GRID, minY = GRID, maxX = -1, maxY = -1;
        for (int j = 0; j < squareCount[i]; ++j) {
            scanf("%d %d", &inputX[j], &inputY[j]);
            if (inputX[j] < minX) minX = inputX[j];
            if (inputX[j] > maxX) maxX = inputX[j];
            if (inputY[j] < minY) minY = inputY[j];
            if (inputY[j] > maxY) maxY = inputY[j];
        }
        boxX[i] = minX;
        boxY[i] = minY;
        boxWidth[i] = maxX - minX + 1;
        boxHeight[i] = maxY - minY + 1;
        // 方块改成相对包围盒左下角的坐标，之后只看相对位置，查表更方便
        for (int j = 0; j < squareCount[i]; ++j) {
            cellX[i][j] = inputX[j] - minX;
            cellY[i][j] = inputY[j] - minY;
            filled[i][cellX[i][j]][cellY[i][j]] = true;
        }
    }

    // 状态是各物体相对初始位置的偏移。物体 1 滑动一格，等价于物体 2、3 同时
    // 反向平移一格，所以只记录物体 2、3 的偏移就能表示全部局面。
    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};

    queue<int> frontier;
    frontier.push(encode(0, 0, 0, 0));
    visited[encode(0, 0, 0, 0)] = true;

    // 逐层扩展，层号就是最少滑动次数
    for (int distance = 0; !frontier.empty(); ++distance) {
        int levelSize = frontier.size();
        for (int s = 0; s < levelSize; ++s) {
            int state = frontier.front();
            frontier.pop();
            int off3Y = state % RANGE;
            int rest = state / RANGE;
            int off3X = rest % RANGE;
            rest /= RANGE;
            int off2Y = rest % RANGE;
            int off2X = rest / RANGE;
            off2X -= LIMIT;
            off2Y -= LIMIT;
            off3X -= LIMIT;
            off3Y -= LIMIT;

            int x1 = boxX[0], y1 = boxY[0];
            int x2 = boxX[1] + off2X, y2 = boxY[1] + off2Y;
            int x3 = boxX[2] + off3X, y3 = boxY[2] + off3Y;
            if (separated(x1, y1, x2, y2, x3, y3)) {
                printf("%d\n", distance);
                return 0;
            }

            for (int d = 0; d < 4; ++d) {
                tryPush(off2X + dx[d], off2Y + dy[d], off3X, off3Y, frontier);
                tryPush(off2X, off2Y, off3X + dx[d], off3Y + dy[d], frontier);
                // 物体 1 朝 (dx, dy) 滑动，相当于物体 2、3 一起朝反方向平移
                tryPush(off2X - dx[d], off2Y - dy[d], off3X - dx[d], off3Y - dy[d], frontier);
            }
        }
    }

    printf("-1\n");
    return 0;
}
