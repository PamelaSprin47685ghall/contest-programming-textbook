#include <cstdio>
#include <map>
#include <set>
using namespace std;

// 题意：每天做 a 个用例要花 3^(a-1) 能量（a>=1，不做则 0）。
// 官方结论：第 i 个需求的答案只取决于需求集合 (m_j, b_j) (j<=i) 与 (0,0) 的“左上凸壳”：
// 在壳上相邻两点 (x1,y1),(x2,y2) 之间，必须新增 y2-y1 个用例，且最优做法是把它们
// 尽可能均摊到 x2-x1 天里（每天 floor 或 ceil 个），由 3^(a-1) 的凸性可知这是最优的。
// 答案就是所有壳边的代价之和，所以只需要动态维护凸壳并维护代价总和。
const long long MOD = 1000000007;

map<long long, long long> ys;   // 凸壳点的纵坐标；不在壳上的横坐标对应 0
set<long long> xs;              // 凸壳点的横坐标（递增），恒包含 (0,0)
long long ans = 0;              // 当前全部壳边的代价之和（模 MOD）

long long pw(long long base, long long e) {
    long long r = 1 % MOD;
    base %= MOD;
    while (e > 0) {
        if (e & 1) r = r * base % MOD;
        base = base * base % MOD;
        e >>= 1;
    }
    return r;
}

// 壳边 (x1,y1)->(x2,y2) 的最小代价：把 dy 个用例分摊到 dx 天，
// 其中 r=dy%dx 天做 q+1 个（单日代价 3^q），其余 dx-r 天做 q 个（单日代价 3^(q-1)）。
long long edge(long long x1, long long x2) {
    long long dx = x2 - x1;
    long long dy = ys[x2] - ys[x1];
    if (dy <= 0) return 0;          // 壳尾可能出现下降边，不需要做任何用例
    long long q = dy / dx, r = dy % dx;
    if (r == 0) {
        return q == 0 ? 0 : pw(3, q - 1) * (dx % MOD) % MOD;
    }
    long long res = pw(3, q) * (r % MOD) % MOD;
    if (q > 0) res = (res + pw(3, q - 1) * ((dx - r) % MOD)) % MOD;
    return res;
}

// 把横坐标 x 加入（del=false）或移出（del=true）凸壳，并同步修正 ans
void upd(long long x, bool del) {
    set<long long>::iterator it = xs.find(x);
    set<long long>::iterator ip = prev(it);
    long long xbef = *ip;
    if (del) {
        ans = (ans - edge(xbef, x) + MOD) % MOD;
        set<long long>::iterator in = next(it);
        if (in != xs.end()) {
            long long xaf = *in;
            ans = (ans + edge(xbef, xaf) - edge(x, xaf) + MOD) % MOD;
        }
        xs.erase(it);
        ys[x] = 0;
    } else {
        ans = (ans + edge(xbef, x)) % MOD;
        set<long long>::iterator in = next(it);
        if (in != xs.end()) {
            long long xaf = *in;
            ans = (ans + edge(x, xaf) - edge(xbef, xaf) + MOD) % MOD;
        }
    }
}

// b 在 a-c 连线之上才是凸壳顶点；返回 true 表示 b 在连线之下或共线，应当删掉
bool redundant(long long a, long long b, long long c) {
    return (ys[b] - ys[a]) * (c - b) <= (ys[c] - ys[b]) * (b - a);
}

void insert_point(long long x, long long y) {
    if (ys[x] && ys[x] >= y) return;   // 同一天已有不更低的壳点，新点被支配
    if (ys[x]) upd(x, 1);              // 旧壳点先摘掉，再和新区间一起重建
    ys[x] = y;
    xs.insert(x);
    upd(x, 0);
    // 插入后只有 x 附近的三元组可能失去凸性；每轮删掉一个非法的中间点后重新扫描，
    // 于是“坏点”会向左或向右逐步传播，最终收敛到正确的凸壳。
    while (true) {
        bool changed = false;
        set<long long>::iterator it = xs.lower_bound(x);
        for (int k = 0; k < 3; k++) {
            set<long long>::iterator i2 = next(it);
            if (i2 != xs.end()) {
                set<long long>::iterator i3 = next(i2);
                if (i3 != xs.end() && redundant(*it, *i2, *i3)) {
                    upd(*i2, 1);
                    changed = true;
                    break;
                }
            }
            if (it == xs.begin()) break;
            --it;
        }
        if (!changed) break;
    }
}

int main() {
    int D;
    scanf("%d", &D);
    xs.insert(0);                  // (0,0) 是最左侧的壳点
    ys[0] = 0;
    for (int i = 0; i < D; i++) {
        long long m, b;
        scanf("%lld %lld", &m, &b);
        insert_point(m, b);
        printf("%lld\n", ans % MOD);
    }
    return 0;
}
