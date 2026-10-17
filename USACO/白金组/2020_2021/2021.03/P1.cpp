#include <cstdio>

using namespace std;

const int MAXN = 200005;  // 牛的数量上限

int n;
int breed[MAXN];       // 每头牛的品种
int lastOcc[MAXN];     // 每种品种最近一次出现的位置
int prevPrev[MAXN];    // 每种品种上上次出现的位置

// 线段树维护数组 val[l]：固定右端点 r 时，
// val[l] = active[l] * unique[l]，其中
//   active[l] = 1 表示 l 处牛的品种在 [l, r] 中独一无二（可以当领导者）；
//   unique[l] = (l, r) 中品种独一无二的牛的数量（可以当第三位领导者）。
// 答案是每个 r 处一段连续 l 的 val 之和。
long long treeSum[4 * MAXN];    // 区间 val 之和
long long activeCnt[4 * MAXN];  // 区间内 active 的位置个数
long long lazyAdd[4 * MAXN];    // 待下传到整个区间的增量

void applyLazy(int node, long long delta) {
    // 只有 active 的位置才吃增量，所以乘上个数
    treeSum[node] += delta * activeCnt[node];
    lazyAdd[node] += delta;
}

void pushDown(int node) {
    if (lazyAdd[node] != 0) {
        applyLazy(node * 2, lazyAdd[node]);
        applyLazy(node * 2 + 1, lazyAdd[node]);
        lazyAdd[node] = 0;
    }
}

// 单点修改 active 状态：delta = 1 激活，delta = -1 失活
void updateActive(int node, int left, int right, int pos, int delta) {
    if (left == right) {
        activeCnt[node] += delta;
        // 失活后这一位永远不再贡献答案，直接清零，避免残留值污染区间的和
        if (activeCnt[node] == 0) treeSum[node] = 0;
        return;
    }
    pushDown(node);
    int mid = (left + right) / 2;
    if (pos <= mid) updateActive(node * 2, left, mid, pos, delta);
    else updateActive(node * 2 + 1, mid + 1, right, pos, delta);
    treeSum[node] = treeSum[node * 2] + treeSum[node * 2 + 1];
    activeCnt[node] = activeCnt[node * 2] + activeCnt[node * 2 + 1];
}

// 给 [ql, qr] 中所有 active 位置的 unique 值加上 delta
void updateRange(int node, int left, int right, int ql, int qr, long long delta) {
    if (ql > qr) return;
    if (qr < left || right < ql) return;
    if (ql <= left && right <= qr) {
        applyLazy(node, delta);
        return;
    }
    pushDown(node);
    int mid = (left + right) / 2;
    updateRange(node * 2, left, mid, ql, qr, delta);
    updateRange(node * 2 + 1, mid + 1, right, ql, qr, delta);
    treeSum[node] = treeSum[node * 2] + treeSum[node * 2 + 1];
    activeCnt[node] = activeCnt[node * 2] + activeCnt[node * 2 + 1];
}

long long queryRange(int node, int left, int right, int ql, int qr) {
    if (ql > qr) return 0;
    if (qr < left || right < ql) return 0;
    if (ql <= left && right <= qr) return treeSum[node];
    pushDown(node);
    int mid = (left + right) / 2;
    return queryRange(node * 2, left, mid, ql, qr) + queryRange(node * 2 + 1, mid + 1, right, ql, qr);
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) scanf("%d", &breed[i]);

    long long answer = 0;
    for (int r = 1; r <= n; ++r) {
        int kind = breed[r];
        int prev = lastOcc[kind];
        // 上一次同品种的牛不再"独一无二"，它作为左端点的贡献作废
        if (prev > 0) updateActive(1, 1, n, prev, -1);
        // prev 不再是独有的牛后，它原来作为"中间领导者"的贡献要为更靠左的左端点减掉；
        // 只有活动区间里才需要减，所以只管 active 位置
        updateRange(1, 1, n, prevPrev[kind] + 1, prev - 1, -1);
        // 此时每个活动左端点处存的值恰好是 (l, r) 中可选中间领导者的个数：
        // 品种与 B_l、B_r 都不同（否则 l 不 active 或 l <= prev），且在中间只出现一次
        answer += queryRange(1, 1, n, prev + 1, r - 1);
        // 右端点向右扩展一格，新增的这头牛对所有 l > prev 来说都是"中间候选人"
        updateRange(1, 1, n, prev + 1, r - 1, 1);
        updateActive(1, 1, n, r, 1);
        prevPrev[kind] = prev;
        lastOcc[kind] = r;
    }

    printf("%lld\n", answer);
    return 0;
}
