#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 100005;  // 救生员数量上限
const int MAXK = 105;     // 必须解雇的人数上限（K ≤ 100）
const int NEG = -1;       // 不可达标记：任何合法方案的覆盖长度都 ≥ 0

int N, K;
int rawStart[MAXN], rawEnd[MAXN];  // 输入的原始区间
int order[MAXN];                   // 按左端点排序用的下标
int s[MAXN], e[MAXN];              // 剔除被包含的区间后，剩下的区间（左右端点都单调递增）
int M;                             // 剩下的区间个数
int f[MAXN][MAXK];                 // f[i][r]：只看前 i 个区间、保留第 i 个、且至少解雇 r 个时的最大覆盖长度
int pRow[MAXK];                    // 滚动维护 P[x][s]：前 x 个区间至少解雇 s 个（不要求保留 x）时的最大覆盖长度

bool cmpByStart(int a, int b) {
    return rawStart[a] < rawStart[b];
}

int main() {
    scanf("%d %d", &N, &K);
    for (int i = 0; i < N; ++i) {
        scanf("%d %d", &rawStart[i], &rawEnd[i]);
        order[i] = i;
    }
    sort(order, order + N, cmpByStart);

    // 若区间 A 被区间 B 完全包含，解雇 A 绝不会减少覆盖；而“多解雇的人可以再放回来”，
    // 所以“至少解雇 K 个”和“正好解雇 K 个”的最优值相同。因此最优解里可以认为被包含的
    // 区间全都被解雇，它们只是在帮我们凑解雇名额。
    int maxEnd = -1;    // 已经保留的区间里最大的右端点
    int removed = 0;    // 因被包含而直接解雇的区间数
    for (int t = 0; t < N; ++t) {
        int id = order[t];
        if (rawEnd[id] < maxEnd) {  // 端点互不相同，所以右端点变小就一定被包含
            ++removed;
            continue;
        }
        ++M;
        s[M] = rawStart[id];
        e[M] = rawEnd[id];
        maxEnd = rawEnd[id];
    }

    int need = K - removed;  // 还需要在剩下的区间里解雇的人数
    if (need < 0) need = 0;  // 已经解雇够了，剩下的区间一个都不用再解雇

    pRow[0] = 0;                                 // P[0][0]：没有区间、解雇 0 个，覆盖为 0
    for (int r = 1; r <= K; ++r) pRow[r] = NEG;  // P[0][r]：0 个区间里解雇不出 r > 0 个

    int j0 = 1;    // 下标 < j0 的区间都与第 i 个区间不相交；j0 就是最左边与 i 相交的那个
    int pIdx = 0;  // pRow 当前保存的是 P[pIdx][*]
    for (int i = 1; i <= M; ++i) {
        // e 单调递增，所以 j0 只会往右推：i 向右移动时相交的门槛也在右移
        while (j0 < i && e[j0] < s[i]) ++j0;
        bool hasCross = (j0 < i);
        // 把 pRow 推进到 P[j0-1][*]，递推式 P[x][s] = max(f[x][s], P[x-1][s-1])
        while (pIdx < j0 - 1) {
            ++pIdx;
            for (int r = K; r >= 1; --r) {
                int v = f[pIdx][r];
                if (pRow[r - 1] > v) v = pRow[r - 1];
                pRow[r] = v;
            }
            pRow[0] = f[pIdx][0];  // 一个都不解雇时，最优就是把前 pIdx 个全留下
        }
        for (int r = 0; r <= K; ++r) {
            int best = NEG;
            if (r <= i - 1) best = e[i] - s[i];  // 情况一：前 i 个里只留第 i 个
            if (hasCross) {
                // 情况二：保留最左边那个与 i 相交的区间 j0；j0 之后到 i 之间全解雇，
                // 再从前 j0 个里挑（f 的定义已经保证 j0 被保留），覆盖只多出 e[i] - e[j0]
                int lo = r - (i - j0 - 1);
                if (lo < 0) lo = 0;
                if (lo <= j0 - 1) {
                    int v = f[j0][lo];
                    if (v >= 0 && v + (e[i] - e[j0]) > best) best = v + (e[i] - e[j0]);
                }
            }
            // 情况三：把所有与 i 相交的区间都解雇，保留的区间全落在前 j0-1 个里；
            // 这些区间与 i 不相交（e < s[i]），所以长度可以直接相加
            int lo = r - (i - j0);
            if (lo < 0) lo = 0;
            if (lo <= j0 - 1 && pRow[lo] >= 0 && pRow[lo] + (e[i] - s[i]) > best)
                best = pRow[lo] + (e[i] - s[i]);
            f[i][r] = best;
        }
    }

    int ans = 0;  // 一个区间都不留（覆盖 0）永远是合法方案，所以答案至少是 0
    for (int i = 1; i <= M; ++i) {
        int r = need - (M - i);  // 第 i 个之后的区间全部解雇，前面还需要解雇这么多
        if (r < 0) r = 0;
        if (f[i][r] > ans) ans = f[i][r];
    }
    printf("%d\n", ans);
    return 0;
}
