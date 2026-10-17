#include <cstdio>
#include <algorithm>

using namespace std;

typedef long long ll;

const int MAXSEG = 256; // 每个方向压缩后的段数上界（斐波那契式最坏约 90 段）

inline ll AbsLL(ll v) { return v < 0 ? -v : v; }

// ============================================================
// 通用情形：两个数对都为正，(x, y) 反复"大减小"，问多少步能变成
// (tx, ty)。每一步的选择是唯一的（只能从较大的数里减去较小的数），
// 否则会变成 0 或负数，再也回不到正数目标。
// 连续的同一种减法用整除一次性跳过。
// ============================================================
ll SubtractChain(ll x, ll y, ll tx, ll ty) {
    ll steps = 0;
    while (true) {
        if (x == tx && y == ty) return steps;
        if (x < tx || y < ty) return -1; // 两个坐标都只会变小
        if (x == y) return -1;           // 再减就出现 0
        if (x > y) {
            if (x == tx) return -1; // x 已就位，但接下来只能继续减 x
            if (y == ty) {
                if ((x - tx) % y != 0) return -1;
                return steps + (x - tx) / y;
            }
            // 还没等关系翻转，x 就会跌破 tx，之后不可能追上
            if ((x - tx) / y < (x - 1) / y) return -1;
            ll runLen = (x - 1) / y; // 一直减到 x <= y 为止的步数
            x -= runLen * y;
            steps += runLen;
        } else {
            if (y == ty) return -1;
            if (x == tx) {
                if ((y - ty) % x != 0) return -1;
                return steps + (y - ty) / x;
            }
            if ((y - ty) / x < (y - 1) / x) return -1;
            ll runLen = (y - 1) / x;
            y -= runLen * x;
            steps += runLen;
        }
    }
}

// ============================================================
// 情形二：起点 (a, b) 满足 a > 0 > b，终点 (c, d) 两坐标均为正。
//
// 路径必然先经过一段"第二坐标非正"的前缀（此时第一坐标必须保持为正），
// 再在某一步 b += a 让第二坐标变正，之后两个坐标全为正，走法唯一
// （从终点倒推时每步的减法选择唯一）。
//
// 于是：
//  - 正向前缀的"可以跳出去的决策点"压缩成若干等差数列（对角方向）；
//  - 从终点倒推的正数链压缩成若干等差数列（轴向）；
//  - 两族段都按坐标和的区间有序，用双指针找重叠区间，段内 O(1) 解交点。
// ============================================================
struct Segment {
    ll px, py; // 段首状态
    ll sx, sy; // 每一步两个坐标各自的增量
    ll count;  // 段内状态个数
    ll base;   // 到达段首状态所需的步数
    ll loSum, hiSum;
};

Segment fwdSeg[MAXSEG], bwdSeg[MAXSEG];
int fwdCnt, bwdCnt;
ll bestMeet;

void AddFwd(ll px, ll py, ll sx, ll sy, ll count, ll base) {
    Segment &s = fwdSeg[fwdCnt++];
    s.px = px; s.py = py; s.sx = sx; s.sy = sy;
    s.count = count; s.base = base;
    s.hiSum = px + py;
    s.loSum = px + (count - 1) * sx + py + (count - 1) * sy;
}

void AddBwd(ll px, ll py, ll sx, ll sy, ll count, ll base) {
    Segment &s = bwdSeg[bwdCnt++];
    s.px = px; s.py = py; s.sx = sx; s.sy = sy;
    s.count = count; s.base = base;
    s.hiSum = px + py;
    s.loSum = px + (count - 1) * sx + py + (count - 1) * sy;
}

