#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 505;

int n;
int cx[MAXN], cy[MAXN];  // 每头牛的坐标
bool holstein[MAXN];     // true 表示荷斯坦牛，false 表示根西牛
int idx[MAXN];           // 按 x 坐标排好序的牛编号
int yval[MAXN];          // 出现过的所有 y 坐标（升序去重）

// sort 排序的是牛的编号，比较时通过全局数组取坐标
bool cmpByX(int a, int b) {
    return cx[a] < cx[b];
}

int main() {
    scanf("%d", &n);
    char breed[8];
    for (int i = 0; i < n; ++i) {
        scanf("%d %d %s", &cx[i], &cy[i], breed);
        holstein[i] = (breed[0] == 'H');
        yval[i] = cy[i];
        idx[i] = i;
    }
    sort(idx, idx + n, cmpByX);
    sort(yval, yval + n);
    int ycnt = (int)(unique(yval, yval + n) - yval);

    // 最优矩形的四条边上都必定有一头荷斯坦牛：边上没有牛的边可以往里缩，
    // 圈到的牛不少、面积却更小。所以上下边界只需枚举出现过的 y 坐标。
    int bestCnt = 0;   // 能圈住的最多荷斯坦牛数
    int bestArea = 0;  // 在牛数最多前提下的最小面积
    for (int a = 0; a < ycnt; ++a) {
        for (int b = a; b < ycnt; ++b) {
            // 只保留 y 落在 [ylo, yhi] 内的牛；a == b 表示高度为 0 的矩形
            int ylo = yval[a], yhi = yval[b];
            bool started = false;  // 当前这段"没有根西牛"的 x 区间是否已开始
            int leftX = 0;
            int cnt = 0;
            for (int i = 0; i < n;) {
                int j = i;
                int hc = 0, gc = 0;
                // 同一 x 坐标上的牛必须一起判断，否则边界上的根西牛会被漏掉
                while (j < n && cx[idx[j]] == cx[idx[i]]) {
                    int u = idx[j];
                    if (cy[u] >= ylo && cy[u] <= yhi) {
                        if (holstein[u]) {
                            ++hc;
                        } else {
                            ++gc;
                        }
                    }
                    ++j;
                }
                if (gc > 0) {
                    // 这一列有根西牛落在矩形内或边界上，矩形不能跨过它
                    started = false;
                    cnt = 0;
                } else if (hc > 0) {
                    if (!started) {
                        started = true;
                        leftX = cx[idx[i]];
                    }
                    cnt += hc;
                    // 左边界只能落在本段第一头荷斯坦牛上：更靠右只会少圈牛，
                    // 而牛数不比当前段最大值少是不可能的（见题解）
                    int area = (yhi - ylo) * (cx[idx[i]] - leftX);
                    if (cnt > bestCnt || (cnt == bestCnt && area < bestArea)) {
                        bestCnt = cnt;
                        bestArea = area;
                    }
                }
                i = j;
            }
        }
    }
    printf("%d\n%d\n", bestCnt, bestArea);
    return 0;
}
