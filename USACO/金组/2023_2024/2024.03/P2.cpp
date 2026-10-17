#include <cstdio>
#include <cstdlib>

using namespace std;

const int MAXN = 200005;

int N;
int L[MAXN];
int R[MAXN];
int K[MAXN];
int lenOrder[MAXN];   // 区间下标按长度从大到小
int kOrder[MAXN];     // 区间下标按 k 从大到小
int rCoord[MAXN];     // r 的坐标压缩（升序去重）
int lCoord[MAXN];     // l 的坐标压缩（升序去重）
int rNum;             // r 坐标个数
int lNum;             // l 坐标个数
int bitR[MAXN + 2];   // 已插入区间的 r 计数
int bitL[MAXN + 2];   // 已插入区间的 l 计数
int ans[MAXN];

int cmpInt(const void *pa, const void *pb) {
    int a = *(const int *)pa;
    int b = *(const int *)pb;
    if (a != b) return a < b ? -1 : 1;
    return 0;
}

// 按区间长度从大到小排序
int cmpLen(const void *pa, const void *pb) {
    int x = *(const int *)pa;
    int y = *(const int *)pb;
    int lx = R[x] - L[x];
    int ly = R[y] - L[y];
    if (lx != ly) return lx > ly ? -1 : 1;
    return x < y ? -1 : (x > y ? 1 : 0);
}

// 按 k 从大到小排序
int cmpK(const void *pa, const void *pb) {
    int x = *(const int *)pa;
    int y = *(const int *)pb;
    if (K[x] != K[y]) return K[x] > K[y] ? -1 : 1;
    return x < y ? -1 : (x > y ? 1 : 0);
}

// 返回升序数组 a 中第一个大于 x 的位置，等价于"<= x 的个数"
int upperBound(int *a, int n, int x) {
    int lo = 0;
    int hi = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (a[mid] <= x) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

void bitAdd(int *bit, int i, int v) {
    for (; i <= N; i += i & (-i)) bit[i] += v;
}

int bitSum(int *bit, int i) {
    int s = 0;
    for (; i > 0; i -= i & (-i)) s += bit[i];
    return s;
}

int main() {
    scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        scanf("%d %d %d", &L[i], &R[i], &K[i]);
        rCoord[i] = R[i];
        lCoord[i] = L[i];
        lenOrder[i] = i;
        kOrder[i] = i;
    }
    qsort(rCoord, N, sizeof(int), cmpInt);
    qsort(lCoord, N, sizeof(int), cmpInt);
    rNum = 0;
    for (int i = 0; i < N; i++) {
        if (i == 0 || rCoord[i] != rCoord[i - 1]) rCoord[rNum++] = rCoord[i];
    }
    lNum = 0;
    for (int i = 0; i < N; i++) {
        if (i == 0 || lCoord[i] != lCoord[i - 1]) lCoord[lNum++] = lCoord[i];
    }
    qsort(lenOrder, N, sizeof(int), cmpLen);
    qsort(kOrder, N, sizeof(int), cmpK);

    // 按 k 从大到小回答：询问的 k 变小时，把长度 >= k 的区间逐个插入 BIT
    int ptr = 0;
    for (int qi = 0; qi < N; qi++) {
        int i = kOrder[qi];
        while (ptr < N && R[lenOrder[ptr]] - L[lenOrder[ptr]] >= K[i]) {
            int j = lenOrder[ptr];
            bitAdd(bitR, upperBound(rCoord, rNum, R[j]), 1);
            bitAdd(bitL, upperBound(lCoord, lNum, L[j]), 1);
            ptr++;
        }
        int total = ptr;  // 长度 >= K[i] 的区间个数（含 i 自己）
        // 长度 >= k_i 且 r_j < l_i + k_i 的区间（右端点太靠左，重叠不足）
        int bad = bitSum(bitR, upperBound(rCoord, rNum, L[i] + K[i] - 1));
        // 长度 >= k_i 且 l_j > r_i - k_i 的区间（左端点太靠右，重叠不足）
        bad += total - bitSum(bitL, upperBound(lCoord, lNum, R[i] - K[i]));
        // 两类"重叠不足"的区间不会同时满足（否则长度 < k_i），可以直接相减
        ans[i] = total - 1 - bad;
    }
    for (int i = 0; i < N; i++) printf("%d\n", ans[i]);
    return 0;
}