// 在两条等差数列上找公共状态，更新最小总步数
void TryMatch(const Segment &f, const Segment &b) {
    ll u = -1, v = -1;
    if (b.count == 1) {
        if (f.sx == 0) {
            if (f.px != b.px || f.py != b.py) return;
            u = 0; v = 0;
        } else {
            if ((b.px - f.px) % f.sx != 0) return;
            u = (b.px - f.px) / f.sx;
            if (u < 0 || u >= f.count) return;
            if (f.py + u * f.sy != b.py) return;
            v = 0;
        }
    } else if (b.sx == 0) {
        // 后向段竖直：第一坐标恒为 b.px
        if (f.sx == 0) {
            if (f.px != b.px) return;
            if ((f.py - b.py) % b.sy != 0) return;
            v = (f.py - b.py) / b.sy;
            if (v < 0 || v >= b.count) return;
            u = 0;
        } else {
            if ((b.px - f.px) % f.sx != 0) return;
            u = (b.px - f.px) / f.sx;
            if (u < 0 || u >= f.count) return;
            ll fy = f.py + u * f.sy;
            if ((fy - b.py) % b.sy != 0) return;
            v = (fy - b.py) / b.sy;
            if (v < 0 || v >= b.count) return;
        }
    } else {
        // 后向段水平：第二坐标恒为 b.py
        if (f.sx == 0) {
            if (f.py != b.py) return;
            if ((f.px - b.px) % b.sx != 0) return;
            v = (f.px - b.px) / b.sx;
            if (v < 0 || v >= b.count) return;
            u = 0;
        } else {
            if ((b.py - f.py) % f.sy != 0) return;
            u = (b.py - f.py) / f.sy;
            if (u < 0 || u >= f.count) return;
            ll fx = f.px + u * f.sx;
            if ((fx - b.px) % b.sx != 0) return;
            v = (fx - b.px) / b.sx;
            if (v < 0 || v >= b.count) return;
        }
    }
    ll cost = f.base + u + b.base + v;
    if (bestMeet < 0 || cost < bestMeet) bestMeet = cost;
}

ll SolveCaseTwo(ll a, ll b, ll c, ll d) {
    fwdCnt = 0;
    bwdCnt = 0;
    bestMeet = -1;

    // ---- 正向前缀：枚举所有"可以通过 b += a 跳进正数区"的状态 ----
    {
        ll A = a, B = b; // A > 0 > B
        ll cnt = 0;      // 到达当前状态 (A, B) 的步数
        while (true) {
            if (B == 0) {
                AddFwd(A, A, 0, 0, 1, cnt + 1); // (A,0) -> (A,A)
                break;
            }
            ll m = -B;
            if (A == m) { // 相等时只能 b+=a 两次：(A,-A)->(A,0)->(A,A)
                AddFwd(m, m, 0, 0, 1, cnt + 2);
                break;
            }
            if (A > m) {
                // |A| > |B| 时每个状态都是一个决策点：此刻可以 b += a 跳出，
                // 或者继续 a += b。连续决策点压缩成等差数列。
                ll q = (A - 1) / m;
                AddFwd(A, A - m, -m, -m, q, cnt + 1);
                A -= q * m;
                cnt += q;
                if (A == m) {
                    AddFwd(A, A, 0, 0, 1, cnt + 2);
                    break;
                }
            }
            // 现在 A < m：只能连续做 b += a（唯一不致死的选择）
            m = -B;
            ll t = m / A;
            if (m % A == 0) {
                // t-1 步后到 (A,-A)，再两步到 (A,A)
                AddFwd(A, A, 0, 0, 1, cnt + (t - 1) + 2);
                break;
            }
            B = -(m % A);
            cnt += t;
        }
    }

    // ---- 终点倒推的正数链：直到 (gcd, gcd) 为止 ----
    {
        ll x = c, y = d;
        ll dist = 0;
        while (x != y) {
            if (x > y) {
                ll k = (x - 1) / y; // 减 k 次后 x 落到 [1, y]
                AddBwd(x, y, -y, 0, k, dist);
                x -= k * y;
                dist += k;
            } else {
                ll k = (y - 1) / x;
                AddBwd(x, y, 0, -x, k, dist);
                y -= k * x;
                dist += k;
            }
        }
        AddBwd(x, y, 0, 0, 1, dist); // 终点 (gcd, gcd)
    }

    // 两族段都按"坐标和区间"严格递减排列，双指针只比较区间重叠的段对
    int j = 0;
    for (int i = 0; i < fwdCnt; ++i) {
        while (j < bwdCnt && bwdSeg[j].loSum > fwdSeg[i].hiSum) ++j;
        for (int jj = j; jj < bwdCnt && bwdSeg[jj].hiSum >= fwdSeg[i].loSum; ++jj) {
            TryMatch(fwdSeg[i], bwdSeg[jj]);
        }
    }
    return bestMeet;
}

