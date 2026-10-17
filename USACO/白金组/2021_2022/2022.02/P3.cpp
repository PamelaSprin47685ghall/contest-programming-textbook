// USACO 2022 February Contest, Platinum Problem 3: Phone Numbers
// 按位置推进的计数 DP：状态 = (最近 4 个 dp 布尔值, 最近 3 个已选数字)，
// 用哈希表把等价的 (状态) 合并起来计数，每个位置间只有常数个状态。
#include <cstdio>
#include <cstring>

using namespace std;

const int MOD = 1000000007;
const int MAXN = 100005;
// bars 占低 4 位, 高位放 3 位数字(每位 0..9), 状态总空间 16 * 1000
const int STATES = 16000;

// 掩码位 1<<d (d=1..9), 最大到 (1<<9)|... = 1022, 故数组开到 1024
bool good_pair[1024];    // 该掩码是否恰为"共享一条边的两个数字"
bool good_square[1024];  // 该掩码是否恰为"2*2 的四个数字"

int S[MAXN];    // 输入串, 1 起下标
int mk2[MAXN];  // S[p-1..p] 的位掩码
int mk4[MAXN];  // S[p-3..p] 的位掩码

// 两张表交替使用；只遍历"出现过"的键来清空，省掉每个位置的全表清零
int valA[STATES], markA[STATES], keysA[STATES], nkA;
int valB[STATES], markB[STATES], keysB[STATES], nkB;

void addKey(int *val, int *mark, int *keys, int &nk, int key, int v) {
    if (!mark[key]) {
        mark[key] = 1;
        val[key] = 0;
        keys[nk++] = key;
    }
    int nv = val[key] + v;
    if (nv >= MOD) nv -= MOD;
    val[key] = nv;
}

void clearTable(int *val, int *mark, int *keys, int &nk) {
    for (int i = 0; i < nk; i++) {
        val[keys[i]] = 0;
        mark[keys[i]] = 0;
    }
    nk = 0;
}

int main() {
    // 预处理所有合法的"一次按键"数字集合
    for (int a = 1; a <= 9; a++) {
        for (int b = 1; b <= 9; b++) {
            if (a == b) continue;
            int ra = (a - 1) / 3, ca = (a - 1) % 3;
            int rb = (b - 1) / 3, cb = (b - 1) % 3;
            int dr = ra > rb ? ra - rb : rb - ra;
            int dc = ca > cb ? ca - cb : cb - ca;
            if (dr + dc == 1) good_pair[(1 << a) | (1 << b)] = true;
        }
    }
    for (int r = 0; r < 2; r++) {
        for (int c = 0; c < 2; c++) {
            int m = (1 << (r * 3 + c + 1)) | (1 << (r * 3 + c + 2)) |
                    (1 << ((r + 1) * 3 + c + 1)) | (1 << ((r + 1) * 3 + c + 2));
            good_square[m] = true;
        }
    }

    int tc;
    if (scanf("%d", &tc) != 1) return 0;
    char buf[MAXN];
    while (tc--) {
        scanf("%s", buf);
        int N = (int)strlen(buf);
        for (int i = 1; i <= N; i++) S[i] = buf[i - 1] - '0';
        for (int p = 1; p <= N; p++) {
            mk2[p] = (p >= 2) ? ((1 << S[p - 1]) | (1 << S[p])) : 0;
            int m = 0;
            for (int k = 0; k < 4 && p - k >= 1; k++) m |= 1 << S[p - k];
            mk4[p] = m;
        }

        int *cv = valA, *cm = markA, *ck = keysA, *cn = &nkA;
        int *nv = valB, *nm = markB, *nk = keysB, *nn = &nkB;
        *cn = 0;
        *nn = 0;
        addKey(cv, cm, ck, *cn, 1, 1);  // 空前缀: dp[0]=真, 还未选任何数字

        for (int p = 1; p <= N; p++) {
            // 第 p 位数字必须落在 S 的某一块里, 而该块完全被 S[p-3..p+3] 覆盖
            int cand[10], ncan = 0;
            bool have[10] = {false};
            for (int q = p - 3; q <= p + 3; q++) {
                if (q < 1 || q > N) continue;
                int d = S[q];
                if (!have[d]) {
                    have[d] = true;
                    cand[ncan++] = d;
                }
            }
            int sp = S[p];
            for (int ki = 0; ki < *cn; ki++) {
                int key = ck[ki];
                int bars = key & 15;  // bit j = dp[t-j]
                int nums = key >> 4;  // 从新到旧的三位数字
                int d0 = nums % 10, d1 = (nums / 10) % 10, d2 = nums / 100;
                int ways = cv[key];
                int shift = (bars & 7) << 1;  // 老 dp 值整体左移一位
                int need1 = (bars & 1) ? sp : 0;      // 长度 1 的块
                int need2 = (bars & 2) ? mk2[p] : 0;  // 长度 2 的块
                int need4 = (bars & 8) ? mk4[p] : 0;  // 长度 4 的块
                bool ok2 = good_pair[need2];
                bool ok4 = good_square[need4];
                int head = 1 << d0;
                int tail = (1 << d1) | (1 << d2);
                for (int ci = 0; ci < ncan; ci++) {
                    int d = cand[ci];
                    int nb = shift;
                    if (d == need1) nb |= 1;
                    // 数字集合相同即多重集相同: 合法块内数字互不相同
                    int st2 = (1 << d) | head;
                    if (ok2 && st2 == need2) nb |= 1;
                    int st4 = st2 | tail;
                    if (ok4 && st4 == need4) nb |= 1;
                    if (!nb) continue;  // 最近 4 个 dp 全假, 之后不可能再变真
                    // 丢掉最后一次分割点及其之前的数字: 未来的块碰不到它们
                    int nnums;
                    if (nb & 8) nnums = d + 10 * d0 + 100 * d1;
                    else if (nb & 4) nnums = d + 10 * d0;
                    else if (nb & 2) nnums = d;
                    else nnums = 0;
                    addKey(nv, nm, nk, *nn, nb | (nnums << 4), ways);
                }
            }
            clearTable(cv, cm, ck, *cn);
            int *t;
            t = cv; cv = nv; nv = t;
            t = cm; cm = nm; nm = t;
            t = ck; ck = nk; nk = t;
            t = cn; cn = nn; nn = t;
        }
        int ans = 0;
        for (int ki = 0; ki < *cn; ki++) {
            int key = ck[ki];
            if (key & 1) {  // dp[N] 为真
                ans += cv[key];
                if (ans >= MOD) ans -= MOD;
            }
        }
        clearTable(cv, cm, ck, *cn);
        printf("%d\n", ans);
    }
    return 0;
}
