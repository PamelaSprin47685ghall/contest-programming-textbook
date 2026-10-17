#include <algorithm>
#include <cstdio>
#include <vector>

struct Plant {
    long long h, a, t;
};

void Solve() {
    int n;
    scanf("%d", &n);
    std::vector<Plant> p(n);
    for (int i = 0; i < n; ++i) scanf("%lld", &p[i].h);
    for (int i = 0; i < n; ++i) scanf("%lld", &p[i].a);
    for (int i = 0; i < n; ++i) scanf("%lld", &p[i].t);

    // 目标是经过 day 天后, 第 i 株植物的高度排名恰好为 p[i].t
    // 将植物按照目标名次 t 排序: 最终要求 height[0] > height[1] > ... > height[n-1]
    std::sort(p.begin(), p.end(), [](const Plant& x, const Plant& y) {
        return x.t < y.t;
    });

    // 维护合法天数区间 [minDay, maxDay]
    long long minDay = 0;
    long long maxDay = 2000000000000000000LL;

    for (int i = 0; i < n - 1; ++i) {
        long long hDiff = p[i].h - p[i + 1].h;
        long long aDiff = p[i].a - p[i + 1].a;

        // 要求: hDiff + day * aDiff > 0
        if (aDiff == 0) {
            if (hDiff <= 0) {
                printf("-1\n");
                return;
            }
        } else if (aDiff > 0) {
            // day * aDiff > -hDiff => day >= floor((-hDiff) / aDiff) + 1
            if (hDiff <= 0) {
                long long req = (-hDiff) / aDiff + 1;
                minDay = std::max(minDay, req);
            }
        } else {
            // aDiff < 0: day * (-aDiff) < hDiff
            if (hDiff <= 0) {
                printf("-1\n");
                return;
            }
            long long lim = (hDiff - 1) / (-aDiff);
            maxDay = std::min(maxDay, lim);
        }
    }

    if (minDay <= maxDay) {
        printf("%lld\n", minDay);
    } else {
        printf("-1\n");
    }
}

int main() {
    int t;
    if (scanf("%d", &t) == 1) {
        while (t--) {
            Solve();
        }
    }
    return 0;
}
