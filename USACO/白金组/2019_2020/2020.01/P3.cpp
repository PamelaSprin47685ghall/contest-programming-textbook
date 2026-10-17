#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 200005;

struct Line {
    long long y;    // 初始 y 坐标 A
    long long d;    // 斜率 -(下标+1)
    int ind;        // 世界编号 (从 0 开始)
};

int N;
long long A[MAXN];
int Q[MAXN];
long long num[MAXN], dem[MAXN];

Line all[MAXN];     // 按世界编号存放, 便于取 "目标世界" 的那条直线
Line li[MAXN];      // 按 A 升序排序后的直线
Line hull[MAXN];    // 单调栈维护的包络
int hullSize;

long long AbsLL(long long x) { return x < 0 ? -x : x; }

long long Gcd(long long a, long long b) {
    while (b != 0) {
        long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

// 比较两条直线与 c 的交点时刻: t(a,c) 与 t(b,c). 返回 -1 / 0 / 1
int CmpT(const Line &a, const Line &b, const Line &c) {
    long long ln = AbsLL(a.y - c.y), ld = AbsLL(a.d - c.d);
    long long rn = AbsLL(b.y - c.y), rd = AbsLL(b.d - c.d);
    long long res = ln * rd - rn * ld;    // 交叉相乘, 最大约 1e9 * 2e5, 不溢出
    if (res < 0) return -1;
    if (res == 0) return 0;
    return 1;
}

// b 是否落在 a 与 c 之间的包络上
bool Used(const Line &a, const Line &b, const Line &c) {
    long long ln = AbsLL(a.y - b.y), ld = AbsLL(a.d - b.d);
    long long rn = AbsLL(b.y - c.y), rd = AbsLL(b.d - c.d);
    return ln * rd - rn * ld < 0;
}

bool CmpY(const Line &a, const Line &b) { return a.y < b.y; }

int main() {
    scanf("%d", &N);
    for (int i = 0; i < N; i++) scanf("%lld", &A[i]);
    for (int i = 0; i < N; i++) {
        scanf("%d", &Q[i]);
        Q[i]--;      // 改成 0 起下标
    }
    for (int i = 0; i < N; i++) {
        all[i].y = A[i];
        all[i].d = -(i + 1);
        all[i].ind = i;
        li[i] = all[i];
    }
    sort(li, li + N, CmpY);

    // 情况一: 目标世界起点更低 (A[Q] < A[i]), 只用起点不低于 i 的世界, 按 A 递减加入
    hullSize = 0;
    for (int i = N - 1; i >= 0; i--) {
        Line now = li[i];
        while (hullSize > 0) {
            if (now.d < hull[hullSize - 1].d) { hullSize--; continue; }
            if (hullSize > 1 && !Used(now, hull[hullSize - 1], hull[hullSize - 2])) { hullSize--; continue; }
            break;
        }
        hull[hullSize++] = now;
        int ind = li[i].ind;
        int tq = Q[ind];
        if (A[ind] > A[tq]) {
            if (hull[0].d > -(long long)(tq + 1)) num[ind] = -1;   // 包络上没有任何世界能追上目标
            else {
                int lo = 0, hi = hullSize - 1;
                while (lo < hi) {
                    int mid = (lo + hi) / 2;
                    int l = mid, r = mid + 1;
                    if (hull[r].d > -(long long)(tq + 1)) { hi = l; continue; }
                    int res = CmpT(hull[l], hull[r], all[tq]);
                    if (res < 0) hi = l;
                    else if (res == 0) { lo = l; hi = l; }
                    else lo = r;
                }
                long long n0 = AbsLL(hull[lo].y - all[tq].y);
                long long d0 = AbsLL(hull[lo].d - all[tq].d);
                long long g = Gcd(n0, d0);
                num[ind] = n0 / g;
                dem[ind] = d0 / g;
            }
        }
    }

    // 情况二: 目标世界起点更高 (A[Q] > A[i]), 只用起点不高于 i 的世界, 按 A 递增加入
    hullSize = 0;
    for (int i = 0; i < N; i++) {
        Line now = li[i];
        while (hullSize > 0) {
            if (now.d > hull[hullSize - 1].d) { hullSize--; continue; }
            if (hullSize > 1 && !Used(now, hull[hullSize - 1], hull[hullSize - 2])) { hullSize--; continue; }
            break;
        }
        hull[hullSize++] = now;
        int ind = li[i].ind;
        int tq = Q[ind];
        if (A[ind] < A[tq]) {
            if (hull[0].d < -(long long)(tq + 1)) num[ind] = -1;
            else {
                int lo = 0, hi = hullSize - 1;
                while (lo < hi) {
                    int mid = (lo + hi) / 2;
                    int l = mid, r = mid + 1;
                    if (hull[r].d < -(long long)(tq + 1)) { hi = l; continue; }
                    int res = CmpT(hull[l], hull[r], all[tq]);
                    if (res < 0) hi = l;
                    else if (res == 0) { lo = l; hi = l; }
                    else lo = r;
                }
                long long n0 = AbsLL(hull[lo].y - all[tq].y);
                long long d0 = AbsLL(hull[lo].d - all[tq].d);
                long long g = Gcd(n0, d0);
                num[ind] = n0 / g;
                dem[ind] = d0 / g;
            }
        }
    }

    for (int i = 0; i < N; i++) {
        if (num[i] == -1) printf("-1\n");
        else printf("%lld/%lld\n", num[i], dem[i]);
    }
    return 0;
}
