#include <algorithm>
#include <climits>
#include <cstdio>

using namespace std;

const int MAXN = 200; // 奶牛数量上限

struct Cow {
    int x;
    int y;
};

Cow cows[MAXN];
int cowCount;

int betweenY[MAXN]; // 按 y 升序存放"夹在左右两头牛之间"的那些牛的 y 坐标
int betweenSize;

long long subsetCount;    // 已经数出来的奶牛集合数（正方形外接矩形的集合会被数两次）
long long squareBoxCount; // 其中外接矩形恰好是正方形的那些

bool compareByX(const Cow &a, const Cow &b) {
    return a.x < b.x;
}

// 插进有序数组并保持升序。坐标两两不同，所以不用处理相等的情况
void addBetweenY(int value) {
    int pos = betweenSize;
    while (pos > 0 && betweenY[pos - 1] > value) {
        betweenY[pos] = betweenY[pos - 1];
        --pos;
    }
    betweenY[pos] = value;
    ++betweenSize;
}

// 数出所有"外接矩形宽大于等于高"的奶牛集合：
// 枚举集合里最左的牛 left 和最右的牛 right，方块边长只能是它们的 x 距离，
// 再让方块的下边从最低可能位置一格一格往上滑，每滑到一个新位置就得到一个新集合
void countSubsetsWithWidthAtLeastHeight() {
    sort(cows, cows + cowCount, compareByX);
    for (int left = 0; left < cowCount; ++left) {
        betweenSize = 0;
        for (int right = left + 1; right < cowCount; ++right) {
            if (right > left + 1) addBetweenY(cows[right - 1].y);
            int side = cows[right].x - cows[left].x;
            // 方块的下边 minBottomY 必须让两端牛的 y 都落进 [minBottomY, minBottomY + side]
            int minBottomY = max(cows[left].y, cows[right].y) - side;
            int maxBottomY = min(cows[left].y, cows[right].y);
            if (minBottomY > maxBottomY) continue; // 方块再高也装不下两端牛的 y 差

            // 双指针：betweenY[low, high] 正是当前方块圈住的中间那些牛。
            // 分界条件是"牛的 y 落在 [下边, 上边]"：y 比下边小就被排除，比上边大就还没进来
            int low = 0, high = -1;
            while (low < betweenSize && minBottomY >= betweenY[low] + 1) ++low;
            while (high + 1 < betweenSize && minBottomY >= betweenY[high + 1] - side) ++high;

            while (true) {
                ++subsetCount;
                int bottom = min(cows[left].y, cows[right].y);
                int top = max(cows[left].y, cows[right].y);
                if (low <= high) {
                    bottom = min(bottom, betweenY[low]);
                    top = max(top, betweenY[high]);
                }
                if (top - bottom == side) ++squareBoxCount;
                // 下一次集合发生变化：下边越过最低的那头牛，或者上边够到下一头牛
                int leaveAt = (low < betweenSize) ? betweenY[low] + 1 : INT_MAX;
                int enterAt = (high + 1 < betweenSize) ? betweenY[high + 1] - side : INT_MAX;
                int changeAt = min(leaveAt, enterAt);
                if (changeAt > maxBottomY) break; // 再往上滑两端牛就装不进去了
                if (leaveAt == changeAt) ++low;
                if (enterAt == changeAt) ++high;
            }
        }
    }
}

int main() {
    scanf("%d", &cowCount);
    for (int i = 0; i < cowCount; ++i) scanf("%d%d", &cows[i].x, &cows[i].y);

    subsetCount = cowCount + 1; // 空集与"只圈一头牛"都一定做得到
    countSubsetsWithWidthAtLeastHeight();
    // 交换 x、y 再数一遍，就能把"外接矩形高大于宽"的集合也数上。
    // 外接矩形恰好是正方形的集合两条边一样长，会被数两次，最后减掉一半
    for (int i = 0; i < cowCount; ++i) {
        int tmp = cows[i].x;
        cows[i].x = cows[i].y;
        cows[i].y = tmp;
    }
    countSubsetsWithWidthAtLeastHeight();

    printf("%lld\n", subsetCount - squareBoxCount / 2);
    return 0;
}