// ============================================================
// 主求解：处理 0 坐标与符号规约后分派到具体情形
// ============================================================
ll Solve(ll a, ll b, ll c, ll d) {
    ll x[2], y[2];
    x[0] = a; x[1] = b;
    y[0] = c; y[1] = d;
    if (x[0] == y[0] && x[1] == y[1]) return 0;

    ll cnt = 0;
    // 起点含 0：唯一有用的第一步是把非零坐标加过来（加 0 是空操作）
    for (int i = 0; i < 2; ++i) {
        if (x[i] == 0) {
            x[i] += x[1 - i];
            ++cnt;
        }
    }
    if (x[0] == 0 && x[1] == 0) return -1; // 永远停在 (0,0)
    if (x[0] == y[0] && x[1] == y[1]) return cnt;

    // 终点含 0：最后一步必然是把非零坐标加到零坐标上，倒推一步
    int zeroCnt = 0;
    for (int i = 0; i < 2; ++i) {
        if (y[i] == 0) {
            y[i] -= y[1 - i];
            ++cnt;
            ++zeroCnt;
        }
    }
    if (zeroCnt == 2) return -1;
    if (x[0] == y[0] && x[1] == y[1]) return cnt;

    bool xs0 = x[0] > 0, xs1 = x[1] > 0;
    bool ys0 = y[0] > 0, ys1 = y[1] > 0;
    ll res;
    if (xs0 == xs1) {
        // 起点两坐标同号：全程保持同号，终点也须同号同向
        if (xs0 != ys0 || xs1 != ys1) return -1;
        ll sgn = xs0 ? 1 : -1;
        res = SubtractChain(sgn * y[0], sgn * y[1], sgn * x[0], sgn * x[1]);
        return res < 0 ? -1 : cnt + res;
    }
    // 起点异号：交换坐标令 x = (正, 负)
    if (x[0] < 0) {
        swap(x[0], x[1]);
        swap(y[0], y[1]);
    }
    if (y[0] > 0 && y[1] < 0) {
        // 终点同样是 (正, 负)：全程保持该模式，绝对值上做"大减小"
        res = SubtractChain(x[0], -x[1], y[0], -y[1]);
        return res < 0 ? -1 : cnt + res;
    }
    if (y[0] > 0 && y[1] > 0) {
        res = SolveCaseTwo(x[0], x[1], y[0], y[1]);
        return res < 0 ? -1 : cnt + res;
    }
    if (y[0] < 0 && y[1] > 0) return -1; // (正,负) 到不了 (负,正)
    // 终点 (负, 负)：整体取反再交换坐标，化为情形二
    x[0] = -x[0]; x[1] = -x[1];
    y[0] = -y[0]; y[1] = -y[1];
    swap(x[0], x[1]);
    swap(y[0], y[1]);
    res = SolveCaseTwo(x[0], x[1], y[0], y[1]);
    return res < 0 ? -1 : cnt + res;
}

int main() {
    int Q;
    scanf("%d", &Q);
    while (Q--) {
        ll a, b, c, d;
        scanf("%lld %lld %lld %lld", &a, &b, &c, &d);
        printf("%lld\n", Solve(a, b, c, d));
    }
    return 0;
}
