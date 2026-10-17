#include <cstdio>
#include <cmath>
#include <set>
#include <algorithm>
#include <utility>
using namespace std;

typedef long long ll;
typedef pair<int, int> pi;   // 一维区间 [first, second]
typedef pair<pi, pi> ppi;    // 一个"坏"区域：x 范围 + y 范围

// 哨兵坐标：比所有真实坐标都大，保证它永远不会被真正跨越，只用来夹住两端
const int BIG = 1000000000;

// 求 floor(sqrt(x))：先用浮点取近似值，再用整数乘法上下校正，
// 这样即使浮点有误差也一定得到精确的整数下取整值
int sqrt_safe(ll x) {
    int s = (int)sqrt((double)x);
    while ((ll)(s + 1) * (s + 1) <= x) ++s;
    while ((ll)s * s > x) --s;
    return s;
}

int N, T, X, Y;
int S[300005], LX[300005], LY[300005];

multiset<ppi> bad;   // 所有"坏"区域，按 (x 区间, y 区间) 的字典序有序
multiset<ll> cands;  // 每对相邻坏区域之间的好通道所对应的最短路径长度

// 把坐标 x 夹进区间 rng，并把移动代价累加进 ans
void clip(ll &ans, pi rng, int &x) {
    if (x < rng.first) { ans += rng.first - x; x = rng.first; }
    if (x > rng.second) { ans += x - rng.second; x = rng.second; }
}

// 左侧坏区域 l 与右侧坏区域 r 夹出的好通道里，从 (X,0) 走到 (0,Y) 的最短长度。
// 因为通道是"好"的，最优路径一定先水平走到 x 范围再斜穿再竖直走到 Y。
ll cost(ppi l, ppi r) {
    int cx = X, cy = Y;
    ll ans = 0;
    clip(ans, make_pair(l.first.second, r.first.first), cx);
    clip(ans, make_pair(l.second.second, r.second.first), cy);
    ans += sqrt_safe((ll)cx * cx + (ll)cy * cy);
    return ans;
}

// 往候选集合里加入 / 删除一条路径长度
void add_cost(ppi l, ppi r, int sgn) {
    ll c = cost(l, r);
    if (sgn > 0) {
        cands.insert(c);
    } else {
        multiset<ll>::iterator it = cands.find(c);
        if (it != cands.end()) cands.erase(it);
    }
}

// a、b 两个坏区域是否相交。不相交当且仅当 a 整体落在 b 的左下方，
// 此时中间的矩形区域全部是"好"的，必有一条路径只穿过这个通道
bool isect(ppi a, ppi b) {
    return !(a.first.second <= b.first.first && a.second.second <= b.second.first);
}

// 迭代器 it 所指的坏区域即将被插入 (sgn=1) 或删除 (sgn=-1)：
// 维护它与左右邻居之间、以及左右邻居彼此之间的候选值
void upd(set<ppi>::iterator it, int sgn) {
    bool has_prev = (it != bad.begin());
    set<ppi>::iterator nx = it;
    ++nx;
    bool has_next = (nx != bad.end());
    set<ppi>::iterator pv = it;
    if (has_prev) --pv;
    if (has_prev) add_cost(*pv, *it, sgn);
    if (has_next) add_cost(*it, *nx, sgn);
    // 去掉 it 后，左右邻居彼此相邻，需要补上它们之间的候选值（反之亦然）
    if (has_prev && has_next) add_cost(*pv, *nx, -sgn);
}

// 插入一条新的视线 x-y：把它当作只含该线段的坏区域，
// 与所有相交的坏区域合并成一个更大的坏区域后再放回集合
void ins_line(int x, int y) {
    ppi p(make_pair(x, x), make_pair(y, y));
    while (true) {
        set<ppi>::iterator it = bad.lower_bound(p);
        if (it != bad.end() && isect(p, *it)) {
            upd(it, -1);
            ppi q = *it;
            bad.erase(it);
            p.first.first = min(p.first.first, q.first.first);
            p.first.second = max(p.first.second, q.first.second);
            p.second.first = min(p.second.first, q.second.first);
            p.second.second = max(p.second.second, q.second.second);
            continue;
        }
        if (it != bad.begin()) {
            set<ppi>::iterator pr = it;
            --pr;
            if (isect(*pr, p)) {
                upd(pr, -1);
                ppi q = *pr;
                bad.erase(pr);
                p.first.first = min(p.first.first, q.first.first);
                p.first.second = max(p.first.second, q.first.second);
                p.second.first = min(p.second.first, q.second.first);
                p.second.second = max(p.second.second, q.second.second);
                continue;
            }
        }
        break;
    }
    set<ppi>::iterator it = bad.insert(p);
    upd(it, 1);
}

int main() {
    scanf("%d%d", &N, &T);
    scanf("%d%d", &X, &Y);
    for (int i = 0; i < N; ++i) {
        scanf("%d%d%d", &S[i], &LX[i], &LY[i]);
    }
    // 初始整个平面都是"好"的，用两个退化到一个点的坏区域把它夹住
    ins_line(0, 0);
    ins_line(BIG, BIG);

    // s 单调不减，用一个指针按时间顺序逐条加入视线即可
    int ptr = 0;
    for (int t = 0; t < T; ++t) {
        while (ptr < N && S[ptr] <= t) {
            ins_line(LX[ptr], LY[ptr]);
            ++ptr;
        }
        printf("%d\n", (int)*cands.begin());
    }
    return 0;
}
