#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// 思路概述：
// 若某一次摆放要盖住原图中「行优先序最靠后」的那个未覆盖非空格 (r,c)，那么这一块自己的
// 行优先序最靠后的非空格也必须正好落在这个格子上：因为平移保持行优先序，该块最后一个格子
// 的位置不会比 (r,c) 更靠前；而它若不等于 (r,c)，就必然落在已经被前面的块盖住的位置上，
// 于是产生重叠，方案不合法。所以第一块的摆放位置完全由「原图最后一个非空格」决定，第二块
// 的摆放位置完全由「盖住第一块后剩下的最后一个未覆盖格」决定，第三块只需检查剩余部分是否
// 与某块碎片完全一致。这样就只需枚举 8K 个朝向作为第一块、8K 个朝向作为第二块。
//
// 每块碎片用两个模数下的多项式哈希表示（横纵坐标线性化为 r*STRIDE+c），于是原图哈希减去
// 已放置的两块的哈希就得到剩余区域的哈希。把每块碎片的哈希规范化到「最后一个非空格固定在
// 下标 KC」这一位置后放进哈希表，查询时对剩余区域做同样的规范化（用它的最后一个未覆盖格），
// 就能 O(1) 判定剩下的部分是不是某块碎片。
//
// 「最后一个未覆盖格」用行优先序后缀计数 + 二分查找求出，每块碎片预先存好按 64 列分块的
// 行内前缀计数与位集合，使单次判定为 O(1)。

static const int STRIDE = 1024;                 // 线性化列宽，保证不同坐标不会碰撞
static const int KC = 1 << 20;                  // 规范化锚点下标，大于任何格子的线性下标
static const int MOD0 = 975919579, MUL0 = 382737283;
static const int MOD1 = 975979579, MUL1 = 382878283;

static int* PW0; // 模 MOD0 的幂表
static int* PW1; // 模 MOD1 的幂表

static inline int mulmod(int a, int b, int mod) { return (int)((long long)a * b % mod); }

// 一块碎片（已裁去四周空白，边界紧贴非空格）
struct Pat {
    int h, w; // 尺寸
    vector<string> g;
    int total; // 非空格个数
};

// 碎片的某个朝向（先水平翻转再顺时针旋转 k 次）
struct Var {
    int pid, f, k;
    int rows, cols, total;
    int lr, lc;    // 该朝向下行优先序最靠后的非空格
    int h0, h1;    // 该朝向的多项式哈希
    int key0, key1; // 规范化到锚点 KC 后的哈希
    int nwords;
    vector<unsigned long long> bits; // 每行非空格位集合
    vector<unsigned short> pre;      // 每行按 64 列分块的前缀非空格计数
    vector<int> rowSuf;              // rowSuf[i] = 第 i 行及其后所有行的非空格数
};

static vector<Pat> pats;    // 去重后的碎片形状
static vector<int> patCnt;  // 每个形状对应的实际碎片块数
static vector<Var> vars;    // 所有能放进原图的朝向
static unordered_map<unsigned long long, int> keyMap; // 规范化哈希 -> 朝向编号
static unordered_map<unsigned long long, int> sigMap; // 形状签名 -> 形状编号

static char* tg = 0;   // 原图（裁剪后）的扁平字符数组
static int TN, TM;     // 原图尺寸
static int totalCells; // 原图非空格总数
static int* tsum = 0;  // 行优先序后缀非空格计数
static int* stampArr = 0;
static int stampCur = 0;

static char* found = 0; // found[(a*np+b)*np+c]：三个形状（升序）能拼成原图

// 当前查询中已放置的块
static int G_V[4], G_R[4], G_C[4], G_N = 0;

// 坐标变换：先水平翻转（f=1 时），再顺时针旋转 k 次，返回新坐标
static inline void mapCell(int h, int w, int f, int k, int r, int c, int& oi, int& oj) {
    int r1 = r;
    int c1 = f ? (w - 1 - c) : c;
    if (k == 0) {
        oi = r1;
        oj = c1;
    } else if (k == 1) {
        oi = c1;
        oj = h - 1 - r1;
    } else if (k == 2) {
        oi = h - 1 - r1;
        oj = w - 1 - c1;
    } else {
        oi = w - 1 - c1;
        oj = r1;
    }
}

static void initPow() {
    PW0 = (int*)malloc(sizeof(int) * (KC + 1));
    PW1 = (int*)malloc(sizeof(int) * (KC + 1));
    PW0[0] = PW1[0] = 1;
    for (int i = 1; i <= KC; i++) {
        PW0[i] = mulmod(PW0[i - 1], MUL0, MOD0);
        PW1[i] = mulmod(PW1[i - 1], MUL1, MOD1);
    }
}

