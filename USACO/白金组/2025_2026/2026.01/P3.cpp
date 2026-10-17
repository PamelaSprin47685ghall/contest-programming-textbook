#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long ll;

// 记忆中的格子：v = 1 表示 '+'，v = 0 表示 '-'
struct Cell {
    int v, r, c;
};

static int R, C, X;
static vector<Cell> cells;
static ll ans;

// 关键观察：合法网格中，同一条反对角线（r + c 相同）上的取值必然完全相同，
// 于是整个网格只由"对角线的值序列"决定；把编号 r + c 称为第 r + c 条对角线。
// 一条 "+ + 链" 指的是某条对角线上所有格子都是 +"的连续两层对角线，
// 把链的位置记作链中右侧那个 + 所在的对角线编号 d（即对角线 d-1 与 d 全是 +）。
// 链会让后续对角线的奇偶交替模式翻转一次，所以整张网格由首对角线的值 + 若干条链唯一确定。

// 上一条链位于 d 时，下一条链的最左可能位置：
// 两条链之间至少要隔开 max(R, C) - 1 条对角线，否则会存在一个长度 max(R, C)
// 的窗口（即某一行或某一列）同时含有两个 "++" 子段，其和会超过 2；
// 又因为中间部分必须严格交替，链位置的奇偶性固定，故距离为奇数，步长 2。
static int minNextChain(int d) {
    int mx = max(R, C);
    if (mx % 2 == 0) return d + mx - 1;
    return d + mx;
}

// 统计 [mi, rb] 内与 mi 同奇偶的位置个数（即 mi, mi+2, ... 的个数）
static ll countPositions(int mi, int rb) {
    if (mi > rb) return 0;
    return (rb - mi) / 2 + 1;
}

// original = true 统计"链朝左下"的网格；original = false 统计镜像（链朝右下）的网格，
// 此时不重复统计"完全没有链"的那张网格（它在两个方向上是同一张）。
static void countOrientation(bool original) {
    int n = R + C - 1;
    vector<int> vals(n, -1);
    for (size_t i = 0; i < cells.size(); i++) {
        int d = cells[i].r + cells[i].c;
        if (vals[d] == -1) vals[d] = cells[i].v;
        else if (vals[d] != cells[i].v) return;   // 同一条对角线上记忆值冲突，无法构造
    }

    // 固定首尾两条对角线的值：这样每条链都落在某两条"已知对角线"之间，
    // 按区间统计链的条数就不会漏掉边界上的链
    bool firstFree = (vals[0] == -1);
    bool lastFree = (vals[n - 1] == -1);
    for (int firstVal = 0; firstVal <= 1; firstVal++) {
        if (!firstFree && vals[0] != firstVal) continue;
        vals[0] = firstVal;
        for (int lastVal = 0; lastVal <= 1; lastVal++) {
            if (!lastFree && vals[n - 1] != lastVal) continue;
            vals[n - 1] = lastVal;

            vector<int> known;
            for (int i = 0; i < n; i++)
                if (vals[i] != -1) known.push_back(i);

            // 相邻两条已知对角线之间：值异或与距离奇偶性不同 <=> 区间内有奇数条链（即恰好 1 条），
            // 相同 <=> 区间内有偶数条链（即 0 条或 2 条）。
            // 全局最多 2 条链，所以"不同"的区间最多出现两个，出现三个以上就无解
            pair<int, int> odd1(-1, -1), odd2(-1, -1);
            bool ok = true;
            for (int i = 0; i + 1 < (int)known.size(); i++) {
                int a = known[i], b = known[i + 1];
                if ((vals[a] ^ vals[b]) != (b - a) % 2) {
                    if (odd1.first == -1) odd1 = make_pair(a, b);
                    else if (odd2.first == -1) odd2 = make_pair(a, b);
                    else { ok = false; break; }
                }
            }
            if (!ok) continue;

            if (odd1.first == -1) {
                // 没有奇数区间：要么 0 条链，要么 2 条链落在同一个区间里
                if (original) ans++;
                for (int i = 0; i + 1 < (int)known.size(); i++) {
                    int L = known[i], Rt = known[i + 1];
                    // 链位于 d 要求第 d-1、d 条对角线的值都是 '+'，
                    // 由 L 的值和交替规律即可确定 d 的奇偶性与最小取值
                    int startL = (vals[L] == 1) ? L + 1 : L + 2;
                    for (int lb = startL; minNextChain(lb) <= Rt; lb += 2)
                        ans += countPositions(minNextChain(lb), Rt);
                }
            } else if (odd2.first == -1) {
                // 恰好一个奇数区间：该区间内恰好有一条链
                int L = odd1.first, Rt = odd1.second;
                int startL = (vals[L] == 1) ? L + 1 : L + 2;
                ans += countPositions(startL, Rt);
            } else {
                // 两个奇数区间：各含一条链，第二条链还须满足与第一条链的距离约束。
                // 两个下界同奇偶（可由两个区间端点值的异或关系推出），故可直接取较大者
                int l1 = odd1.first, r1 = odd1.second;
                int l2 = odd2.first, r2 = odd2.second;
                int start1 = (vals[l1] == 1) ? l1 + 1 : l1 + 2;
                int start2 = (vals[l2] == 1) ? l2 + 1 : l2 + 2;
                for (int lb = start1; lb <= r1; lb += 2) {
                    int rb = max(minNextChain(lb), start2);
                    ans += countPositions(rb, r2);
                }
            }
        }
    }
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        scanf("%d %d %d", &R, &C, &X);
        cells.clear();
        for (int i = 0; i < X; i++) {
            char ch;
            int r, c;
            scanf(" %c %d %d", &ch, &r, &c);
            Cell cell;
            cell.v = (ch == '+') ? 1 : 0;
            cell.r = r - 1;
            cell.c = c - 1;
            cells.push_back(cell);
        }

        if (R == 1 && C == 1) {
            printf("%d\n", X == 1 ? 1 : 2);
            continue;
        }

        ans = 0;
        countOrientation(true);
        if (min(R, C) > 1) {
            // 把所有列左右镜像：链朝右下的网格与镜像后链朝左下的网格一一对应
            for (size_t i = 0; i < cells.size(); i++) cells[i].c = C - 1 - cells[i].c;
            countOrientation(false);
        }

        if (R == 2 && C == 2) {
            // 2x2 全 + 的网格在两个方向上都表现为一条链，被重复统计了一次，需要减掉
            bool allPlus = true;
            for (size_t i = 0; i < cells.size(); i++)
                if (cells[i].v == 0) { allPlus = false; break; }
            if (allPlus) ans--;
        }

        printf("%lld\n", ans);
    }
    return 0;
}
