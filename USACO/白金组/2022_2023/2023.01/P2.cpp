#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 18;      // 水池数上限
const int MAXQ = 200000;  // 询问数上限
const long long MAXDIST = 1000000001LL;             // 视为"不可达"的距离（大于任何询问时间 s <= 1e9）
const long long FLOORADD = -1800000000000000000LL;  // dp 值下界，保证中间量相加不溢出

int N, M, Q;
long long rate[MAXN];     // 每个水池每秒积累的法力
long long rateSum[1 << 18];  // 每个集合的速率之和
long long dist[MAXN][MAXN];  // 任意两点最短路
static long long dp[MAXN][1 << 18]; // dp[now][rem]：最后停在 now，还要（更早地）访问集合 rem 时的最优"截距"

struct Line {
    long long slope, add; // 法力 = slope * s + add
};

Line lineBuf[1 << 17]; // 以某个水池结尾的全部直线
Line hull[1 << 17];    // 上凸壳
long long hullStart[1 << 17]; // hullStart[k]：第 k 条线开始不劣于第 k-1 条线的最小时间
int hullCount;

long long queryS[MAXQ];
int queryE[MAXQ];
int order[MAXQ];
long long answer[MAXQ];

bool lineLess(const Line &a, const Line &b) {
    if (a.slope != b.slope) return a.slope < b.slope;
    return a.add < b.add;
}

bool orderLess(int a, int b) {
    if (queryE[a] != queryE[b]) return queryE[a] < queryE[b];
    return queryS[a] < queryS[b];
}

// 斜率更大的 (m1, b1) 从哪个整数时间开始不劣于斜率更小的 (m0, b0)（调用时保证 m1 > m0 且 b1 < b0）
long long takeOver(long long m1, long long b1, long long m0, long long b0) {
    long long diffAdd = b0 - b1;
    long long diffSlope = m1 - m0;
    long long res = diffAdd / diffSlope;
    if (diffAdd % diffSlope != 0) ++res; // 向上取整
    return res;
}

// 按斜率递增的顺序插入直线，维护最大值上凸壳
void addLine(long long m, long long b) {
    if (hullCount > 0 && hull[hullCount - 1].slope == m) {
        if (hull[hullCount - 1].add >= b) return; // 同斜率更差，直接丢弃
        --hullCount;                              // 同斜率更好，替换掉旧线
    }
    while (hullCount > 0) {
        // 新线斜率更大且截距不劣，最后一条线被完全支配
        if (b >= hull[hullCount - 1].add) {
            --hullCount;
            continue;
        }
        // 最后一条线还没轮到它最优，就被新线超过了，说明它永远不会最优
        if (hullCount >= 2) {
            long long x = takeOver(m, b, hull[hullCount - 1].slope, hull[hullCount - 1].add);
            if (x <= hullStart[hullCount - 1]) {
                --hullCount;
                continue;
            }
        }
        break;
    }
    long long start = 1; // 时间 s >= 1，第一条线从 1 开始接管
    if (hullCount > 0) start = takeOver(m, b, hull[hullCount - 1].slope, hull[hullCount - 1].add);
    hull[hullCount].slope = m;
    hull[hullCount].add = b;
    hullStart[hullCount] = start;
    ++hullCount;
}

int main() {
    scanf("%d%d", &N, &M);
    for (int i = 0; i < N; ++i) scanf("%lld", &rate[i]);

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) dist[i][j] = MAXDIST;
    for (int i = 0; i < M; ++i) {
        int a, b;
        long long t;
        scanf("%d%d%lld", &a, &b, &t);
        --a;
        --b;
        if (t < dist[a][b]) dist[a][b] = t;
    }
    for (int k = 0; k < N; ++k)
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                if (dist[i][k] + dist[k][j] < dist[i][j]) dist[i][j] = dist[i][k] + dist[k][j];

    int full = 1 << N;
    rateSum[0] = 0;
    for (int mask = 1; mask < full; ++mask) {
        for (int i = 0; i < N; ++i) {
            if ((mask >> i) & 1) {
                rateSum[mask] = rateSum[mask ^ (1 << i)] + rate[i];
                break;
            }
        }
    }

    // 一段旅程 p_1..p_k 在时间 s 结束时最多收取 sum(m_{p_i} * s) - sum(m_{p_i} * 它后面那几段路程长度)，
    // 也就是一条关于 s 的直线。dp 倒着构造旅程：先决定最后停在哪，再往前挑下一个水池。
    // 每往前接一个水池 i，rem 中所有水池的"后续路程"都多出 dist[i][now] 这一段，
    // 因此代价要减去 rateSum[rem] * dist[i][now]。
    for (int rem = 1; rem < full; ++rem) {
        long long rs = rateSum[rem];
        for (int now = 0; now < N; ++now) {
            if ((rem >> now) & 1) continue;
            long long best = FLOORADD;
            for (int i = 0; i < N; ++i) {
                if (!((rem >> i) & 1)) continue;
                long long cand = dp[i][rem ^ (1 << i)] - rs * dist[i][now];
                if (cand > best) best = cand;
            }
            dp[now][rem] = best;
        }
    }

    scanf("%d", &Q);
    for (int i = 0; i < Q; ++i) {
        scanf("%lld%d", &queryS[i], &queryE[i]);
        --queryE[i];
        order[i] = i;
    }
    // 按结尾水池分组、组内按时间递增，就能用单调指针在凸壳上滑动
    sort(order, order + Q, orderLess);

    int start = 0;
    while (start < Q) {
        int e = queryE[order[start]];
        int end = start;
        while (end < Q && queryE[order[end]] == e) ++end;

        int count = 0;
        for (int mask = 0; mask < full; ++mask) {
            if ((mask >> e) & 1) continue; // 结尾水池必须在集合里，且作为最后一个
            lineBuf[count].slope = rateSum[mask] + rate[e];
            lineBuf[count].add = dp[e][mask];
            ++count;
        }
        sort(lineBuf, lineBuf + count, lineLess);
        hullCount = 0;
        for (int i = 0; i < count; ++i) addLine(lineBuf[i].slope, lineBuf[i].add);

        int ptr = 0;
        for (int i = start; i < end; ++i) {
            int qi = order[i];
            long long x = queryS[qi];
            while (ptr + 1 < hullCount && hullStart[ptr + 1] <= x) ++ptr;
            answer[qi] = hull[ptr].slope * x + hull[ptr].add;
        }
        start = end;
    }

    for (int i = 0; i < Q; ++i) printf("%lld\n", answer[i]);
    return 0;
}
