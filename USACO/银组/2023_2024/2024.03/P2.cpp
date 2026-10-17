#include <cstdio>
#include <algorithm>
#include <cstdlib>

using namespace std;

const int MAXP = 200005;

int n, postCount;                // 奶牛数、柱子数
int px[MAXP], py[MAXP];          // 柱子坐标
int vs[MAXP], hs[MAXP];          // 分别按 (x,y) 与 (y,x) 升序排好的柱子下标
int adjV[MAXP], adjH[MAXP];      // 每根柱子被哪条竖直 / 水平栅栏段连接的另一端
int ord[MAXP];                   // 沿栅栏绕行一圈依次经过的柱子下标
int indexInOrd[MAXP];            // 柱子在绕行顺序里的位置
long long distOrd[MAXP + 1];     // 绕行顺序里第 i 根柱子到起点沿栅栏的距离
int cum[MAXP + 1];               // 差分数组：区间加 1 最后前缀和

// 同一个 x 上的柱子按 y 排序后必然相邻成对，同一条水平线同理
bool cmpByXY(int a, int b) {
    if (px[a] != px[b]) return px[a] < px[b];
    return py[a] < py[b];
}

bool cmpByYX(int a, int b) {
    if (py[a] != py[b]) return py[a] < py[b];
    return px[a] < px[b];
}

// 在 sortArr 的下标区间 [lo,hi) 内，按 keyArr 找第一个 key >= target 的位置
int lowerBoundKey(const int *sortArr, int lo, int hi, const int *keyArr, int target) {
    while (lo < hi) {
        int mid = (lo + hi) >> 1;
        if (keyArr[sortArr[mid]] < target) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

// 在 sortArr 的下标区间 [lo,hi) 内，按 keyArr 找第一个 key > target 的位置
int upperBoundKey(const int *sortArr, int lo, int hi, const int *keyArr, int target) {
    while (lo < hi) {
        int mid = (lo + hi) >> 1;
        if (keyArr[sortArr[mid]] <= target) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

// 求点 (x,y) 沿栅栏「上次经过的柱子」：outIdx 为柱子编号，outAlong 为再走多远；
// 洞若观火：若点正好落在柱子上则 outAlong = 0
void getPos(int x, int y, int &outIdx, int &outAlong) {
    // 先看竖直方向：x 这条竖线上若有点，它一定落在某条竖栅栏段的内部或端点上
    int gLo = lowerBoundKey(vs, 0, postCount, px, x);
    if (gLo < postCount && px[vs[gLo]] == x) {
        int gHi = upperBoundKey(vs, gLo, postCount, px, x);
        int i = lowerBoundKey(vs, gLo, gHi, py, y);
        if (i < gHi && py[vs[i]] == y) {
            outIdx = indexInOrd[vs[i]];
            outAlong = 0;
            return;
        }
        if ((i - gLo) % 2 == 1) {       // 落在第 (i-gLo) 段竖栅栏段的内部
            int a = vs[i - 1], b = vs[i];
            // 两端点必然在绕行顺序里相邻，把靠前的那端记作 a
            if ((indexInOrd[a] + 1) % postCount != indexInOrd[b]) {
                int tmp = a;
                a = b;
                b = tmp;
            }
            outIdx = indexInOrd[a];
            outAlong = abs(py[a] - y);
            return;
        }
    }
    // 否则点只能落在水平栅栏段上
    int hLo = lowerBoundKey(hs, 0, postCount, py, y);
    int hHi = upperBoundKey(hs, hLo, postCount, py, y);
    int j = lowerBoundKey(hs, hLo, hHi, px, x);
    if (j < hHi && px[hs[j]] == x) {
        outIdx = indexInOrd[hs[j]];
        outAlong = 0;
        return;
    }
    int a = hs[j - 1], b = hs[j];
    if ((indexInOrd[a] + 1) % postCount != indexInOrd[b]) {
        int tmp = a;
        a = b;
        b = tmp;
    }
    outIdx = indexInOrd[a];
    outAlong = abs(px[a] - x);
}

// 给绕行顺序里的下标区间 [l,r] 整体加 1；l > r 时差分数组自动抵消
void addRange(int l, int r) {
    ++cum[l];
    --cum[r + 1];
}

int main() {
    scanf("%d %d", &n, &postCount);
    for (int i = 0; i < postCount; ++i) {
        scanf("%d %d", &px[i], &py[i]);
        vs[i] = hs[i] = i;
    }
    sort(vs, vs + postCount, cmpByXY);
    sort(hs, hs + postCount, cmpByYX);

    // 找出所有栅栏段：每条竖线（横线）上按坐标排序后的相邻成对柱子相连
    for (int lo = 0; lo < postCount;) {
        int hi = upperBoundKey(vs, lo, postCount, px, px[vs[lo]]);
        for (int i = lo; i + 1 < hi; i += 2) {
            adjV[vs[i]] = vs[i + 1];
            adjV[vs[i + 1]] = vs[i];
        }
        lo = hi;
    }
    for (int lo = 0; lo < postCount;) {
        int hi = upperBoundKey(hs, lo, postCount, py, py[hs[lo]]);
        for (int i = lo; i + 1 < hi; i += 2) {
            adjH[hs[i]] = hs[i + 1];
            adjH[hs[i + 1]] = hs[i];
        }
        lo = hi;
    }

    // 从 0 号柱子出发绕栅栏走一圈，得到柱子沿栅栏的顺序
    int ordLen = 0;
    ord[ordLen++] = 0;
    while (true) {
        int cur = ord[ordLen - 1];
        int prev = (ordLen >= 2) ? ord[ordLen - 2] : -1;
        int nxt = (adjV[cur] != prev) ? adjV[cur] : adjH[cur];
        ord[ordLen++] = nxt;
        if (nxt == 0) break;
    }
    --ordLen;                            // 去掉重复记录的起点
    for (int i = 0; i < ordLen; ++i) indexInOrd[ord[i]] = i;
    for (int i = 1; i <= postCount; ++i) {
        int a = ord[i - 1], b = ord[i % postCount];
        distOrd[i] = distOrd[i - 1] + abs(px[a] - px[b]) + abs(py[a] - py[b]);
    }

    for (int q = 0; q < n; ++q) {
        int x1, y1, x2, y2;
        scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
        int i1, a1, i2, a2;
        getPos(x1, y1, i1, a1);
        getPos(x2, y2, i2, a2);
        long long d1 = distOrd[i1] + a1, d2 = distOrd[i2] + a2;
        if (d1 > d2) {                   // 只按绕行顺序讨论，先交换成前者在先
            int tmp = i1; i1 = i2; i2 = tmp;
            tmp = a1; a1 = a2; a2 = tmp;
            long long td = d1; d1 = d2; d2 = td;
        }
        // 两端点离得近就走正方向，否则绕远的那一侧才是牛真正走的路
        if (2 * (d2 - d1) > distOrd[postCount]) {
            addRange(i2 + (a2 > 0 ? 1 : 0), postCount - 1);
            addRange(0, i1);
        } else {
            addRange(i1 + (a1 > 0 ? 1 : 0), i2);
        }
    }

    for (int i = 1; i < postCount; ++i) cum[i] += cum[i - 1];
    for (int i = 0; i < postCount; ++i) printf("%d\n", cum[indexInOrd[i]]);
    return 0;
}
