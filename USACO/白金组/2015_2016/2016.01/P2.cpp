#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 100000;      // 天数上限，段数最多 N-1
const int MAXH = 100000;      // 水平段 / 竖直段的段数上限
const int MAXENTRY = 2000000; // 二维 BIT 每个外层节点都要存一张内层坐标表

int n, tGap;
int px[MAXN + 1]; // 每天机器所在的 x
int py[MAXN + 1]; // 每天机器所在的 y

int hCnt;          // 水平段个数
int hTime[MAXH];   // 段编号（第几段），代表割草的先后
int hY[MAXH];      // 水平段的 y 坐标
int hRank[MAXH];   // 压缩后的 y 编号（0 起）
int hLeft[MAXH];   // 左端点的 x
int hRight[MAXH];  // 右端点的 x

int vCnt;         // 竖直段个数
int vTime[MAXH];  // 段编号
int vX[MAXH];     // 所在直线的 x
int vLow[MAXH];   // 下端点的 y
int vHigh[MAXH];  // 上端点的 y

int yVal[MAXH]; // 所有水平段的 y 坐标（排序去重后）
int yCount;

// 扫描线事件：同一 x 上先做完水平段的增删，再回答竖直段的询问
struct Event {
    int x;
    int type; // 0 加入水平段、1 删除水平段、2 询问
    int time; // 段编号
    int low;  // 加入/删除时是 y 坐标编号；询问时是 y 的下界（不含）
    int high; // 询问时是 y 的上界（不含）
};
Event ev[MAXN * 3];

bool cmpEvent(const Event &a, const Event &b) {
    if (a.x != b.x) return a.x < b.x;
    return a.type < b.type;
}

// 外层按"段编号（时间）"、内层按"y 坐标"的二维 BIT。
// 因为所有加点位置都能提前算出来，内层坐标表可以离线建好，省下动态开点的空间。
int fenN;                    // 外层 BIT 大小 = 段数
int nodeCoordCnt[MAXH + 2];  // 每个外层节点内层坐标个数
int nodeCoordStart[MAXH + 2];
int nodeCoord[MAXENTRY];     // 各外层节点的 y 编号表（升序）
int nodeTreeStart[MAXH + 2];
int nodeTree[MAXENTRY];      // 各外层节点的内层 BIT

int flatTree[MAXH + 2]; // 一维 BIT：当前激活的水平段按 y 编号的分布

