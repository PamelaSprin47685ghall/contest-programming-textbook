#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 100000; // 奶牛数上限

int cowCount;           // 奶牛数
int cowX[MAXN + 1];     // 每头奶牛的 x 坐标
int cowY[MAXN + 1];     // 每头奶牛的 y 坐标
int orderByY[MAXN + 1]; // 奶牛编号按 y 从小到大排序
int orderByX[MAXN + 1]; // 奶牛编号按 x 从小到大排序
int yRank[MAXN + 1];    // 每头奶牛的 y 坐标被压成 1..distinctY 的编号
int leftTree[MAXN + 1];  // 树状数组：竖直栅栏左侧，各 y 编号上的奶牛数
int rightTree[MAXN + 1]; // 树状数组：竖直栅栏右侧，各 y 编号上的奶牛数
int distinctY;          // 不同 y 坐标的个数

bool yOrderLess(int a, int b) {
    if (cowY[a] != cowY[b]) return cowY[a] < cowY[b];
    return cowX[a] < cowX[b];
}

bool xOrderLess(int a, int b) {
    if (cowX[a] != cowX[b]) return cowX[a] < cowX[b];
    return cowY[a] < cowY[b];
}

// 树状数组：给编号 pos 的计数加上 delta
void treeAdd(int* tree, int pos, int delta) {
    for (; pos <= distinctY; pos += pos & -pos) tree[pos] += delta;
}

// 树状数组：求编号 1..pos 的计数之和
int treeSum(int* tree, int pos) {
    int total = 0;
    for (; pos > 0; pos -= pos & -pos) total += tree[pos];
    return total;
}

// 竖直栅栏已经分好左右两侧时，求横向栅栏最优时四个区域中最大的奶牛数。
// 参数是左右两侧各自的奶牛总数，栅栏下方的奶牛按 y 编号的前缀在这些树里统计。
int bestHorizontalSplit(int leftTotal, int rightTotal) {
    // 横向栅栏往上移：下方两头数单调不减，上方两头数单调不增，
    // 所以"下方最多"与"上方最多"这两条曲线只会交叉一次，最优解就在交叉处
    int low = 1, high = distinctY;
    while (low < high) {
        int mid = (low + high) / 2;
        int leftBelow = treeSum(leftTree, mid);
        int rightBelow = treeSum(rightTree, mid);
        int belowMost = max(leftBelow, rightBelow);
        int aboveMost = max(leftTotal - leftBelow, rightTotal - rightBelow);
        if (belowMost >= aboveMost) high = mid;
        else low = mid + 1;
    }
    // 交叉处两次都试：分界也可以落在所有奶牛下方（即编号 0）
    int best = cowCount;
    for (int split = max(low - 1, 0); split <= low; ++split) {
        int leftBelow = treeSum(leftTree, split);
        int rightBelow = treeSum(rightTree, split);
        int worst = max(max(leftBelow, rightBelow),
                        max(leftTotal - leftBelow, rightTotal - rightBelow));
        if (worst < best) best = worst;
    }
    return best;
}

int main() {
    scanf("%d", &cowCount);
    for (int i = 1; i <= cowCount; ++i) {
        scanf("%d%d", &cowX[i], &cowY[i]);
        orderByX[i] = i;
        orderByY[i] = i;
    }

    // 离散化 y 坐标：同一 y 共用一个编号，编号越小 y 越小
    sort(orderByY + 1, orderByY + cowCount + 1, yOrderLess);
    distinctY = 0;
    for (int i = 1; i <= cowCount; ++i) {
        if (i == 1 || cowY[orderByY[i]] != cowY[orderByY[i - 1]]) ++distinctY;
        yRank[orderByY[i]] = distinctY;
    }

    // 把竖直栅栏放到奶牛左边时，所有奶牛都在栅栏右侧
    sort(orderByX + 1, orderByX + cowCount + 1, xOrderLess);
    for (int i = 1; i <= cowCount; ++i) treeAdd(rightTree, yRank[i], 1);
    int answer = bestHorizontalSplit(0, cowCount);

    // 竖直栅栏从左往右推：每推过一个 x 坐标，该处的奶牛就换到左侧。
    // 因为奶牛坐标是奇数，栅栏放在 x + 1（偶数）处正好把这一批分到左边。
    int leftTotal = 0;
    for (int i = 1; i <= cowCount; ) {
        int j = i;
        while (j <= cowCount && cowX[orderByX[j]] == cowX[orderByX[i]]) {
            int cow = orderByX[j];
            treeAdd(rightTree, yRank[cow], -1);
            treeAdd(leftTree, yRank[cow], 1);
            ++leftTotal;
            ++j;
        }
        answer = min(answer, bestHorizontalSplit(leftTotal, cowCount - leftTotal));
        i = j;
    }

    printf("%d\n", answer);
    return 0;
}
