#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;

// 做法（官方 O(NK) 结构，K=10）：
// 把每个花坛拆成若干「单位」：A_i > B_i 时产生 (A_i-B_i) 个多余的土单位(+1)，
// B_i > A_i 时产生 (B_i-A_i) 个缺土单位(-1)，它们的位置都是 i。
// 把 +1 看成向上迈一步、-1 看成向下迈一步，则沿途每条水平「层」上单位的
// 方向必然交替出现（同向才会换层），而不同层之间互不影响，可以各自独立求解。
// 层内配对分两种：相邻配对属于「短边」，代价 Z*距离；跨过一段的配对可以看成
// 「长边」，代价 X+Y（等价于删掉一单位再买一单位，官方称之为 teleport）。
// 长度奇数的层恰有一个单位不参与配对，代价是该方向的 X 或 Y。
// 由于最优解可以做到不交叉、且长边之下只有紧挨着配对的短边，层内用一次单调
// 扫描的 DP 即可在线性时间内求出：result[j] 表示把前 j+1 个单位全部配对的最小
// 代价（j 为奇数），转移要么让 j 与 j-1 相邻配对，要么让 j 用长边连到更早的 i，
// 中间各单位依次相邻配对。位置递增使得「值得用长边」的候选 i 单调右移。

const int MAXU = 1000000;    // 单位总数上限：10 * 100000 <= 10^6
const int OFFSET = 1000000;  // 层号基准，保证层号非负
const int MAXLV = 2 * OFFSET + 1;

static int X, Y, Z;
static int unitPos[MAXU];                       // 单位所在花坛下标
static int unitDir[MAXU];                       // +1 表示多余土，-1 表示缺土
static int unitLvl[MAXU];                       // 单位所属的层号
static int cnt[MAXLV + 2], startAt[MAXLV + 2];  // 计数排序，把单位按层号分组
static int order[MAXU];                         // 分组后的单位下标（组内位置递增）
static int levelPos[MAXU], revPos[MAXU];        // 一层的岗位序列及其反转
static long long dpFwd[MAXU + 1], dpBwd[MAXU + 1];

// 层内位置递增、方向交替，v[j] 只能与 v[j-1]（短边）或更早的某个 v[i]（长边）配对
void dp(const int *v, int len, long long *result)
{
    long long prefCostJ = 0;  // 相邻配对 (0,1),(2,3),...,(j-2,j-1) 的代价和
    long long prefCostI = 0;  // 同样的代价和，但只截到候选 i 为止
    long long bestLong = (long long)4e18;  // 目前最优的「用一条长边」方案代价
    int i = -2;                            // 长边左端点候选，只能单调右移
    for (int j = 1; j < len; j += 2) {
        if (j > 1) {
            // j 增大时，长边方案内部多出一对相邻配对，已积累的候选代价同步增加
            long long add = (long long)Z * abs(v[j - 2] - v[j - 1]);
            prefCostJ += add;
            bestLong += add;
        }
        // 只有当长边不再比「走这段路」更贵时，该候选才可能被用到；位置递增，
        // 所以这个窗口只右移不收缩，整体扫描是线性的
        while (i + 2 < j && (long long)X + Y <= (long long)Z * abs(v[j] - v[i + 2])) {
            i += 2;
            if (i > 0) prefCostI += (long long)Z * abs(v[i] - v[i - 1]);
            long long cand = (long long)X + Y + prefCostJ - prefCostI
                             + (i > 0 ? result[i - 1] : 0);
            if (cand < bestLong) bestLong = cand;
        }
        long long shortEdge = (long long)Z * abs(v[j] - v[j - 1]) + (j > 1 ? result[j - 2] : 0);
        result[j] = bestLong < shortEdge ? bestLong : shortEdge;
    }
}

// 一层的最小代价；ecost 是该层唯一未配对单位的代价（多土为 Y，缺土为 X）
long long solve(const int *v, int len, int ecost)
{
    if (len == 0) return 0;
    if (len == 1) return ecost;
    dp(v, len, dpFwd);  // dpFwd[j]：把 v[0..j] 全部配对（j 为奇数）
    for (int k = 0; k < len; k++) revPos[k] = v[len - 1 - k];
    dp(revPos, len, dpBwd);
    // 反转回来后 dpBwd[i] 表示把后缀 v[i..len-1] 全部配对的最小代价
    for (int l = 0, r = len - 1; l < r; l++, r--) {
        long long t = dpBwd[l];
        dpBwd[l] = dpBwd[r];
        dpBwd[r] = t;
    }
    if (len % 2 == 0) return dpFwd[len - 1];  // 偶数长度必须全部配对
    // 奇数长度：枚举被留下的那个单位，前缀与后缀各自独立配对
    long long best = (long long)ecost + (dpFwd[len - 2] < dpBwd[1] ? dpFwd[len - 2] : dpBwd[1]);
    for (int i = 2; i <= len - 3; i += 2) {
        long long cand = dpFwd[i - 1] + ecost + dpBwd[i + 1];
        if (cand < best) best = cand;
    }
    return best;
}

int main()
{
    int N;
    if (scanf("%d %d %d %d", &N, &X, &Y, &Z) != 4) return 0;
    int M = 0;                 // 单位个数
    int lastDir = 0, level = OFFSET;
    for (int i = 0; i < N; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        if (a == b) continue;  // 差值为 0：既不产生单位，也不改变迈步方向
        int dir = a > b ? 1 : -1;
        int num = a > b ? a - b : b - a;
        for (int t = 0; t < num; t++) {
            // 与上一个单位同向时继续换层，反向时留在本层，于是层内方向交替
            if (lastDir == dir) level += dir;
            unitPos[M] = i;
            unitDir[M] = dir;
            unitLvl[M] = level;
            M++;
            lastDir = dir;
        }
    }
    // 计数排序：单位按层号分组，组内保持生成顺序（位置不减）
    for (int k = 0; k < M; k++) cnt[unitLvl[k]]++;
    int run = 0;
    for (int l = 0; l <= MAXLV; l++) {
        int c = cnt[l];
        startAt[l] = run;
        cnt[l] = run;  // 就地改成写指针，节省一个数组
        run += c;
    }
    for (int k = 0; k < M; k++) order[cnt[unitLvl[k]]++] = k;

    long long total = 0;
    for (int l = 0; l < MAXLV; l++) {
        int s = startAt[l], e = startAt[l + 1];
        if (s == e) continue;
        int len = e - s;
        for (int k = s; k < e; k++) levelPos[k - s] = unitPos[order[k]];
        // 该层第一个单位的方向决定「剩下的那一个」是多余土（删掉，代价 Y）还是缺土（买进，代价 X）
        int ecost = unitDir[order[s]] > 0 ? Y : X;
        total += solve(levelPos, len, ecost);
    }
    printf("%lld\n", total);
    return 0;
}
