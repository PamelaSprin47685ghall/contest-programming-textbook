#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

typedef long long ll;

// 点在某张网格中的坐标：a、b 是两个基本方向上的坐标，c 用于第三方向（a+b+c）
struct Loc {
    int a, b, c;
};

int N;                       // 当前测试点个数
vector<Loc> loc;             // 当前网格下所有点的坐标
vector<int> ks;              // 待统计的一维坐标
vector<pair<int, int> > grp; // 压缩后的 (坐标, 个数)

// 对 ks 中所有坐标统计：按坐标排序后，逐条"缝隙"累加横跨该缝隙的点对数
ll calc_dir() {
    sort(ks.begin(), ks.end());
    grp.clear();
    for (size_t i = 0; i < ks.size(); i++) {
        if (i == 0 || ks[i] != ks[i - 1]) grp.push_back(make_pair(ks[i], 1));
        else grp.back().second++;
    }
    ll ret = 0;
    int pos = grp[0].first;
    ll cnt = grp[0].second;
    for (size_t i = 1; i < grp.size(); i++) {
        // 两个相邻坐标之间的所有平行边都要被 cnt 个点与 N-cnt 个点之间的路径各跨一次
        ret += (ll)(grp[i].first - pos) * cnt * (N - cnt);
        cnt += grp[i].second;
        pos = grp[i].first;
    }
    return ret;
}

// 统计当前网格中所有点对的曼哈顿型距离之和（三个方向独立相加）
ll calc() {
    ll ret = 0;
    ks.clear();
    for (size_t i = 0; i < loc.size(); i++) ks.push_back(loc[i].a);
    ret += calc_dir();
    ks.clear();
    for (size_t i = 0; i < loc.size(); i++) ks.push_back(loc[i].b);
    ret += calc_dir();
    ks.clear();
    for (size_t i = 0; i < loc.size(); i++) ks.push_back(loc[i].a + loc[i].b + loc[i].c);
    ret += calc_dir();
    return ret;
}

// 处理一个方向的菱形链：同一条链上不同菱形之间的真实距离比两张六边形网格之和多 1，
// 所以把整条链内所有点对计入，再扣掉落在同一菱形（链坐标相同）内的点对
void chains(vector<pair<int, int> > &v, ll &ans) {
    sort(v.begin(), v.end());
    size_t i = 0;
    while (i < v.size()) {
        size_t j = i;
        ll total = 0;
        while (j < v.size() && v[j].first == v[i].first) {
            size_t k = j;
            while (k < v.size() && v[k].first == v[i].first && v[k].second == v[j].second) k++;
            ll cnt = (ll)(k - j);
            ans -= cnt * (cnt - 1) / 2;
            total += cnt;
            j = k;
        }
        ans += total * (total - 1) / 2;
        i = j;
    }
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        scanf("%d", &N);
        vector<int> X(N), Y(N), Z(N);
        for (int i = 0; i < N; i++) scanf("%d %d %d", &X[i], &Y[i], &Z[i]);

        ll ans = 0;

        // 第一张网格：整数顶点构成的等边三角形网格
        loc.clear();
        for (int i = 0; i < N; i++) {
            int x = X[i], y = Y[i], z = Z[i];
            Loc t;
            t.a = x - ((z > 1 && z < 8) ? 1 : 0);
            t.b = y - ((z > 5) ? 1 : 0);
            t.c = ((z % 4) > 1) ? 1 : 0;
            loc.push_back(t);
        }
        ans += calc();

        // 第二张网格：指向上方三角形的外接正六边形；距离恰为对应三角形距离的一半
        loc.clear();
        for (int i = 0; i < N; i++) {
            int x = X[i], y = Y[i], z = Z[i];
            Loc t;
            t.a = x - ((z > 2 && z < 7) ? 1 : 0);
            t.b = y - ((z > 6 && z != 11) ? 1 : 0);
            t.c = 0;
            loc.push_back(t);
        }
        ans += calc() / 2;

        // 第三张网格：指向下方三角形的外接正六边形
        loc.clear();
        for (int i = 0; i < N; i++) {
            int x = X[i], y = Y[i], z = Z[i];
            Loc t;
            t.a = x - ((z != 0 && z < 9) ? 1 : 0);
            t.b = y - ((z == 0 || z > 4) ? 1 : 0);
            t.c = 1;
            loc.push_back(t);
        }
        ans += calc() / 2;

        // 三个方向的菱形链修正
        vector<pair<int, int> > c1, c2, c3;
        for (int i = 0; i < N; i++) {
            int x = X[i], y = Y[i], z = Z[i];
            int d = z % 6;
            if (d == 0 || d == 5) { // 水平长对角线的菱形
                c1.push_back(make_pair(y, x - ((z == 5 || z == 6) ? 1 : 0)));
            } else if (d == 1 || d == 2) { // 右上方
                c2.push_back(make_pair(x, y - ((z == 7 || z == 8) ? 1 : 0)));
            } else { // 左上方
                c3.push_back(make_pair(x + y, y - ((z == 9 || z == 10) ? 1 : 0)));
            }
        }
        chains(c1, ans);
        chains(c2, ans);
        chains(c3, ans);

        printf("%lld\n", ans);
    }
    return 0;
}
