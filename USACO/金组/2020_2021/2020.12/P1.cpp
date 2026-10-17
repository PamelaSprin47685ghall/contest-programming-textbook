#include <cstdio>

using namespace std;

const int MAXN = 1000;             // 网格边长上限
const int MAXCELL = MAXN * MAXN;   // 格子总数上限

const int DIR_R[4] = {-1, 1, 0, 0};
const int DIR_C[4] = {0, 0, -1, 1};

char farm[MAXN][MAXN + 1];    // 牧场，每行末尾留一个 '\0' 给 scanf("%s")
int rockDist[MAXN][MAXN];     // 每格到最近岩石的距离（BFS 步数）
int centerDist[MAXN][MAXN];   // 机器人"中心"最早能在第几小时到达该格，-1 表示到不了
bool occupied[MAXN][MAXN];    // 该格是否可能出现过机器人（岩石从一开始就记为 true）

int queueCell[MAXCELL];       // 两种 BFS 共用的队列，元素是格子编号 r * n + c
int bucketBegin[MAXN + 2];    // 桶 level 的起点在 bucketCell 里的下标
int bucketCount[MAXN + 2];    // 桶 level 里有几个中心
int fillAt[MAXN + 2];         // 往桶里放格子时用的写指针
int bucketCell[MAXCELL];      // 所有中心按"最多能复制几次"分桶后的结果
int frontier[MAXCELL];        // 本阶段待向外扩散一格的格子集合
int nextFrontier[MAXCELL];    // 下一阶段的格子集合

int main() {
    int n;
    long long d;
    scanf("%d%lld", &n, &d);
    for (int r = 0; r < n; ++r) scanf("%s", farm[r]);

    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            rockDist[r][c] = -1;
            centerDist[r][c] = -1;
            // 岩石永远不可能有机器人，把它当成"已经占住"的格子，
            // 后面判断"这一格是不是空地"只需要看 occupied 一个标志
            occupied[r][c] = (farm[r][c] == '#');
        }
    }

    // 第一步：所有岩石一起出发做多源 BFS，求出每格到最近岩石的距离。
    // 这个距离决定了中心停在这里时最多能安全复制几次：距离为 k 时最多复制 k - 1 次。
    int head = 0, tail = 0;
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            if (farm[r][c] == '#') {
                rockDist[r][c] = 0;
                queueCell[tail++] = r * n + c;
            }
        }
    }
    while (head < tail) {
        int id = queueCell[head++];
        int r = id / n, c = id % n;
        for (int k = 0; k < 4; ++k) {
            int nr = r + DIR_R[k], nc = c + DIR_C[k];
            if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
            if (rockDist[nr][nc] != -1) continue;
            rockDist[nr][nc] = rockDist[r][c] + 1;
            queueCell[tail++] = nr * n + nc;
        }
    }

    // 第二步：从所有起点出发做 BFS，求出哪些格子能当"中心"，以及最早到达时间。
    // 机器人每次复制都变成以中心为圆心、半径加 1 的实心方块，
    // 所以一格的到达时间就是中心从起点走到这里花掉的小时数。
    head = 0, tail = 0;
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            if (farm[r][c] == 'S') {
                centerDist[r][c] = 0;
                occupied[r][c] = true;
                queueCell[tail++] = r * n + c;
            }
        }
    }
    while (head < tail) {
        int id = queueCell[head++];
        int r = id / n, c = id % n;
        // 中心此刻停下并复制，机器人会铺满半径 rockDist 的方块，碰到岩石；
        // 既然连复制都不行，就不可能再整体移动一格，于是这个中心不再向外扩张
        if (centerDist[r][c] >= d * rockDist[r][c]) continue;
        for (int k = 0; k < 4; ++k) {
            int nr = r + DIR_R[k], nc = c + DIR_C[k];
            if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
            if (farm[nr][nc] == '#') continue;
            if (centerDist[nr][nc] != -1) continue;
            int arriveHour = centerDist[r][c] + 1;
            // 迁移到新格子后，中心还要能在那里"待得住"：复制半径不能碰到岩石，
            // 也就是到达时间不超过 d * rockDist，否则半路就会全部停机
            if (arriveHour > d * rockDist[nr][nc]) continue;
            centerDist[nr][nc] = arriveHour;
            occupied[nr][nc] = true;
            queueCell[tail++] = nr * n + nc;
        }
    }

    // 第三步：把每个中心按"最多能安全复制的次数" level = rockDist - 1 分桶。
    // 中心停在原地不动时，能复制 level 次，覆盖它周围 level 步以内的所有格子
    int maxLevel = 0;
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            if (centerDist[r][c] < 0) continue;
            int level = rockDist[r][c] - 1;
            if (level > maxLevel) maxLevel = level;
        }
    }
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            if (centerDist[r][c] < 0) continue;
            ++bucketCount[rockDist[r][c] - 1];
        }
    }
    // 计数排序求每个桶的起点：桶 level 的格子集中在 bucketCell 的一段连续区间里
    int pos = 0;
    for (int level = 0; level <= maxLevel; ++level) {
        bucketBegin[level] = pos;
        pos += bucketCount[level];
        fillAt[level] = bucketBegin[level];
    }
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            if (centerDist[r][c] < 0) continue;
            bucketCell[fillAt[rockDist[r][c] - 1]++] = r * n + c;
        }
    }

    // 第四步：按 level 从大到小分阶段扩散。每往下一个阶段，允许的距离预算就多一格，
    // 所以"离某个中心不超过它的 level 步"的格子会被恰好扩散到，而不会多用预算
    int frontierSize = 0;
    int nextSize = 0;
    for (int level = maxLevel; level >= 0; --level) {
        for (int i = 0; i < frontierSize; ++i) {
            int id = frontier[i];
            int r = id / n, c = id % n;
            for (int k = 0; k < 4; ++k) {
                int nr = r + DIR_R[k], nc = c + DIR_C[k];
                if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
                if (occupied[nr][nc]) continue; // 岩石和已经算过的格子都在这里被挡住
                occupied[nr][nc] = true;
                nextFrontier[nextSize++] = nr * n + nc;
            }
        }
        // 本阶段加入的中心：它们自己一定算答案格，并且下一阶段继续向外扩散一格
        for (int i = bucketBegin[level]; i < bucketBegin[level] + bucketCount[level]; ++i) {
            nextFrontier[nextSize++] = bucketCell[i];
        }
        for (int i = 0; i < nextSize; ++i) frontier[i] = nextFrontier[i];
        frontierSize = nextSize;
        nextSize = 0;
    }

    int total = 0;
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            if (farm[r][c] != '#' && occupied[r][c]) ++total;
        }
    }
    printf("%d\n", total);
    return 0;
}