// 裁剪掉四周全为空的行列，得到紧贴边界的碎片
static void cropGrid(vector<string>& g, int& h, int& w, int& total) {
    int R = (int)g.size();
    int C = R ? (int)g[0].size() : 0;
    int mnr = R, mxr = -1, mnc = C, mxc = -1;
    total = 0;
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            if (g[i][j] == '.') continue;
            total++;
            if (i < mnr) mnr = i;
            if (i > mxr) mxr = i;
            if (j < mnc) mnc = j;
            if (j > mxc) mxc = j;
        }
    }
    vector<string> ng;
    ng.reserve(mxr - mnr + 1);
    for (int i = mnr; i <= mxr; i++) ng.push_back(g[i].substr(mnc, mxc - mnc + 1));
    g.swap(ng);
    h = (int)g.size();
    w = h ? (int)g[0].size() : 0;
}

static vector<string> readGrid() {
    static char buf[16384];
    int R, C;
    scanf("%d %d", &R, &C);
    vector<string> g;
    g.reserve(R);
    for (int i = 0; i < R; i++) {
        scanf("%s", buf);
        g.push_back(string(buf));
    }
    return g;
}

// 建立某个朝向的数据结构；放不进原图的朝向直接丢弃（它不可能参与任何方案）
static void buildVariant(int pid, int o, long long H0, long long H1) {
    const Pat& P = pats[pid];
    int f = o >> 2, k = o & 3;
    int rows = (k & 1) ? P.w : P.h;
    int cols = (k & 1) ? P.h : P.w;
    if (rows > TN || cols > TM) return;

    Var V;
    V.pid = pid;
    V.f = f;
    V.k = k;
    V.rows = rows;
    V.cols = cols;
    V.total = P.total;
    V.nwords = (cols + 63) >> 6;
    V.bits.assign((size_t)rows * V.nwords, 0ULL);
    V.pre.assign((size_t)rows * (V.nwords + 1), 0);
    V.rowSuf.assign(rows + 1, 0);

    int lr = -1, lc = -1;
    for (int r = 0; r < P.h; r++) {
        for (int c = 0; c < P.w; c++) {
            if (P.g[r][c] == '.') continue;
            int i, j;
            mapCell(P.h, P.w, f, k, r, c, i, j);
            V.bits[(size_t)i * V.nwords + (j >> 6)] |= 1ULL << (j & 63);
            V.rowSuf[i]++;
            if (i > lr || (i == lr && j > lc)) {
                lr = i;
                lc = j;
            }
        }
    }
    for (int r = rows - 1; r >= 0; r--) V.rowSuf[r] += V.rowSuf[r + 1];
    for (int r = 0; r < rows; r++) {
        unsigned short* pr = &V.pre[(size_t)r * (V.nwords + 1)];
        int acc = 0;
        for (int q = 0; q < V.nwords; q++) {
            pr[q] = (unsigned short)acc;
            acc += __builtin_popcountll(V.bits[(size_t)r * V.nwords + q]);
        }
        pr[V.nwords] = (unsigned short)acc;
    }
    V.lr = lr;
    V.lc = lc;
    V.h0 = (int)H0;
    V.h1 = (int)H1;
    int li = lr * STRIDE + lc;
    V.key0 = mulmod((int)H0, PW0[KC - li], MOD0);
    V.key1 = mulmod((int)H1, PW1[KC - li], MOD1);

    vars.push_back(V);
    unsigned long long key = ((unsigned long long)V.key0 << 32) | (unsigned)V.key1;
    keyMap.insert(make_pair(key, (int)vars.size() - 1));
}

// 读入一块碎片；相同形状的碎片只建一次朝向，用计数区分块数
static void addPiece(vector<string> g) {
    int h, w, tot;
    cropGrid(g, h, w, tot);

    long long hs0[8], hs1[8];
    long long sig0 = -1, sig1 = -1;
    for (int o = 0; o < 8; o++) {
        int f = o >> 2, k = o & 3;
        long long a = 0, b = 0;
        for (int r = 0; r < h; r++) {
            for (int c = 0; c < w; c++) {
                char ch = g[r][c];
                if (ch == '.') continue;
                int i, j;
                mapCell(h, w, f, k, r, c, i, j);
                long long v = 26 + (ch - 'a'); // 加 26 保证任意两块相减的结果都不会像合法颜色
                a = (a + v * PW0[i * STRIDE + j]) % MOD0;
                b = (b + v * PW1[i * STRIDE + j]) % MOD1;
            }
        }
        hs0[o] = a;
        hs1[o] = b;
        if (sig0 < 0 || a < sig0 || (a == sig0 && b < sig1)) {
            sig0 = a;
            sig1 = b;
        }
    }
    // 用「8 个朝向哈希的最小值」作为旋转翻转意义下的形状签名
    unsigned long long key = ((unsigned long long)sig0 << 32) | (unsigned)sig1;
    unordered_map<unsigned long long, int>::iterator sit = sigMap.find(key);
    if (sit != sigMap.end()) { // 与已有形状等价，只增加块数
        patCnt[sit->second]++;
        return;
    }
    Pat P;
    P.h = h;
    P.w = w;
    P.g = g;
    P.total = tot;
    pats.push_back(P);
    patCnt.push_back(1);
    int pid = (int)pats.size() - 1;
    sigMap.insert(make_pair(key, pid));
    for (int o = 0; o < 8; o++) buildVariant(pid, o, hs0[o], hs1[o]);
}

