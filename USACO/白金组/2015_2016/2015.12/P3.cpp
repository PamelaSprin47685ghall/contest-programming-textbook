#include <cstdio>

using namespace std;

const int MAXN = 200000;

// 线段树维护每个区间的干草捆最小值、总数，以及"还没发给儿子"的增量。
// 约定：minimum 和 sum 都已经包含本结点自己的 lazy，
// 所以查询到整段时可以直接返回，不用再往下走。
long long minimum[4 * MAXN];
long long sum[4 * MAXN];
long long lazy[4 * MAXN];

int initial[MAXN + 1]; // 各田块初始的干草捆数

const long long INF = 1000000000000000000LL; // 大于任何可能的答案，用作"没有交集"的返回值

// 把增量加到 node 负责的整段上：先只改这一层的记录，孩子等比真正走到时再补
void applyAdd(int node, int left, int right, long long delta) {
    minimum[node] += delta;
    sum[node] += delta * (right - left + 1);
    lazy[node] += delta;
}

// 把欠孩子的增量发下去，这样孩子被单独访问时数字才是对的
void pushDown(int node, int left, int right) {
    if (lazy[node] == 0) return;
    int mid = (left + right) / 2;
    applyAdd(node * 2, left, mid, lazy[node]);
    applyAdd(node * 2 + 1, mid + 1, right, lazy[node]);
    lazy[node] = 0;
}

// 两个儿子的值都准了以后，重新算出本段的统计量
void pullUp(int node) {
    minimum[node] = minimum[node * 2] < minimum[node * 2 + 1]
                        ? minimum[node * 2]
                        : minimum[node * 2 + 1];
    sum[node] = sum[node * 2] + sum[node * 2 + 1];
}

void build(int node, int left, int right) {
    if (left == right) {
        minimum[node] = initial[left];
        sum[node] = initial[left];
        return;
    }
    int mid = (left + right) / 2;
    build(node * 2, left, mid);
    build(node * 2 + 1, mid + 1, right);
    pullUp(node);
}

// 把 [queryLeft, queryRight] 里每个田块都加上 delta
void addRange(int node, int left, int right, int queryLeft, int queryRight, long long delta) {
    if (queryRight < left || right < queryLeft) return;
    if (queryLeft <= left && right <= queryRight) {
        applyAdd(node, left, right, delta);
        return;
    }
    // 本段和查询区间只相交一部分，必须把孩子算准：先发增量，再分别递归
    pushDown(node, left, right);
    int mid = (left + right) / 2;
    addRange(node * 2, left, mid, queryLeft, queryRight, delta);
    addRange(node * 2 + 1, mid + 1, right, queryLeft, queryRight, delta);
    pullUp(node);
}

long long queryMin(int node, int left, int right, int queryLeft, int queryRight) {
    if (queryRight < left || right < queryLeft) return INF;
    if (queryLeft <= left && right <= queryRight) return minimum[node];
    pushDown(node, left, right);
    int mid = (left + right) / 2;
    long long leftMin = queryMin(node * 2, left, mid, queryLeft, queryRight);
    long long rightMin = queryMin(node * 2 + 1, mid + 1, right, queryLeft, queryRight);
    return leftMin < rightMin ? leftMin : rightMin;
}

long long querySum(int node, int left, int right, int queryLeft, int queryRight) {
    if (queryRight < left || right < queryLeft) return 0;
    if (queryLeft <= left && right <= queryRight) return sum[node];
    pushDown(node, left, right);
    int mid = (left + right) / 2;
    long long leftSum = querySum(node * 2, left, mid, queryLeft, queryRight);
    long long rightSum = querySum(node * 2 + 1, mid + 1, right, queryLeft, queryRight);
    return leftSum + rightSum;
}

int main() {
    int n, q;
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &initial[i]);
    }
    build(1, 1, n);

    for (int i = 0; i < q; ++i) {
        char type;
        int a, b;
        scanf(" %c%d%d", &type, &a, &b); // %c 前面的空格跳掉行首空白
        if (type == 'P') {
            int c;
            scanf("%d", &c);
            addRange(1, 1, n, a, b, c);
        } else if (type == 'M') {
            printf("%lld\n", queryMin(1, 1, n, a, b));
        } else {
            printf("%lld\n", querySum(1, 1, n, a, b));
        }
    }
    return 0;
}
