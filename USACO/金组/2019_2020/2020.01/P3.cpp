#include <cstdio>

using namespace std;

const int MAXP = 100005;
const int MAXE = 2 * MAXP;
const long long INF = 1000000000000000000LL;

int N, P;

struct Event {
    int x, y;
    int kind;               // 0 = 跳板落点（插入），1 = 跳板起点（查询）
    int id;
};

Event ev[MAXE], evBuf[MAXE];
int evCnt;

int sx[MAXP], sy[MAXP], lx[MAXP], ly[MAXP];   // 起点 (sx,sy)，落点 (lx,ly)
long long dp[MAXP];                            // 走到第 i 块跳板起点所需的最小步行距离

int ys[MAXP + 1], ysBuf[MAXP + 1];             // 落点 y 坐标 + 一个虚落点 y=0，用于离散化
int ysc;                                       // 离散化后的不同 y 值个数

long long bit[MAXP + 2];                       // 前缀最小值树状数组，下标 1..ysc

int cmpEvent(const Event &a, const Event &b) {
    if (a.x != b.x) return a.x < b.x ? -1 : 1;
    if (a.y != b.y) return a.y < b.y ? -1 : 1;
    if (a.kind != b.kind) return a.kind < b.kind ? -1 : 1;
    return 0;
}

void sortEvents(int lo, int hi) {              // 归并排序 [lo, hi)
    if (hi - lo <= 1) return;
    int mid = lo + (hi - lo) / 2;
    sortEvents(lo, mid);
    sortEvents(mid, hi);
    int i = lo, j = mid, k = lo;
    while (i < mid && j < hi) {
        if (cmpEvent(ev[i], ev[j]) <= 0) evBuf[k++] = ev[i++];
        else evBuf[k++] = ev[j++];
    }
    while (i < mid) evBuf[k++] = ev[i++];
    while (j < hi) evBuf[k++] = ev[j++];
    for (int t = lo; t < hi; t++) ev[t] = evBuf[t];
}

void sortInts(int *a, int *tmp, int lo, int hi) {   // 归并排序 [lo, hi)
    if (hi - lo <= 1) return;
    int mid = lo + (hi - lo) / 2;
    sortInts(a, tmp, lo, mid);
    sortInts(a, tmp, mid, hi);
    int i = lo, j = mid, k = lo;
    while (i < mid && j < hi) {
        if (a[i] <= a[j]) tmp[k++] = a[i++];
        else tmp[k++] = a[j++];
    }
    while (i < mid) tmp[k++] = a[i++];
    while (j < hi) tmp[k++] = a[j++];
    for (int t = lo; t < hi; t++) a[t] = tmp[t];
}

int posOf(int v) {                             // ys 中值为 v 的位置（1 起）
    int lo = 1, hi = ysc;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (ys[mid] < v) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

int prefixOf(int v) {                          // ys 中 <= v 的元素个数
    int lo = 0, hi = ysc;
    while (lo < hi) {
        int mid = lo + (hi - lo + 1) / 2;
        if (ys[mid] <= v) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

void bitUpdate(int pos, long long val) {        // 单点插入，取最小值
    for (int p = pos; p <= ysc; p += p & (-p))
        if (val < bit[p]) bit[p] = val;
}

long long bitQuery(int pos) {                   // 前缀最小值
    long long res = INF;
    for (int p = pos; p > 0; p -= p & (-p))
        if (bit[p] < res) res = bit[p];
    return res;
}

int main() {
    scanf("%d %d", &N, &P);
    evCnt = 0;
    ysc = 0;
    ys[++ysc] = 0;                              // 虚落点：不使用任何跳板，额外代价 0
    for (int i = 0; i < P; i++) {
        scanf("%d %d %d %d", &sx[i], &sy[i], &lx[i], &ly[i]);
        if (sx[i] == lx[i] && sy[i] == ly[i]) continue;   // 原地跳板不可能改善答案，忽略
        ev[evCnt].x = sx[i]; ev[evCnt].y = sy[i]; ev[evCnt].kind = 1; ev[evCnt].id = i; evCnt++;
        ev[evCnt].x = lx[i]; ev[evCnt].y = ly[i]; ev[evCnt].kind = 0; ev[evCnt].id = i; evCnt++;
        ys[++ysc] = ly[i];
    }
    sortEvents(0, evCnt);
    sortInts(ys, ysBuf, 1, ysc + 1);
    int m = 1;
    for (int t = 2; t <= ysc; t++)
        if (ys[t] != ys[m]) ys[++m] = ys[t];
    ysc = m;

    for (int p = 1; p <= ysc; p++) bit[p] = INF;
    bitUpdate(posOf(0), 0);

    for (int t = 0; t < evCnt; t++) {
        int id = ev[t].id;
        if (ev[t].kind == 0) {
            bitUpdate(posOf(ly[id]), dp[id] - lx[id] - ly[id]);
        } else {
            // 直接走到起点是 sx+sy；或先借某块可用跳板 j（ly[j] <= sy，且 lx[j] <= sx 已由扫描顺序保证）
            dp[id] = (long long)sx[id] + sy[id] + bitQuery(prefixOf(sy[id]));
        }
    }

    // 最后一段路：从最优的最后落点走到 (N,N)，即 2N + (dp - lx - ly)，虚落点代表不用跳板
    printf("%lld\n", 2LL * N + bitQuery(ysc));
    return 0;
}
