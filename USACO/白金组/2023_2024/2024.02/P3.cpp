#include <cstdio>

using namespace std;

// 题意：城市 i 的传送门周期 T_i 是 2 的幂，且 sum T_i <= 1e5。
// 在 (城市, 天) 上单步前进 = 一天。查询从 (v, t) 走 Delta 步后的城市。
//
// 关键观察：把城市按 T_i 的大小分层（层号 = log2 T_i）。
// 因为所有 T_i 都是 2 的幂，T 小的层天然是 T 大的层的"取模细化"，
// 所以从任意状态出发，定义：
//   Fh/FhDays ：第一次到达"更高层"城市（层号更大）的城市与所需天数；
//   Bl/BlDays ：同层内第 2^k 次到达"与起点同层"的城市（只数同层落脚点）
//               的城市与所需天数。
// 于是同层内的跳跃满足 Bl_days >= 2^k，并且可以在 O(1) 内拼接
// （两个半程都是同一个层，时间取模 T 后下标合法）。
//
// 查询时贪心：先把"能负担得起的去更高层的跳跃"全部用掉（层号严格递增，
// 至多 log S 次），再用 Bl 在同层里尽量走远，最后若还有剩余天数就走一步；
// 这一步必然落到更低的层，因此整个过程层号先升后降，每轮询问
// 只有 O(log S) 次外层循环。
//
// 注意 Bl 条目仅在 "Fh 不存在或 FhDays > 2^k" 时才有意义：
// 若 firstHigherDays <= 2^k，则该条目在查询中永远不可能被使用
// （使用它要求剩余天数 >= 条目天数 >= 2^k >= firstHigherDays，
//  而能跳到更高层时贪心会先跳），因此可以留空以省时间。

const int MAXN = 100005;  // 城市数上限
const int MAXM = 100005;  // sum T_i 上限
const int LG = 60;        // Delta <= 1e18 < 2^60
const long long LIMIT = 1000000000000000000LL;  // Delta 的上限

static int T[MAXN];      // 每个城市的周期
static int Off[MAXN];    // 城市 i 的数组在扁平数组中的起始下标
static int Cv[MAXM];     // c_{i,t}，扁平存储
static int Fh[MAXM];                 // 第一次到达更高层城市（0 表示不存在）
static long long FhDays[MAXM];       // 到达它的天数
static int Bl[MAXM * LG];            // 同层第 2^k 个落脚点
static long long BlDays[MAXM * LG];  // 到达它的天数
static bool Seen[MAXM];

// 显式栈实现 DFS（等价于官方解法里的递归），避免 1e5 长的链爆栈。
// 每一帧保存当前状态 (a, t) 以及推进过程里的 laterCity / days。
static int Ska[MAXM], Skt[MAXM], Sklater[MAXM], SklaterT[MAXM];
static long long Skdays[MAXM];
static bool Skres[MAXM];
static int SkTop;

inline long long readNum();

void dfs(int a0, int t0)
{
    SkTop = 0;
    {
        int id = Off[a0] + t0;
        Seen[id] = true;
        Ska[0] = a0;
        Skt[0] = t0;
        Sklater[0] = Cv[id];  // 先走一步
        Skdays[0] = 1;
        Skres[0] = false;
        SkTop = 1;
    }
    while (SkTop > 0) {
        int top = SkTop - 1;
        int a = Ska[top];
        if (Skres[top]) {
            // 子状态 (child, ct) 已经算完，接着推进本状态
            int child = Sklater[top];
            int ct = SklaterT[top];
            int cid = Off[child] + ct;
            if (T[child] == T[a]) {
                int pid = Off[a] + Skt[top];
                Bl[pid * LG] = child;
                BlDays[pid * LG] = Skdays[top];
            }
            Skdays[top] += FhDays[cid];
            Sklater[top] = Fh[cid];
            Skres[top] = false;
        }
        bool pushed = false;
        while (Sklater[top] != 0 && T[Sklater[top]] <= T[a]) {
            int later = Sklater[top];
            int lt = (int)((Skt[top] + Skdays[top]) % T[later]);
            int cid = Off[later] + lt;
            if (!Seen[cid]) {
                SklaterT[top] = lt;
                Skres[top] = true;
                Seen[cid] = true;
                Ska[SkTop] = later;
                Skt[SkTop] = lt;
                Sklater[SkTop] = Cv[cid];
                Skdays[SkTop] = 1;
                Skres[SkTop] = false;
                SkTop++;
                pushed = true;
                break;
            }
            // 已经访问过：可能是算完的（记忆化），也可能是栈上正在算的。
            // 后者意味着走路回到了自身，只会陷入同层死循环，
            // 此时 Fh/FhDays 恰好都保持 0，和最终值一致。
            if (T[later] == T[a]) {
                int pid = Off[a] + Skt[top];
                Bl[pid * LG] = later;
                BlDays[pid * LG] = Skdays[top];
            }
            Skdays[top] += FhDays[cid];
            Sklater[top] = Fh[cid];
        }
        if (pushed) continue;
        if (Sklater[top] != 0) {
            int pid = Off[a] + Skt[top];
            Fh[pid] = Sklater[top];
            FhDays[pid] = Skdays[top];
        }
        SkTop--;
    }
}

