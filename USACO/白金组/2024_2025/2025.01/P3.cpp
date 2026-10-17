#include <cstdio>
#include <deque>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> pll;

const int MAXN = 500005;
const ll INF = (ll)1e18;

int n;
ll w[MAXN], c[MAXN];

// 维护 dp 的差分数组, 用若干 "长度 + 公差" 的连续段存进双端队列.
// tot      : 差分数组的总长度 (即 dp 的状态数, 超过上界时用 INF 段表示无限长)
// totdiff  : 所有段贡献的差分和 (用来在反转时把 dp 的基准值翻转)
// start    : dp 数组首项的值 (即当前前缀的答案)
// lz       : 加操作留下的懒标记, 段里存的是真实公差减去 lz
// rev      : 差分数组是否处于反转方向
ll tot, totdiff, start, lz;
int rev;
deque<pll> seg; // first = 段长, second = 段的公差 (真实公差为 second + lz)

// 把差分数组的长度截到 / 扩到 x, 同时反转方向
// (对应 "reverse 操作 + cut 操作" 的合并, 先把数组长度调整到 x 再翻转)
void doReverse(ll x) {
    if (!rev) {
        // 当前方向没有翻转: 从尾部削段, 削下来的段公差必须 <= 0 (即 dp 单调不减)
        pll last(-1, -1);
        while (tot > x && !seg.empty()) {
            last = seg.back();
            seg.pop_back();
            if (last.second + lz <= 0) {
                // 整个 dp 的最小值落在切点之后, 差分数组可以整体清零
                rev = 1; start += totdiff; totdiff = 0; tot = INF; lz = 0;
                seg.clear(); seg.push_back(pll(INF, 0));
                return;
            }
            tot -= last.first;
            totdiff -= last.first * (last.second + lz);
        }
        if (tot < x) {
            // 长度不够, 用最后削掉的段的公差补一段 (它正是切点处的公差)
            seg.push_back(pll(x - tot, last.second));
            totdiff += (x - tot) * (last.second + lz);
        }
        rev = 1; start += totdiff; totdiff = -totdiff; tot = x;
        // 反转后原来的差分全部取反, 队首那些变成负数的段不再需要, 直接丢掉
        while (!seg.empty() && seg.front().second + lz < 0) {
            tot -= seg.front().first;
            totdiff += seg.front().first * (seg.front().second + lz);
            seg.pop_front();
        }
        seg.push_front(pll(INF, -lz)); // 补一段差值恒为 0 的无限长段
        tot += INF;
    } else {
        // 已经翻转: 从队首 (也就是当前方向的尾部) 削段
        pll last(-1, -1);
        while (tot > x && !seg.empty()) {
            last = seg.front();
            seg.pop_front();
            if (last.second + lz >= 0) {
                rev = 0; start += totdiff; totdiff = 0; tot = INF; lz = 0;
                seg.clear(); seg.push_back(pll(INF, 0));
                return;
            }
            tot -= last.first;
            totdiff += last.first * (last.second + lz);
        }
        if (tot < x) {
            seg.push_front(pll(x - tot, last.second));
            totdiff -= (x - tot) * (last.second + lz);
        }
        rev = 0; start += totdiff; totdiff = -totdiff; tot = x;
        while (!seg.empty() && seg.back().second + lz > 0) {
            tot -= seg.back().first;
            totdiff -= seg.back().first * (seg.back().second + lz);
            seg.pop_back();
        }
        seg.push_back(pll(INF, -lz));
        tot += INF;
    }
}

// 给全部差分加上 p (即 dp[j] += p*j), 只打懒标记
void add(ll p) {
    if (!rev) {
        lz += p;
        totdiff += p * tot;
    } else {
        lz -= p;
        totdiff += p * tot;
    }
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%lld", &w[i]);
    for (int i = 0; i + 1 < n; i++) scanf("%lld", &c[i]);

    seg.push_back(pll(INF, INF));
    tot = INF, start = 0, rev = 0;
    // 1e18 * 1e18 按 2^64 取模后的值；写成常量避免有符号溢出 UB，行为与原式一致。
    totdiff = -5527149226598858752LL;
    doReverse(w[0]);
    for (int i = 1; i < n; i++) {
        add(c[i - 1]);
        doReverse(w[i]);
        printf("%lld\n", start);
    }
    return 0;
}
