#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 200005;

struct Event {
    long long v;       // 45 度旋转后的坐标 t - x
    long long u;       // 45 度旋转后的坐标 t + x
    long long amount;  // 奶牛/苹果的数量
};

int n;
Event cows[MAXN], apples[MAXN];
long long cowU[MAXN];       // 所有奶牛 u 坐标（用来离散化）
long long segTree[4 * MAXN];  // 线段树：每个 u 位置还剩多少头可用奶牛
int segSize;

bool byV(const Event &a, const Event &b) {
    if (a.v != b.v) {
        return a.v < b.v;
    }
    return a.u < b.u;
}

// 线段树上位置 pos 的奶牛数增加 delta（delta 可以为负）
void addCows(int pos, long long delta) {
    int p = pos + segSize;
    segTree[p] += delta;
    for (p >>= 1; p >= 1; p >>= 1) {
        segTree[p] = segTree[2 * p] + segTree[2 * p + 1];
    }
}

// 在 [0, limit] 中找最靠右的非零位置；没有则返回 -1。
// 先搜右子树，所以找到的就是 u 尽量大的那头奶牛。
int findRightmost(int node, int left, int right, int limit) {
    if (left > limit || segTree[node] == 0) {
        return -1;
    }
    if (left == right) {
        return left;
    }
    int mid = (left + right) >> 1;
    int res = findRightmost(2 * node + 1, mid + 1, right, limit);
    if (res != -1) {
        return res;
    }
    return findRightmost(2 * node, left, mid, limit);
}

int main() {
    scanf("%d", &n);
    int cowCount = 0, appleCount = 0;
    for (int i = 0; i < n; ++i) {
        int q;
        long long t, x, amount;
        scanf("%d %lld %lld %lld", &q, &t, &x, &amount);
        Event e;
        // 把 (x, t) 顺时针旋转 45 度：奶牛 (u, v) 能抓到苹果 (u', v')
        // 当且仅当 u <= u' 且 v <= v'，原来的"V 形可达区域"变成了一个象限。
        e.v = t - x;
        e.u = t + x;
        e.amount = amount;
        if (q == 1) {
            cows[cowCount++] = e;
        } else {
            apples[appleCount++] = e;
        }
    }
    sort(cows, cows + cowCount, byV);
    sort(apples, apples + appleCount, byV);

    for (int i = 0; i < cowCount; ++i) {
        cowU[i] = cows[i].u;
    }
    sort(cowU, cowU + cowCount);
    int distinctU = unique(cowU, cowU + cowCount) - cowU;

    segSize = 1;
    while (segSize < distinctU) {
        segSize <<= 1;
    }
    for (int i = 0; i < 2 * segSize; ++i) {
        segTree[i] = 0;
    }

    long long answer = 0;
    int nextCow = 0;
    for (int a = 0; a < appleCount; ++a) {
        // 只有 v 不超过当前苹果 v 的奶牛才可能抓到它
        while (nextCow < cowCount && cows[nextCow].v <= apples[a].v) {
            int pos = lower_bound(cowU, cowU + distinctU, cows[nextCow].u) - cowU;
            addCows(pos, cows[nextCow].amount);
            ++nextCow;
        }
        long long remaining = apples[a].amount;
        // 对当前这组苹果，反复取 u 不超过苹果 u 的奶牛中 u 最大的那头。
        // 这头奶牛能覆盖的苹果集合包含其他任何可行奶牛的集合，所以贪心最优。
        int limit = upper_bound(cowU, cowU + distinctU, apples[a].u) - cowU - 1;
        while (remaining > 0) {
            int pos = findRightmost(1, 0, segSize - 1, limit);
            if (pos == -1) {
                break;
            }
            long long available = segTree[pos + segSize];
            long long take = available < remaining ? available : remaining;
            answer += take;
            remaining -= take;
            addCows(pos, -take);
        }
    }
    printf("%lld\n", answer);
    return 0;
}
