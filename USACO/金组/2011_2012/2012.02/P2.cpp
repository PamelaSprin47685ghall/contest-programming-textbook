#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 1000;
const int LIMIT = 10000;       // 坐标的绝对值上界
const long long SPAN = 20001;  // 坐标可能取的整数个数, 用它把 (x, y) 压成一个整数

int n;
int xs[MAXN], ys[MAXN];
long long pointKeys[MAXN]; // 所有给定点编码后排序, 用二分查找判断一个点是不是给定点

// 在 [-10000, 10000] 内的点各有唯一的编码, 所以编码相等就代表同一个点
long long encodePoint(long long x, long long y) {
    return (x + LIMIT) * SPAN + (y + LIMIT);
}

bool isGivenPoint(long long x, long long y) {
    long long key = encodePoint(x, y);
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (pointKeys[mid] == key) return true;
        if (pointKeys[mid] < key) low = mid + 1;
        else high = mid - 1;
    }
    return false;
}

long long gcdOf(long long a, long long b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

const int MAXLINE = 2 * MAXN + 5;
long long axisA[MAXLINE], axisB[MAXLINE], axisC[MAXLINE]; // 已确认的对称轴 A x + B y + C = 0
int axisCount = 0;

// 直线用 A x + B y + C = 0 表示. 同一个直线的表示不唯一, 约掉公因数并定号后就能直接比较
void checkAxis(long long A, long long B, long long C) {
    long long denom = A * A + B * B; // 法向量的长度平方, 反射时要用到
    for (int i = 0; i < n; i++) {
        long long offset = A * xs[i] + B * ys[i] + C;
        // 点沿法向量反射: (x, y) -> (x - 2A*offset/denom, y - 2B*offset/denom)
        long long numX = 2 * A * offset;
        long long numY = 2 * B * offset;
        if (numX % denom != 0 || numY % denom != 0) return; // 镜像点不是整点, 这条线不可能对称
        long long rx = xs[i] - numX / denom;
        long long ry = ys[i] - numY / denom;
        if (rx < -LIMIT || rx > LIMIT || ry < -LIMIT || ry > LIMIT) return; // 越界, 不可能是给定点
        if (!isGivenPoint(rx, ry)) return;
    }
    long long g = gcdOf(gcdOf(A, B), C);
    if (g != 0) {
        A /= g;
        B /= g;
        C /= g;
    }
    if (A < 0 || (A == 0 && B < 0)) { // 统一符号, 保证同一条线只有一种写法
        A = -A;
        B = -B;
        C = -C;
    }
    for (int i = 0; i < axisCount; i++) {
        if (axisA[i] == A && axisB[i] == B && axisC[i] == C) return; // 同一条线不重复计数
    }
    axisA[axisCount] = A;
    axisB[axisCount] = B;
    axisC[axisCount] = C;
    axisCount++;
}

// 点 p 与点 q 的中垂线
void checkBisector(int p, int q) {
    long long dx = xs[q] - xs[p];
    long long dy = ys[q] - ys[p];
    checkAxis(2 * dx, 2 * dy, -(dx * (xs[p] + xs[q]) + dy * (ys[p] + ys[q])));
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &xs[i], &ys[i]);
        pointKeys[i] = encodePoint(xs[i], ys[i]);
    }
    sort(pointKeys, pointKeys + n);

    // 对称轴只有三种可能, 取两个固定点 p = 0 号, q = 1 号:
    //   1) 不过 p: 它把 p 映到某个别的点, 所以是 p 与那个点的中垂线;
    //   2) 过 p 但不过 q: 它把 q 映到某个 q 以外的点, 所以是 q 与那个点的中垂线;
    //   3) 同时过 p 和 q: 那就是直线 pq.
    for (int j = 1; j < n; j++) checkBisector(0, j);
    for (int j = 0; j < n; j++) {
        if (j != 1) checkBisector(1, j);
    }
    long long dx = xs[1] - xs[0];
    long long dy = ys[1] - ys[0];
    checkAxis(dy, -dx, dx * ys[0] - dy * xs[0]);

    printf("%d\n", axisCount);
    return 0;
}
