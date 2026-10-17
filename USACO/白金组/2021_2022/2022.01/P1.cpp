#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 100000;        // 干草堆数量上限
const int INF_ADD = 1000000000; // 把已取走的位置抬高到"永远不会是 0"的增量

int n, k;
int h[MAXN];            // 每堆干草的高度
int orderByH[MAXN];     // 按高度升序排列后的干草堆编号
int sortedHeight[MAXN]; // orderByH[p] 对应的高度，用于二分
int locationOf[MAXN];   // 每堆干草在"按高度排序"中的位置

int blockedAt[MAXN];   // 每个位置的初始阻塞数
int segMin[4 * MAXN];  // 线段树：区间内阻塞数的最小值
int segLazy[4 * MAXN]; // 线段树：区间加的懒标记

int bitCnt[MAXN + 1]; // 树状数组，按下标统计已经插入的高度

void bitAdd(int pos, int delta) {
    for (int i = pos + 1; i <= n; i += i & (-i)) bitCnt[i] += delta;
}

// 位置 [0, pos] 内已插入的干草堆个数，pos 为 -1 时返回 0
int bitSum(int pos) {
    int res = 0;
    for (int i = pos + 1; i > 0; i -= i & (-i)) res += bitCnt[i];
    return res;
}

bool compareByHeight(int a, int b) { return h[a] < h[b]; }

void applyAdd(int node, int delta) {
    segMin[node] += delta;
    segLazy[node] += delta;
}

void pushDown(int node) {
    if (segLazy[node] != 0) {
        applyAdd(2 * node, segLazy[node]);
        applyAdd(2 * node + 1, segLazy[node]);
        segLazy[node] = 0;
    }
}

void build(int node, int left, int right) {
    segLazy[node] = 0;
    if (left == right) {
        segMin[node] = blockedAt[left];
        return;
    }
    int mid = (left + right) / 2;
    build(2 * node, left, mid);
    build(2 * node + 1, mid + 1, right);
    segMin[node] = min(segMin[2 * node], segMin[2 * node + 1]);
}

void rangeAdd(int node, int left, int right, int ql, int qr, int delta) {
    if (qr < left || right < ql) return;
    if (ql <= left && right <= qr) {
        applyAdd(node, delta);
        return;
    }
    pushDown(node);
    int mid = (left + right) / 2;
    rangeAdd(2 * node, left, mid, ql, qr, delta);
    rangeAdd(2 * node + 1, mid + 1, right, ql, qr, delta);
    segMin[node] = min(segMin[2 * node], segMin[2 * node + 1]);
}

// 找出最靠左的阻塞数为 0 的位置；题目保证这样的位置一定存在
int findFirstZero(int node, int left, int right) {
    if (segMin[node] > 0) return -1; // 这一整段都被阻塞，不必深入
    if (left == right) return left;
    pushDown(node);
    int mid = (left + right) / 2;
    int res = findFirstZero(2 * node, left, mid);
    if (res != -1) return res;
    return findFirstZero(2 * node + 1, mid + 1, right);
}

int main() {
    scanf("%d%d", &n, &k);
    for (int i = 0; i < n; ++i) scanf("%d", &h[i]);

    for (int i = 0; i < n; ++i) orderByH[i] = i;
    sort(orderByH, orderByH + n, compareByHeight);
    for (int p = 0; p < n; ++p) {
        sortedHeight[p] = h[orderByH[p]];
        locationOf[orderByH[p]] = p;
    }

    // 初始阻塞数：第 i 堆左边有多少堆与它高度差超过 K。
    // 按输入顺序插入树状数组，就能只统计下标更小的那些干草堆。
    for (int i = 0; i < n; ++i) {
        int low = lower_bound(sortedHeight, sortedHeight + n, h[i] - k) - sortedHeight;
        int high = upper_bound(sortedHeight, sortedHeight + n, h[i] + k) - sortedHeight;
        int blocked = 0;
        if (low > 0) blocked += bitSum(low - 1);            // 比 h[i]-K 还矮的
        if (high < n) blocked += bitSum(n - 1) - bitSum(high - 1); // 比 h[i]+K 还高的
        blockedAt[locationOf[i]] = blocked;
        bitAdd(locationOf[i], 1);
    }

    build(1, 0, n - 1);

    for (int step = 0; step < n; ++step) {
        // 阻塞数为 0 的最小高度，就是这一步能挪到最前面的最小干草堆
        int pos = findFirstZero(1, 0, n - 1);
        int idx = orderByH[pos];
        printf("%d\n", h[idx]);

        // 被取走的这堆此后永远在剩余干草堆的左边，
        // 所以它"挡住"的那些（高度差超过 K 的）阻塞数都要减一
        int low = lower_bound(sortedHeight, sortedHeight + n, h[idx] - k) - sortedHeight;
        int high = upper_bound(sortedHeight, sortedHeight + n, h[idx] + k) - sortedHeight;
        if (low > 0) rangeAdd(1, 0, n - 1, 0, low - 1, -1);
        if (high < n) rangeAdd(1, 0, n - 1, high, n - 1, -1);
        rangeAdd(1, 0, n - 1, pos, pos, INF_ADD); // 该位置标记为已取走
    }
    return 0;
}
