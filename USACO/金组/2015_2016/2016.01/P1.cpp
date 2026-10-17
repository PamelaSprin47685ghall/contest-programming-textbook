#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 50005;    // 干草堆数量上限
const int INF = 2000000000;

int n;
// 所有坐标都乘 2: 这样爆发半径也只用整数表示, 答案必然是 0.5 的倍数, 输出不会因浮点误差出错
int a[MAXN];
int leftMin[MAXN];    // leftMin[i]: 只有第 i 堆先爆炸时, 引爆它左边所有堆所需的最小功率(乘 2)
int rightMin[MAXN];   // rightMin[i]: 同理, 引爆它右边所有堆所需的最小功率(乘 2)
int pre[MAXN];        // 判定函数里用: pre[i] 是 i 及之前最后一个满足 leftMin 要求的下标
int mirror[MAXN];     // 求 rightMin 时用的镜像位置数组
int mirrorRes[MAXN];

// 从左到右两指针扫描, 维护 src[i] 这一堆"往左连锁引爆全部"的最小功率。
// 若 i 以功率 p 爆炸, 被波及的最左堆是满足 src[j] >= src[i]-p 的最小下标 j,
// 那么还需要 j 以功率 p-2 继续往左引爆, 所以 p >= max(src[i]-src[j], res[j]+2)。
// 距离项随 j 增大而减少、答案项随 j 增大而增大, 交点附近的 j 最优, 指针只会右移。
void sweepLeft(const int *src, int *res) {
    for (int i = 0; i < n; i++) res[i] = INF;
    res[0] = -2;   // 左边没有堆, 不需要功率, 用 -2 当哨兵
    int j = 0;
    for (int i = 1; i < n; i++) {
        while (j + 1 < i && src[i] - src[j + 1] > res[j + 1] + 2) j++;
        // j 及其左边全部由距离项决定, 取最靠右的 j 最优
        int cand = src[i] - src[j];
        if (res[j + 1] + 2 < cand) cand = res[j + 1] + 2;
        res[i] = cand;
    }
}

// 判定半径为 D(乘 2 后的功率)的一次发射能否引爆全部干草堆。
// 设落点爆炸区间盖住的干草堆是连续一段 [p, q], 则只需:
//   第 p 堆以功率 D-2 向左引爆全部 (leftMin[p] <= D-2),
//   第 q 堆以功率 D-2 向右引爆全部 (rightMin[q] <= D-2),
//   区间要同时盖住 p 和 q, 即 a[q]-a[p] <= 2*D(区间宽度是 2*D)。
// 对固定的 q 取满足条件的最大 p, 距离最小, 所以扫一遍即可。
bool ok(int D) {
    int best = -1;
    for (int i = 0; i < n; i++) {
        if (leftMin[i] <= D - 2) best = i;
        pre[i] = best;
    }
    for (int q = 0; q < n; q++) {
        if (rightMin[q] <= D - 2 && pre[q] >= 0 &&
            (long long)a[q] - a[pre[q]] <= 2LL * D) {
            return true;
        }
    }
    return false;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
        a[i] *= 2;
    }
    sort(a, a + n);

    sweepLeft(a, leftMin);

    // 把位置数组按 a[0]+a[n-1]-a[n-1-i] 左右镜像(镜像后仍递增), 跑同一个扫描,
    // 再翻回来就是"向右引爆全部"的答案: 镜像不改变任何两点间的距离
    for (int i = 0; i < n; i++) mirror[i] = a[0] + a[n - 1] - a[n - 1 - i];
    sweepLeft(mirror, mirrorRes);
    for (int i = 0; i < n; i++) rightMin[i] = mirrorRes[n - 1 - i];

    // 功率越大越容易成功, 所以二分最小可行功率; 上界取"一炮盖住最左最右两堆"
    int lo = 0;
    int hi = a[n - 1] - a[0];
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (ok(mid)) hi = mid;
        else lo = mid + 1;
    }

    printf("%d.%d\n", lo / 2, (lo % 2) ? 5 : 0);
    return 0;
}