int main()
{
    int n = (int)readNum();
    int q = (int)readNum();
    for (int i = 1; i <= n; i++) T[i] = (int)readNum();

    int m = 0;
    for (int i = 1; i <= n; i++) {
        Off[i] = m;
        m += T[i];
    }
    for (int i = 1; i <= n; i++)
        for (int t = 0; t < T[i]; t++) Cv[Off[i] + t] = (int)readNum();

    // 第一步：算出每个状态的 Fh / FhDays 和同层第一步 Bl[.][0]
    for (int a = 1; a <= n; a++)
        for (int t = 0; t < T[a]; t++)
            if (!Seen[Off[a] + t]) dfs(a, t);

    // 第二步：同层内的倍增表，每次用两个 2^{k-1} 半程拼接
    for (int k = 1; k < LG; k++) {
        long long pw = 1LL << k;
        for (int a = 1; a <= n; a++) {
            int base = Off[a];
            for (int t = 0; t < T[a]; t++) {
                int id = base + t;
                // 若 2^k 天内已经能到更高层，该条目查询时用不到，留空
                if (Fh[id] != 0 && FhDays[id] <= pw) continue;
                int half = Bl[id * LG + k - 1];
                if (half == 0) continue;
                int hid = Off[half] + (int)((t + BlDays[id * LG + k - 1]) % T[half]);
                long long days = BlDays[id * LG + k - 1] + BlDays[hid * LG + k - 1];
                if (Bl[hid * LG + k - 1] != 0 && days <= LIMIT) {
                    Bl[id * LG + k] = Bl[hid * LG + k - 1];
                    BlDays[id * LG + k] = days;
                }
            }
        }
    }

    for (int j = 0; j < q; j++) {
        int cur = (int)readNum();
        long long day = readNum();
        long long rem = readNum();
        while (rem > 0) {
            // 能跳到更高层就跳（层号严格递增，至多 log S 次）
            while (true) {
                int id = Off[cur] + (int)(day % T[cur]);
                if (Fh[id] == 0 || rem < FhDays[id]) break;
                day += FhDays[id];
                rem -= FhDays[id];
                cur = Fh[id];
            }
            // 在当前层内尽量走远
            for (int k = LG - 1; k >= 0; k--) {
                int id = Off[cur] + (int)(day % T[cur]);
                if (Bl[id * LG + k] != 0 && rem >= BlDays[id * LG + k]) {
                    day += BlDays[id * LG + k];
                    rem -= BlDays[id * LG + k];
                    cur = Bl[id * LG + k];
                }
            }
            // 还有剩余天数：走一步，这一步必然落到更低的层
            if (rem > 0) {
                int id = Off[cur] + (int)(day % T[cur]);
                day++;
                rem--;
                cur = Cv[id];
            }
        }
        printf("%d\n", cur);
    }
    return 0;
}

// 快速读入：题目最大约 3.5e5 个整数，且时间戳可达 1e18
static char inbuf[1 << 20];
static int inpos = 0, inlen = 0;

inline int gc()
{
    if (inpos == inlen) {
        inlen = (int)fread(inbuf, 1, sizeof(inbuf), stdin);
        inpos = 0;
        if (inlen <= 0) return -1;
    }
    return inbuf[inpos++];
}

inline long long readNum()
{
    int c = gc();
    while (c != -1 && (c < '0' || c > '9')) c = gc();
    long long x = 0;
    while (c >= '0' && c <= '9') {
        x = x * 10 + (c - '0');
        c = gc();
    }
    return x;
}
