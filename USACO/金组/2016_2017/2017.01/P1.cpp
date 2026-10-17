#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 100005;

int height[MAXN];
int order[MAXN];    // 按高度从高到低排列的下标
int fenwick[MAXN];  // 树状数组: 记录某个位置上是否已经插入了更高的牛

// 高度降序排序, 于是处理第 k 头牛时, 树状数组里装的恰好是前 k-1 头更高的牛
bool tallerFirst(int a, int b) {
    return height[a] > height[b];
}

void insertPos(int pos, int n) {
    for (; pos <= n; pos += pos & (-pos)) {
        fenwick[pos] += 1;
    }
}

// 统计树状数组里下标不超过 pos 的牛数, 也就是牛 i 左侧比自己高的牛数
int countUpTo(int pos) {
    int sum = 0;
    for (; pos > 0; pos -= pos & (-pos)) {
        sum += fenwick[pos];
    }
    return sum;
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &height[i]);
        order[i] = i;
    }
    sort(order + 1, order + n + 1, tallerFirst);

    int unbalanced = 0;
    for (int k = 1; k <= n; ++k) {
        int idx = order[k];
        // 比它高的牛共 k-1 头, 左侧的有 left 头, 剩下的全在右侧
        int left = countUpTo(idx);
        int right = (k - 1) - left;
        if (max(left, right) > 2 * min(left, right)) {
            ++unbalanced;
        }
        insertPos(idx, n);
    }
    printf("%d\n", unbalanced);
    return 0;
}
