#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 200005;            // 道路条数上限
const int MAXL = 18;                // 2^18 = 262144 > 2*10^5，链上最多换这么多次路
const long long CAP = 4000000000LL; // 比任何剩余时间都大，用来表示「这一段走不完」

int nV = 0, nH = 0;                 // 竖路、横路的条数
long long V[MAXN], H[MAXN];         // 排序后的坐标；下标 nV / nH 处留作哨兵

// oppV[i]：下标大于 i 的、坐标奇偶性与 V[i] 相反的第一条竖路（没有则为 nV）
int oppV[MAXN], oppH[MAXN];
// parV[q][k]：下标 >= k 的第一条坐标奇偶性为 q 的竖路（没有则为 nV）
int parV[2][MAXN], parH[2][MAXN];
// 倍增表：upV[k][i] 是从竖路 i 出发、连续 2^k 次「换到奇偶性相反的下一条竖路」后的落点
int upV[MAXL][MAXN], upH[MAXL][MAXN];
// 与倍增表配套的距离和（超过 CAP 就截断，因为 CAP 已经大于任何可能的剩余时间）
long long sumV[MAXL][MAXN], sumH[MAXL][MAXN];

void buildOne(long long *A, int n, int *opp, int par[2][MAXN],
              int up[MAXL][MAXN], long long sum[MAXL][MAXN]) {
    opp[n] = n;
    for (int i = n - 1; i >= 0; --i) {
        if (i + 1 >= n) {
            opp[i] = n;
        } else if ((A[i + 1] ^ A[i]) & 1LL) {
            opp[i] = i + 1;
        } else {
            // A[i+1] 与 A[i] 同奇偶，此时 opp[i+1]（相对 A[i+1] 找相反奇偶）也是
            // 相对 A[i] 的第一个相反奇偶下标，可以直接沿用。
            opp[i] = opp[i + 1];
        }
    }
    par[0][n] = par[1][n] = n;
    for (int i = n - 1; i >= 0; --i) {
        int q = (int)(A[i] & 1LL);
        par[q][i] = i;                  // 自己就满足奇偶性 q
        par[1 - q][i] = par[1 - q][i + 1];
    }
    for (int i = 0; i <= n; ++i) {
        up[0][i] = (i == n ? n : opp[i]);
        sum[0][i] = (up[0][i] == n ? CAP : A[up[0][i]] - A[i]);
    }
    for (int k = 1; k < MAXL; ++k) {
        for (int i = 0; i <= n; ++i) {
            int mid = up[k - 1][i];
            up[k][i] = up[k - 1][mid];
            long long s = sum[k - 1][i] + sum[k - 1][mid];
            sum[k][i] = (s > CAP ? CAP : s);
        }
    }
}

// 当前位置是竖路 V[i] 与横路 H[j] 的交点，剩余时间 d。
// dir == 0 表示此刻正沿横路向东走，dir == 1 表示正沿竖路向北走。
// 此时后面的路线完全由两条「跳向相反奇偶性下一条路」的链决定，可以倍增跳跃。
void finish(int i, int j, long long d, int dir) {
    // 一次「迭代」＝向东走到下一条竖路再向北走到下一条横路（起点向北时顺序相反，
    // 距离之和不变，走完一个迭代后朝向也不变）。
    for (int k = MAXL - 1; k >= 0; --k) {
        int ni = upV[k][i], nj = upH[k][j];
        if (ni == nV || nj == nH) continue;         // 某条链不够 2^k 步
        long long cost = sumV[k][i] + sumH[k][j];
        if (cost > d) continue;
        d -= cost;
        i = ni;
        j = nj;
    }
    // 剩下的距离不足一个完整迭代（或链已到头），逐段走完即可
    while (true) {
        if (dir == 0) {
            int ni = oppV[i];
            if (ni == nV || V[ni] - V[i] > d) {
                printf("%lld %lld\n", V[i] + d, H[j]);
                return;
            }
            d -= V[ni] - V[i];
            i = ni;
            dir = 1;
        } else {
            int nj = oppH[j];
            if (nj == nH || H[nj] - H[j] > d) {
                printf("%lld %lld\n", V[i], H[j] + d);
                return;
            }
            d -= H[nj] - H[j];
            j = nj;
            dir = 0;
        }
    }
}

int main() {
    int n, q;
    scanf("%d %d", &n, &q);
    for (int k = 0; k < n; ++k) {
        char c;
        long long x;
        scanf(" %c %lld", &c, &x);
        if (c == 'V') V[nV++] = x;
        else H[nH++] = x;
    }
    sort(V, V + nV);
    sort(H, H + nH);
    buildOne(V, nV, oppV, parV, upV, sumV);
    buildOne(H, nH, oppH, parH, upH, sumH);

    for (int k = 0; k < q; ++k) {
        long long x, y, d;
        scanf("%lld %lld %lld", &x, &y, &d);
        // 走过的总时间 t 满足 t = (当前横坐标 - x) + (当前纵坐标 - y)，
        // 所以 t 的奇偶性等于 (x + y) 的奇偶性异或上「当前坐标和」的奇偶性。
        // 记 p = (x + y) mod 2：在交点 (v, h) 上，向北走当且仅当 (v + h + p) 为偶数。
        int p = (int)((x + y) & 1LL);

        int iv = (int)(lower_bound(V, V + nV, x) - V);
        bool onV = (iv < nV && V[iv] == x);
        int jh = (int)(lower_bound(H, H + nH, y) - H);
        bool onH = (jh < nH && H[jh] == y);

        if (onV) {
            // 起点在竖路上（若同时落在横路上，因为已走了 0 秒是偶数秒，仍然向北走）。
            // 向北途中，第一条能让它转向东的横路必须满足奇偶性 (p + V[iv] + 1) mod 2。
            int need = (int)((p + (V[iv] & 1LL) + 1) & 1LL);
            int from = (int)(lower_bound(H, H + nH, y + 1) - H);
            int j2 = parH[need][from];
            if (j2 == nH || H[j2] - y > d) {
                printf("%lld %lld\n", V[iv], y + d);
                continue;
            }
            d -= H[j2] - y;
            finish(iv, j2, d, 0);
        } else {
            // 只可能在横路上：沿着横路向东走。
            // 第一条能让它转向北的竖路必须满足奇偶性 (p + H[jh]) mod 2。
            (void)onH;
            int need = (int)((p + (H[jh] & 1LL)) & 1LL);
            int from = (int)(lower_bound(V, V + nV, x + 1) - V);
            int i2 = parV[need][from];
            if (i2 == nV || V[i2] - x > d) {
                printf("%lld %lld\n", x + d, H[jh]);
                continue;
            }
            d -= V[i2] - x;
            finish(i2, jh, d, 1);
        }
    }
    return 0;
}
