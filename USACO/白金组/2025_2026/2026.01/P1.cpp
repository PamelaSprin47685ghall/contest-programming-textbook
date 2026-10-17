#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

typedef long long ll;

// 方向：把 (h-s, p-s) 用 gcd 约分成最简向量，方向相同当且仅当约分后完全相等
struct Dir {
    ll x, y;   // 最简方向向量
    ll w;      // 落在该方向上的奶牛数
};

// 为什么不用 atan2：坐标可达 1e9，浮点角度会因精度误差把两个相邻方向判反，用叉积比较是精确的
static int halfOf(ll x, ll y) {
    // 上半平面（含正 x 轴）记为 0，下半平面记为 1
    return (y > 0 || (y == 0 && x > 0)) ? 0 : 1;
}

static ll crossOf(ll ax, ll ay, ll bx, ll by) {
    return ax * by - ay * bx;
}

// 按极角从小到大排序
static bool dirCmp(const Dir &a, const Dir &b) {
    int ha = halfOf(a.x, a.y), hb = halfOf(b.x, b.y);
    if (ha != hb) return ha < hb;
    return crossOf(a.x, a.y, b.x, b.y) > 0;
}

static ll gcdll(ll a, ll b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) {
        ll t = a % b;
        a = b;
        b = t;
    }
    return a;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int N;
        scanf("%d", &N);
        vector<Dir> all;
        all.reserve(N);
        for (int i = 0; i < N; i++) {
            ll h, p, s;
            scanf("%lld %lld %lld", &h, &p, &s);
            // 为什么这样化简：胜负不等式只依赖 (h-s, p-s)，三者同时加常数不改变分布
            ll x = h - s, y = p - s;
            // 均匀分布(1/3,1/3,1/3)的奶牛与所有奶牛打平，不可能出现在任何循环三元组里
            if (x == 0 && y == 0) continue;
            ll g = gcdll(x, y);
            Dir d;
            d.x = x / g;
            d.y = y / g;
            d.w = 1;
            all.push_back(d);
        }

        // 同一方向的奶牛必然排在相邻位置，直接合并成一个方向类
        sort(all.begin(), all.end(), dirCmp);
        vector<Dir> cls;
        for (size_t i = 0; i < all.size(); i++) {
            if (!cls.empty() && cls.back().x == all[i].x && cls.back().y == all[i].y) cls.back().w++;
            else cls.push_back(all[i]);
        }
        int d = (int)cls.size();
        ll total = 0;
        for (int i = 0; i < d; i++) total += cls[i].w;

        // S3：三个方向互不相同的奶牛三元组总数（用初等对称和滚动累加）
        ll e1 = 0, e2 = 0, S3 = 0;
        for (int i = 0; i < d; i++) {
            ll w = cls[i].w;
            S3 += w * e2;
            e2 += w * e1;
            e1 += w;
        }

        // Santi：含有一对相反方向（这对奶牛互相打平）的三元组数
        // 每对相反方向贡献 w_a*w_b*(total-w_a-w_b)：第三头奶牛取其它任意方向
        // 每个无序对会被两个方向各统计一次，所以最后除以 2
        ll Santi2 = 0;
        for (int i = 0; i < d; i++) {
            Dir target;
            target.x = -cls[i].x;
            target.y = -cls[i].y;
            target.w = 0;
            int lo = 0, hi = d;
            while (lo < hi) {
                int mid = (lo + hi) / 2;
                if (dirCmp(cls[mid], target)) lo = mid + 1;
                else hi = mid;
            }
            if (lo < d && cls[lo].x == target.x && cls[lo].y == target.y)
                Santi2 += cls[i].w * cls[lo].w * (total - cls[i].w - cls[lo].w);
        }
        ll Santi = Santi2 / 2;

        // U：三对关系全部分出胜负（无打平）的无序三元组数
        ll U = S3 - Santi;

        // OPEN：三个方向落在某个开半平面内的三元组数（这类三元组必是"传递"型，不是循环）
        // 每个这样的三元组有唯一的最顺时针方向，所以对每个方向只需统计它逆时针 180 度内的方向两两配对
        // 用双倍数组 + 前缀和，并用单调指针定位"逆时针 180 度"的右边界
        vector<ll> pw(2 * d + 1, 0), pw2(2 * d + 1, 0);
        for (int i = 0; i < 2 * d; i++) {
            ll w = cls[i % d].w;
            pw[i + 1] = pw[i] + w;
            pw2[i + 1] = pw2[i] + w * w;
        }
        ll OPEN = 0;
        int r = 1;
        for (int i = 0; i < d; i++) {
            if (r < i + 1) r = i + 1;
            // 叉积 > 0 等价于"逆时针夹角严格小于 180 度"，因此不会把相反方向算进来
            while (r < i + d && crossOf(cls[i].x, cls[i].y, cls[r % d].x, cls[r % d].y) > 0) r++;
            ll P = pw[r] - pw[i + 1];   // 窗口内方向类的奶牛数之和
            ll Q = pw2[r] - pw2[i + 1]; // 窗口内方向类的奶牛数平方和
            OPEN += cls[i].w * ((P * P - Q) / 2); // 窗口内两两配对：先取一头奶牛再取另一头，去掉同方向的两头
        }

        // 无打平的三元组分成两类：落在开半平面内的（传递型）和绕满一圈的（循环型）
        printf("%lld\n", U - OPEN);
    }
    return 0;
}
