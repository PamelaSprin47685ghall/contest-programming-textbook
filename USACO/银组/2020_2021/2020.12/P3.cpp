#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 1000;

int n;
int xs[MAXN], ys[MAXN];              // 每头奶牛的起点坐标
int eastList[MAXN], northList[MAXN]; // 分别存放朝东、朝北奶牛的下标
int eastCnt, northCnt;
bool stopped[MAXN];  // 该奶牛是否已经停在别人吃过的格子上
int blame[MAXN];     // 该奶牛直接或间接拦停的奶牛数

// 朝东的奶牛按 y 升序：它到达某个交叉点所需的步数随 y 单调递增，
// 于是先枚举到的配对一定先发生，可以边扫边确定胜负
bool cmpEast(int a, int b) {
    return ys[a] < ys[b];
}

// 朝北的奶牛按 x 升序，理由同上
bool cmpNorth(int a, int b) {
    return xs[a] < xs[b];
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        char dir;
        scanf(" %c %d %d", &dir, &xs[i], &ys[i]);
        if (dir == 'E') {
            eastList[eastCnt++] = i;
        } else {
            northList[northCnt++] = i;
        }
    }
    sort(eastList, eastList + eastCnt, cmpEast);
    sort(northList, northList + northCnt, cmpNorth);

    for (int ei = 0; ei < eastCnt; ++ei) {
        int e = eastList[ei];
        for (int ni = 0; ni < northCnt; ++ni) {
            int w = northList[ni];
            // 两头牛都还没被拦停时，按上面的枚举顺序它们一定能走到交叉点
            if (stopped[e] || stopped[w]) {
                continue;
            }
            // 朝东的牛沿 y = ys[e] 向右，朝北的牛沿 x = xs[w] 向上，
            // 只有 (xs[w], ys[e]) 位于起点 (xs[e], ys[e]) 的右上方时才可能相交
            if (xs[w] <= xs[e] || ys[e] <= ys[w]) {
                continue;
            }
            int distE = xs[w] - xs[e];  // 朝东的牛走到交叉点的步数
            int distN = ys[e] - ys[w];  // 朝北的牛走到交叉点的步数
            if (distE == distN) {
                continue;  // 同一步到达则共享草地，谁也不被拦停
            }
            if (distE > distN) {
                // 朝北的牛先到并吃掉草，朝东的牛随后停下
                stopped[e] = true;
                blame[w] += 1 + blame[e];
            } else {
                stopped[w] = true;
                blame[e] += 1 + blame[w];
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        printf("%d\n", blame[i]);
    }
    return 0;
}
