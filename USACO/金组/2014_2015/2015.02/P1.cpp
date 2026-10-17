#include <cstdio>

using namespace std;

const int MAXR = 755;
const int MAXC = 755;
const int MAXK = 562505; // K <= R*C <= 562500
const int MOD = 1000000007;

int R, C, K;
int grid[MAXR][MAXC];

// 每种颜色单独开一个树状数组太费内存, 但如果只关心它真正出现过的列, 压缩之后
// 所有颜色的总长度不超过 R*C, 所以把它们的树状数组拼在一个大数组里
int lastSeen[MAXK];      // 按列扫描时记录颜色上一次被计入的列号, 用于去重
int colorCnt[MAXK];      // 每种颜色出现过的不同列数
int listOff[MAXK];       // 每种颜色的列列表段在 colList 中的起始位置
int treeOff[MAXK];       // 每种颜色的树状数组段在 colorTree 中的起始位置
int writeAt[MAXK];       // 第二遍扫描时各颜色列列表的写入口
int colList[MAXK];       // 所有颜色出现过的列号, 按颜色分段, 段内升序
int colorTree[2 * MAXK]; // 每条颜色段内下标 1..colorCnt 是它的树状数组

int fullTree[MAXC + 1]; // 全局: 按列统计所有已处理格子的路径数

void fullAdd(int col, int val) {
    for (int i = col + 1; i <= C; i += i & (-i)) {
        fullTree[i] += val;
        if (fullTree[i] >= MOD) fullTree[i] -= MOD;
    }
}

// 列号 <= col 的所有格子路径数之和
int fullSum(int col) {
    int res = 0;
    for (int i = col + 1; i > 0; i -= i & (-i)) {
        res += fullTree[i];
        if (res >= MOD) res -= MOD;
    }
    return res;
}

// 颜色 color 的列列表里 <= col 的元素个数, 也就是 col 在段内的 1-based 下标
int colorRank(int color, int col) {
    int lo = 0, hi = colorCnt[color];
    int base = listOff[color];
    while (lo < hi) {
        int mid = (lo + hi + 1) >> 1;
        if (colList[base + mid - 1] <= col) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

// 颜色 color 的格子里列号 <= col 的路径数之和
int colorSum(int color, int col) {
    int base = treeOff[color];
    int res = 0;
    for (int i = colorRank(color, col); i > 0; i -= i & (-i)) {
        res += colorTree[base + i];
        if (res >= MOD) res -= MOD;
    }
    return res;
}

void colorAdd(int color, int col, int val) {
    int base = treeOff[color];
    for (int i = colorRank(color, col); i <= colorCnt[color]; i += i & (-i)) {
        colorTree[base + i] += val;
        if (colorTree[base + i] >= MOD) colorTree[base + i] -= MOD;
    }
}

int main() {
    scanf("%d %d %d", &R, &C, &K);
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++)
            scanf("%d", &grid[i][j]);

    // 第一遍: 统计每种颜色出现在多少个不同的列, 好给树状数组分配紧凑的空间
    for (int i = 0; i <= K; i++) lastSeen[i] = -1;
    for (int j = 0; j < C; j++)
        for (int i = 0; i < R; i++) {
            int c = grid[i][j];
            if (lastSeen[c] != j) {
                lastSeen[c] = j;
                colorCnt[c]++;
            }
        }

    int total = 0;  // 所有颜色列列表的总长度
    int treeAt = 0; // colorTree 已占用的长度
    for (int c = 1; c <= K; c++) {
        listOff[c] = total;
        treeOff[c] = treeAt;
        treeAt += colorCnt[c] + 1; // 多留一格, 让段内树状数组从 1 开始编号
        writeAt[c] = total;
        total += colorCnt[c];
    }

    // 第二遍: 按列扫描, 填出每种颜色的列列表 (列号自然升序)
    for (int i = 0; i <= K; i++) lastSeen[i] = -1;
    for (int j = 0; j < C; j++)
        for (int i = 0; i < R; i++) {
            int c = grid[i][j];
            if (lastSeen[c] != j) {
                lastSeen[c] = j;
                colList[writeAt[c]++] = j;
            }
        }

    // 站在起点 (0,0) 上的那条"空路径"算一条
    fullAdd(0, 1);
    colorAdd(grid[0][0], 0, 1);

    // 逐行从上往下、每行从右往左算 dp。倒着扫列是为了让本行刚加入的格子都落在
    // 当前格子右边, 因此不会被前缀和查到: 跳跃必须严格向右下, 同行和同列都不能跳
    for (int i = 1; i <= R - 2; i++) {
        for (int j = C - 2; j >= 1; j--) {
            int c = grid[i][j];
            int val = fullSum(j - 1) - colorSum(c, j - 1);
            if (val < 0) val += MOD;
            fullAdd(j, val);
            colorAdd(c, j, val);
        }
    }

    // 终点 (R-1,C-1): 查它左上方所有格子, 再扣掉颜色相同的那些
    int ans = fullSum(C - 2) - colorSum(grid[R - 1][C - 1], C - 2);
    if (ans < 0) ans += MOD;
    printf("%d\n", ans);
    return 0;
}
