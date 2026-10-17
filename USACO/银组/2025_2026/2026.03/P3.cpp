// P3 Point Elimination
// 交换 y 坐标可以先全部做完，等价于把 y 的多重集任意重排到给定的各个 x 上。
// 要能全部消掉，点必须两两配对，每对要么横着（y 相同、x 差 1），要么竖着（x 相同、y 差 1）。
// 单看 x 的多重集：竖着的对是"相等对"，横着的对是"差 1 对"；单看 y 的多重集则正好反过来。
// 设 x 侧能配出 k 个差 1 对（就是横着消的对数），则 y 侧必须恰好配出 N/2-k 个差 1 对。
// 两个多重集的"差 1 对数"都取到区间 [最少, 最多] 内所有同奇偶的值，于是判两个区间能否对接。
// 另外排序后相邻两两配对 (v[0],v[1]), (v[2],v[3])... 若某对差 >= 2，这一侧两侧元素个数
// 都是奇数，根本没法和"相等/差 1"的对凑完，直接 NO。
#include <cstdio>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

// 把有序数组 v 配成"相等对"或"差 1 对"，返回 (最少, 最多) 的差 1 对数。
pair<int, int> diffRange(const vector<int> &v) {
    int n = v.size();

    // 最少：排序后相邻两两配对，每次尽量用相等对消化元素。
    int lo = 0;
    for (int i = 0; i < n; i += 2) {
        if (v[i] != v[i + 1]) ++lo;
    }

    // 最多：从小到大，值 x 尽量和 x+1 配成差 1 对；剩下的 x 只能彼此相等配对，
    // 所以配对数要和剩余个数同奇偶，多配一张会留下奇数个 x 配不完。
    int hi = 0;
    map<int, int> cnt;
    for (int i = 0; i < n; ++i) ++cnt[v[i]];
    while (!cnt.empty()) {
        map<int, int>::iterator it = cnt.begin();
        int val = it->first;
        int c = it->second;
        map<int, int>::iterator nxt = cnt.find(val + 1);
        if (nxt != cnt.end() && nxt->second > 0) {
            int d = min(c, nxt->second);
            if ((c - d) % 2 != 0) --d; // 剩余 c-d 个 val 必须能两两配对
            nxt->second -= d;
            hi += d;
        }
        cnt.erase(it);
    }

    return make_pair(lo, hi);
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        int N;
        scanf("%d", &N);
        vector<int> x(N), y(N);
        for (int i = 0; i < N; ++i) {
            scanf("%d %d", &x[i], &y[i]);
        }
        sort(x.begin(), x.end());
        sort(y.begin(), y.end());

        bool ok = true;
        for (int i = 0; i < N; i += 2) {
            if (x[i + 1] - x[i] >= 2 || y[i + 1] - y[i] >= 2) {
                ok = false;
                break;
            }
        }
        if (!ok) {
            printf("NO\n");
            continue;
        }

        pair<int, int> xr = diffRange(x);
        pair<int, int> yr = diffRange(y);
        int half = N / 2;
        bool yes = false;
        // k 是 x 侧的差 1 对数（横向消除数），y 侧必须配出 half-k 个差 1 对。
        for (int k = xr.first; k <= xr.second; k += 2) {
            int need = half - k;
            if (need >= yr.first && need <= yr.second && (need - yr.first) % 2 == 0) {
                yes = true;
                break;
            }
        }
        printf(yes ? "YES\n" : "NO\n");
    }
    return 0;
}