// 一块碎片放在 (orr,occ) 时是否完全盖在原图同色非空格上；mark 为真时顺便标记占用并检测重叠
static bool checkPlacement(const Var& V, int orr, int occ, bool mark) {
    const Pat& P = pats[V.pid];
    int st = stampCur;
    for (int r = 0; r < P.h; r++) {
        for (int c = 0; c < P.w; c++) {
            char ch = P.g[r][c];
            if (ch == '.') continue;
            int i, j;
            mapCell(P.h, P.w, V.f, V.k, r, c, i, j);
            int rr = orr + i;
            int cc = occ + j;
            if (rr < 0 || rr >= TN || cc < 0 || cc >= TM) return false;
            int id = rr * TM + cc;
            if (tg[id] != ch) return false; // 颜色不同或原图此处为空，都不合法
            if (mark) {
                if (stampArr[id] == st) return false; // 与已放置的块重叠
                stampArr[id] = st;
            }
        }
    }
    return true;
}

// 一块碎片与某个查询位置的行优先序后缀贡献：统计该碎片中线性下标 >= (r,c) 的非空格数
static inline int contribution(int vi, int orr, int occ, int r, int c) {
    const Var& V = vars[vi];
    int er = r - orr;
    int ec = c - occ;
    if (ec >= V.cols) { // 查询列在该块右侧：该块只有更下面的行还算数
        er++;
        ec = 0;
    }
    if (er >= V.rows) return 0; // 整块都在查询位置上方
    if (er < 0) return V.total; // 整块都在查询位置下方
    int k = ec > 0 ? ec : 0;
    int wq = k >> 6;
    const unsigned short* pr = &V.pre[(size_t)er * (V.nwords + 1)];
    int got = pr[wq];
    int rem = k & 63;
    if (rem) got += __builtin_popcountll(V.bits[(size_t)er * V.nwords + wq] & ((1ULL << rem) - 1));
    return V.rowSuf[er] - got;
}

// 二分求「原图去掉已放置的 G_N 块后，行优先序最靠后的未覆盖非格」的线性下标
static int lastUncovered() {
    int lo = 0, hi = TN * TM - 1;
    while (lo < hi) {
        int md = (lo + hi + 1) >> 1;
        int r = md / TM;
        int c = md - r * TM;
        int val = tsum[md];
        for (int t = 0; t < G_N; t++) val -= contribution(G_V[t], G_R[t], G_C[t], r, c);
        if (val > 0) lo = md;
        else hi = md - 1;
    }
    return lo;
}