// 在升序数组 tab 的 [0, cnt) 里数出"小于 v"的元素个数
int lowerBoundCount(const int *tab, int cnt, int v) {
    int lo = 0, hi = cnt;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (tab[mid] < v) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

// 数出 y 编号不超过 yLimit 的水平段在节点 p 里的当前个数
int countInNode(int p, int yLimit) {
    int start = nodeCoordStart[p];
    int maxIdx = lowerBoundCount(nodeCoord + start, nodeCoordCnt[p], yLimit + 1);
    int res = 0;
    for (int q = maxIdx; q > 0; q -= q & -q) res += nodeTree[nodeTreeStart[p] + q];
    return res;
}

// 统计段编号 < time、且 y 编号落在 [low, high] 内的水平段数
long long countBefore(int time, int low, int high) {
    if (low > high) return 0;
    if (time > fenN) time = fenN;
    long long res = 0;
    for (int p = time; p > 0; p -= p & -p) {
        res += countInNode(p, high) - countInNode(p, low - 1);
    }
    return res;
}

// 在（段编号 time，y 编号 rank）这个位置上加上 v
void addPoint(int time, int rank, int v) {
    for (int p = time + 1; p <= fenN; p += p & -p) {
        int start = nodeCoordStart[p];
        int pos = lowerBoundCount(nodeCoord + start, nodeCoordCnt[p], rank) + 1; // 1 起
        for (int q = pos; q <= nodeCoordCnt[p]; q += q & -q) nodeTree[nodeTreeStart[p] + q] += v;
    }
}

void flatAdd(int rank, int v) {
    for (int q = rank + 1; q <= yCount; q += q & -q) flatTree[q] += v;
}

int flatPrefix(int rank) { // y 编号 <= rank 的激活段数
    int res = 0;
    for (int q = rank + 1; q > 0; q -= q & -q) res += flatTree[q];
    return res;
}

int main() {
    scanf("%d%d", &n, &tGap);
    for (int i = 0; i < n; ++i) scanf("%d%d", &px[i], &py[i]);

    int segCnt = n - 1; // 段数，段编号从 0 起
    for (int idx = 0; idx < segCnt; ++idx) {
        int x0 = px[idx], y0 = py[idx], x1 = px[idx + 1], y1 = py[idx + 1];
        if (x0 == x1 && y0 == y1) continue; // 防御性地跳过长度为 0 的段
        if (y0 == y1) {                     // 水平段
            hTime[hCnt] = idx;
            hY[hCnt] = y0;
            hLeft[hCnt] = (x0 < x1 ? x0 : x1);
            hRight[hCnt] = (x0 < x1 ? x1 : x0);
            ++hCnt;
        } else { // 竖直段
            vTime[vCnt] = idx;
            vX[vCnt] = x0;
            vLow[vCnt] = (y0 < y1 ? y0 : y1);
            vHigh[vCnt] = (y0 < y1 ? y1 : y0);
            ++vCnt;
        }
    }

    // 把水平段的 y 坐标排序去重，得到压缩编号
    for (int i = 0; i < hCnt; ++i) yVal[i] = hY[i];
    sort(yVal, yVal + hCnt);
    yCount = 0;
    for (int i = 0; i < hCnt; ++i) {
        if (i == 0 || yVal[i] != yVal[yCount - 1]) yVal[yCount++] = yVal[i];
    }
    for (int i = 0; i < hCnt; ++i) hRank[i] = lowerBoundCount(yVal, yCount, hY[i]);

    fenN = segCnt;

    // 一、数出每个外层节点的内层坐标个数，定好各段的起点
    for (int p = 1; p <= fenN; ++p) nodeCoordCnt[p] = 0;
    for (int i = 0; i < hCnt; ++i) {
        for (int p = hTime[i] + 1; p <= fenN; p += p & -p) ++nodeCoordCnt[p];
    }
    int total = 0;
    for (int p = 1; p <= fenN; ++p) {
        nodeCoordStart[p] = total;
        total += nodeCoordCnt[p];
    }
    // 二、把每个水平段的 y 编号塞进它经过的所有外层节点
    {
        static int fillPtr[MAXH + 2];
        for (int p = 1; p <= fenN; ++p) fillPtr[p] = nodeCoordStart[p];
        for (int i = 0; i < hCnt; ++i) {
            for (int p = hTime[i] + 1; p <= fenN; p += p & -p) nodeCoord[fillPtr[p]++] = hRank[i];
        }
    }
    // 三、每个节点各自排序去重，并把紧凑后的起点写回
    int write = 0;
    for (int p = 1; p <= fenN; ++p) {
        int start = nodeCoordStart[p], cnt = nodeCoordCnt[p];
        sort(nodeCoord + start, nodeCoord + start + cnt);
        int uniq = 0;
        for (int k = 0; k < cnt; ++k) {
            if (k == 0 || nodeCoord[start + k] != nodeCoord[start + k - 1]) {
                nodeCoord[write + uniq] = nodeCoord[start + k];
                ++uniq;
            }
        }
        nodeCoordStart[p] = write;
        nodeCoordCnt[p] = uniq;
        write += uniq;
    }
    // 四、给每个节点的内层 BIT 分配连续空间（下标 1 起，所以多留一格）
    int treeTotal = 0;
    for (int p = 1; p <= fenN; ++p) {
        nodeTreeStart[p] = treeTotal;
        treeTotal += nodeCoordCnt[p] + 1;
    }

    // 收集扫描线事件
    int evCnt = 0;
    for (int i = 0; i < hCnt; ++i) {
        ev[evCnt].x = hLeft[i] + 1; // 端点不算交叉点，所以从 left+1 才开始生效
        ev[evCnt].type = 0;
        ev[evCnt].time = hTime[i];
        ev[evCnt].low = hRank[i];
        ev[evCnt].high = 0;
        ++evCnt;
        ev[evCnt].x = hRight[i]; // 扫到右端点时该段已经失效
        ev[evCnt].type = 1;
        ev[evCnt].time = hTime[i];
        ev[evCnt].low = hRank[i];
        ev[evCnt].high = 0;
        ++evCnt;
    }
    for (int j = 0; j < vCnt; ++j) {
        ev[evCnt].x = vX[j];
        ev[evCnt].type = 2;
        ev[evCnt].time = vTime[j];
        ev[evCnt].low = vLow[j] + 1;   // 交叉点不能在竖直段端点上
        ev[evCnt].high = vHigh[j] - 1; // 同上
        ++evCnt;
    }
    sort(ev, ev + evCnt, cmpEvent);

    long long answer = 0;
    for (int e = 0; e < evCnt; ++e) {
        if (ev[e].type == 0) {
            addPoint(ev[e].time, ev[e].low, 1);
            flatAdd(ev[e].low, 1);
        } else if (ev[e].type == 1) {
            addPoint(ev[e].time, ev[e].low, -1);
            flatAdd(ev[e].low, -1);
        } else {
            // 竖直段对应的 y 编号区间是 [loRank, hiRank]
            int loRank = lowerBoundCount(yVal, yCount, ev[e].low);
            int hiRank = lowerBoundCount(yVal, yCount, ev[e].high + 1) - 1;
            if (loRank > hiRank) continue; // 中间夹不到任何水平段所在的 y
            // 早割了 T 天以上的：段编号 <= 当前编号 - T
            long long earlier = countBefore(ev[e].time - tGap + 1, loRank, hiRank);
            // 晚割 T 天以上的：用当前激活总数减去编号 < 当前编号 + T 的部分
            long long active = flatPrefix(hiRank) - flatPrefix(loRank - 1);
            long long later = active - countBefore(ev[e].time + tGap, loRank, hiRank);
            answer += earlier + later;
        }
    }

    printf("%lld\n", answer);
    return 0;
}
