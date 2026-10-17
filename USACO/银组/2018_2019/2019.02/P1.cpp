#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100000;

int cowCount;
int position[MAXN];             // 所有奶牛的坐标, 读入后升序排序

int solveMin() {
    int headSpan = position[cowCount - 2] - position[0];            // 前 N-1 头牛的跨度
    int tailSpan = position[cowCount - 1] - position[1];            // 后 N-1 头牛的跨度
    int firstGap = position[1] - position[0];                       // 最左边的空隙
    int lastGap = position[cowCount - 1] - position[cowCount - 2];  // 最右边的空隙

    // 特例: N-1 头牛已经连续, 只剩最后一头牛落单且隔了不止 2 格.
    // 把它挪进紧挨着的那格看似 1 步就够, 但它的落点一定会成为新的最小值或最大值,
    // 而题意要求移动后它不能是端点, 所以必须再挪一次.
    if (headSpan == cowCount - 2 && lastGap > 2) return 2;
    if (tailSpan == cowCount - 2 && firstGap > 2) return 2;

    // 一般情况: 找一段宽度不超过 N-1 的区间, 让落进去的牛尽量多, 也就是里面的空位尽量少.
    // 区间里每个空位都能让区间外的一头牛一次补上, 所以最少步数等于最少空位数.
    // 左端点右移时区间的右端点只会跟着右移, 于是用两个指针一次扫完.
    int mostCows = 0;
    int right = 0;
    for (int left = 0; left < cowCount; left++) {
        while (right + 1 < cowCount && position[right + 1] - position[left] <= cowCount - 1) {
            right++;
        }
        mostCows = max(mostCows, right - left + 1);
    }
    return cowCount - mostCows;
}

int main() {
    scanf("%d", &cowCount);
    for (int i = 0; i < cowCount; i++) {
        scanf("%d", &position[i]);
    }
    sort(position, position + cowCount);

    // 最多步数: 每一步都把某一端的牛只往中间挪一点, 就能拖出最多的步数.
    // 第一次移动必然要放弃两端外侧空隙中的一段, 于是留下较大的那段逐格消耗;
    // 除此以外的每个空位都能靠左右交替推进填满, 所以最大步数就是除去那一段空隙后的空位数.
    int maxMoves = max(position[cowCount - 2] - position[0], position[cowCount - 1] - position[1]) - (cowCount - 2);

    printf("%d\n%d\n", solveMin(), maxMoves);
    return 0;
}