int main() {
    initPow();

    int K;
    if (scanf("%d", &K) != 1) return 0;

    vector<string> tgrid = readGrid();
    int th, tw, ttot;
    cropGrid(tgrid, th, tw, ttot);
    TN = th;
    TM = tw;
    totalCells = ttot;
    tg = (char*)malloc((size_t)TN * TM);
    for (int i = 0; i < TN; i++)
        for (int j = 0; j < TM; j++) tg[i * TM + j] = tgrid[i][j];

    tsum = (int*)malloc(sizeof(int) * (size_t)TN * TM);
    stampArr = (int*)calloc((size_t)TN * TM, sizeof(int));
    int acc = 0;
    int lastIdx = -1;
    for (int id = TN * TM - 1; id >= 0; id--) {
        if (tg[id] != '.') {
            acc++;
            if (lastIdx < 0) lastIdx = id; // 行优先序最靠后的非空格
        }
        tsum[id] = acc;
    }

    long long HT0 = 0, HT1 = 0;
    for (int i = 0; i < TN; i++) {
        for (int j = 0; j < TM; j++) {
            char ch = tg[i * TM + j];
            if (ch == '.') continue;
            long long v = 26 + (ch - 'a');
            int e = i * STRIDE + j;
            HT0 = (HT0 + v * PW0[e]) % MOD0;
            HT1 = (HT1 + v * PW1[e]) % MOD1;
        }
    }

    for (int i = 0; i < K; i++) addPiece(readGrid());

    int np = (int)pats.size();
    found = (char*)calloc((size_t)np * np * np, 1);

    int tmR = lastIdx / TM, tmC = lastIdx - tmR * TM;

    // 第一块必须让它最靠后的格子落在原图最后一个非空格上，位置唯一确定
    vector<int> s1v;
    vector<int> s1r, s1c;
    for (int v = 0; v < (int)vars.size(); v++) {
        const Var& V = vars[v];
        int orr = tmR - V.lr, occ = tmC - V.lc;
        if (orr < 0 || occ < 0) continue;
        if (orr + V.rows > TN || occ + V.cols > TM) continue;
        if (!checkPlacement(V, orr, occ, false)) continue;
        s1v.push_back(v);
        s1r.push_back(orr);
        s1c.push_back(occ);
    }

    int nv = (int)vars.size();
    for (int a = 0; a < (int)s1v.size(); a++) {
        const Var& V1 = vars[s1v[a]];
        G_N = 1;
        G_V[0] = s1v[a];
        G_R[0] = s1r[a];
        G_C[0] = s1c[a];
        int t2 = lastUncovered();
        int t2r = t2 / TM, t2c = t2 - t2r * TM;

        int off1 = s1r[a] * STRIDE + s1c[a];
        long long base0 = (HT0 - (long long)V1.h0 * PW0[off1]) % MOD0;
        long long base1 = (HT1 - (long long)V1.h1 * PW1[off1]) % MOD1;
        if (base0 < 0) base0 += MOD0;
        if (base1 < 0) base1 += MOD1;

        for (int j = 0; j < nv; j++) {
            const Var& V2 = vars[j];
            int orr = t2r - V2.lr, occ = t2c - V2.lc;
            if (orr < 0 || occ < 0) continue;
            if (orr + V2.rows > TN || occ + V2.cols > TM) continue;

            G_N = 2;
            G_V[1] = j;
            G_R[1] = orr;
            G_C[1] = occ;
            int t3 = lastUncovered();
            int t3r = t3 / TM, t3c = t3 - t3r * TM;

            int off2 = orr * STRIDE + occ;
            long long rem0 = (base0 - (long long)V2.h0 * PW0[off2]) % MOD0;
            long long rem1 = (base1 - (long long)V2.h1 * PW1[off2]) % MOD1;
            if (rem0 < 0) rem0 += MOD0;
            if (rem1 < 0) rem1 += MOD1;

            int anch = KC - (t3r * STRIDE + t3c);
            int k0 = mulmod((int)rem0, PW0[anch], MOD0);
            int k1 = mulmod((int)rem1, PW1[anch], MOD1);
            unsigned long long key = ((unsigned long long)k0 << 32) | (unsigned)k1;
            unordered_map<unsigned long long, int>::iterator it = keyMap.find(key);
            if (it == keyMap.end()) continue;

            int q = it->second;
            const Var& V3 = vars[q];
            int orr3 = t3r - V3.lr, occ3 = t3c - V3.lc;
            if (orr3 < 0 || occ3 < 0) continue;
            if (orr3 + V3.rows > TN || occ3 + V3.cols > TM) continue;
            if (V1.total + V2.total + V3.total != totalCells) continue; // 格数不匹配肯定不对

            // 完整校验这一次拼接（颜色、越界、两两不重叠）
            stampCur++;
            if (!checkPlacement(V1, s1r[a], s1c[a], true)) continue;
            if (!checkPlacement(V2, orr, occ, true)) continue;
            if (!checkPlacement(V3, orr3, occ3, true)) continue;

            int x = V1.pid, y = V2.pid, z = V3.pid;
            if (x > y) swap(x, y);
            if (y > z) swap(y, z);
            if (x > y) swap(x, y);
            found[((size_t)x * np + y) * np + z] = 1;
        }
    }

    long long ans = 0;
    for (int a = 0; a < np; a++) {
        for (int b = a; b < np; b++) {
            for (int c = b; c < np; c++) {
                if (!found[((size_t)a * np + b) * np + c]) continue;
                long long ca = patCnt[a], cb = patCnt[b], cc = patCnt[c];
                if (a == c) ans += ca * (ca - 1) * (ca - 2) / 6; // 三块同形状
                else if (a == b) ans += ca * (ca - 1) / 2 * cc;  // 前两块同形状
                else if (b == c) ans += ca * cb * (cb - 1) / 2;  // 后两块同形状
                else ans += ca * cb * cc;                        // 三块形状互不相同
            }
        }
    }
    printf("%lld\n", ans);
    return 0;
}
