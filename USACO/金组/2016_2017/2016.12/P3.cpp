#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 100005;
const int MAXV = MAXN + 5;  // 直线条数: 每根柱子一条横线一条竖线, 外加激光和谷仓的坐标

int postX[MAXN];
int postY[MAXN];

int valueX[MAXV];  // 出现过的所有 x 值, 排序去重后给垂直直线编号
int valueY[MAXV];

int lineX[MAXN];  // 第 i 根柱子所在的垂直直线编号
int lineY[MAXN];  // 第 i 根柱子所在的水平直线编号

int bucketX[MAXV];   // 先当"每条垂直直线上有几根柱子"用, 再原地变成该组的起始下标
int bucketY[MAXV];
int positionX[MAXV];  // 填分组时的游标, 避免破坏已经算好的起始下标
int positionY[MAXV];
int orderX[MAXN];     // 柱子下标按垂直直线分组, 同一条直线上的连续存放
int orderY[MAXN];     // 同上, 按水平直线分组

// 状态编号: 0..numY-1 是水平直线, numY..numY+numX-1 是垂直直线.
int dist[MAXV * 2];
int bfsQueue[MAXV * 2];

int main() {
    int n;
    int laserX, laserY, barnX, barnY;
    scanf("%d%d%d%d%d", &n, &laserX, &laserY, &barnX, &barnY);

    // 激光和谷仓所在的直线也必须拿到编号, 否则起点和终点没有落脚的状态.
    int numX = 0;
    int numY = 0;
    valueX[numX++] = laserX;
    valueX[numX++] = barnX;
    valueY[numY++] = laserY;
    valueY[numY++] = barnY;
    for (int i = 0; i < n; ++i) {
        scanf("%d%d", &postX[i], &postY[i]);
        valueX[numX++] = postX[i];
        valueY[numY++] = postY[i];
    }
    sort(valueX, valueX + numX);
    numX = (int)(unique(valueX, valueX + numX) - valueX);
    sort(valueY, valueY + numY);
    numY = (int)(unique(valueY, valueY + numY) - valueY);

    for (int i = 0; i < n; ++i) {
        lineX[i] = (int)(lower_bound(valueX, valueX + numX, postX[i]) - valueX);
        lineY[i] = (int)(lower_bound(valueY, valueY + numY, postY[i]) - valueY);
        ++bucketX[lineX[i]];
        ++bucketY[lineY[i]];
    }

    // 计数排序把柱子按直线分组, 于是处理一条直线只要扫过一段连续区间.
    // 每条直线只被处理一次, 全部直线加起来也只扫 n 根柱子, 所以搜索是线性的.
    int total = 0;
    for (int j = 0; j < numX; ++j) {
        int amount = bucketX[j];
        bucketX[j] = total;
        total += amount;
    }
    bucketX[numX] = total;  // 哨兵: 第 j 组的终点就是第 j+1 组的起点
    total = 0;
    for (int j = 0; j < numY; ++j) {
        int amount = bucketY[j];
        bucketY[j] = total;
        total += amount;
    }
    bucketY[numY] = total;

    for (int j = 0; j <= numX; ++j) {
        positionX[j] = bucketX[j];
    }
    for (int j = 0; j <= numY; ++j) {
        positionY[j] = bucketY[j];
    }
    for (int i = 0; i < n; ++i) {
        orderX[positionX[lineX[i]]++] = i;
        orderY[positionY[lineY[i]]++] = i;
    }

    for (int state = 0; state < numY + numX; ++state) {
        dist[state] = -1;
    }

    // 起点是激光所在的横线和竖线, 因为激光一开始就能朝水平或竖直方向射出, 不用镜子.
    // 终点是谷仓所在的横线或竖线: 光只要沿这条线走, 就一定会扫过谷仓.
    int startHorizontal = (int)(lower_bound(valueY, valueY + numY, laserY) - valueY);
    int startVertical = numY + (int)(lower_bound(valueX, valueX + numX, laserX) - valueX);
    int goalHorizontal = (int)(lower_bound(valueY, valueY + numY, barnY) - valueY);
    int goalVertical = numY + (int)(lower_bound(valueX, valueX + numX, barnX) - valueX);

    int head = 0;
    int tail = 0;
    dist[startHorizontal] = 0;
    bfsQueue[tail++] = startHorizontal;
    dist[startVertical] = 0;
    bfsQueue[tail++] = startVertical;

    int answer = -1;
    while (head < tail) {
        int current = bfsQueue[head++];
        // 每条边都只花一面镜子, 队列按镜子数不减的顺序出队,
        // 所以第一次碰到谷仓所在的直线时, 用的镜子一定最少.
        if (current == goalHorizontal || current == goalVertical) {
            answer = dist[current];
            break;
        }
        if (current < numY) {
            // 沿水平直线前进会经过它上面的每一根柱子, 在柱子上装镜子就转到对应的垂直直线.
            for (int k = bucketY[current]; k < bucketY[current + 1]; ++k) {
                int next = numY + lineX[orderY[k]];
                if (dist[next] == -1) {
                    dist[next] = dist[current] + 1;
                    bfsQueue[tail++] = next;
                }
            }
        } else {
            int line = current - numY;
            for (int k = bucketX[line]; k < bucketX[line + 1]; ++k) {
                int next = lineY[orderX[k]];
                if (dist[next] == -1) {
                    dist[next] = dist[current] + 1;
                    bfsQueue[tail++] = next;
                }
            }
        }
    }
    printf("%d\n", answer);
    return 0;
}
