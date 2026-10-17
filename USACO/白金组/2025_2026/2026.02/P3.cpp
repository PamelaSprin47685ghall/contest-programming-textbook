#include <cstdio>

using namespace std;

/*
题意：树上的随机游走。当前点有儿子时等概率走向一个儿子；
没有儿子（叶子）时等概率走向它的一个祖先。多组询问 (x,y)，
求从 x 出发首次到达 y 的期望步数，模 1e9+7。

做法（详见 P3.md）：
  up[i]   从 i 出发首次到达 i 的某个祖先的期望时间
  down[i] 从 par[i] 出发首次到达 i 的期望时间
  eVal[i] 从根出发首次到达 i 的期望时间
所有量都只依赖祖先链上的信息，故可线性递推。
输入保证 par[i] < i，所以不必递归 DFS：
逆序扫描即可先算完所有儿子（编号更大），顺序扫描即可保证父亲先算完。
*/

const int MAXN = 200005;
const int LOG = 19;              // 2^18 > 2*10^5，19 层足以覆盖任意祖先
const long long MOD = 1000000007LL;

int n, q;
int par[MAXN];                   // 父亲结点编号（根为 0）
int dep[MAXN];                   // 祖先个数：根为 0，儿子为父亲 +1
int cnt[MAXN];                   // 儿子个数
long long up[MAXN];              // 见文件头注释
long long down[MAXN];
long long eVal[MAXN];
long long esum[MAXN];            // 根..i 路径上 eVal 之和（含 i）
long long upsum[MAXN];           // i 的所有儿子的 up 之和
long long upd[MAXN];             // 根..i 路径上（不含根）up[z]/(dep[z]+1) 之和
int jmp[LOG][MAXN];              // 倍增表：jmp[k][i] 是 i 的第 2^k 级祖先

long long modpow(long long b, long long p) {
    long long r = 1;
    while (p) {
        if (p & 1) r = r * b % MOD;
        b = b * b % MOD;
        p >>= 1;
    }
    return r;
}

// 模意义下的除法一律换成乘逆元
long long inv(long long x) {
    return modpow(x, MOD - 2);
}

int main() {
    scanf("%d %d", &n, &q);
    for (int i = 1; i < n; i++) {
        int p;
        scanf("%d", &p);
        par[i] = p - 1;
        cnt[par[i]]++;
    }
    for (int i = 1; i < n; i++) dep[i] = dep[par[i]] + 1;

    // 逆序递推 up：儿子的编号一定大于父亲，所以轮到 i 时它的儿子都已算完
    for (int i = n - 1; i >= 1; i--) {
        if (cnt[i] == 0) {
            up[i] = 1;           // 叶子下一步必然落在某个祖先上
        } else {
            // 先走到父亲那一层或直接跳到某个祖先，落到自己身上后再重来一次
            // up_i = (1 + 1/dep_i) * (1 + (1/|C_i|) * sum up_c)
            up[i] = (1 + inv(dep[i])) % MOD
                    * ((1 + inv(cnt[i]) * upsum[i]) % MOD) % MOD;
        }
        upsum[par[i]] = (upsum[par[i]] + up[i]) % MOD;
    }

    // 顺序递推 eVal/upd：父下标小于子下标，保证父亲的信息已经就绪
    for (int i = 1; i < n; i++) {
        int p = par[i];
        long long cp = cnt[p];                             // 父亲的儿子个数
        long long sib = (upsum[p] - up[i] + MOD) % MOD;    // 兄弟的 up 之和
        // down_i = |C_p| + sum_{c != i} up_c
        //          + (|C_p|-1) * (E_p - (1/dep_i) * sum_{a in A_i} E_a)
        long long t = (eVal[p] - inv(dep[i]) * esum[p]) % MOD;
        if (t < 0) t += MOD;
        down[i] = (cp + sib + (cp - 1) * t) % MOD;
        eVal[i] = (eVal[p] + down[i]) % MOD;
        esum[i] = (esum[p] + eVal[i]) % MOD;
        // 路径和差分用：x 到 LCA 之间的 up/(dep+1) 之和 = upd[par[x]] - upd[l]
        upd[i] = (upd[p] + up[i] * inv(dep[i] + 1)) % MOD;
    }

    // 倍增预处理 LCA；根的父亲指向自己，省掉越界判断
    for (int i = 0; i < n; i++) jmp[0][i] = (i == 0 ? 0 : par[i]);
    for (int k = 1; k < LOG; k++)
        for (int i = 0; i < n; i++)
            jmp[k][i] = jmp[k - 1][jmp[k - 1][i]];

    while (q--) {
        int x, y;
        scanf("%d %d", &x, &y);
        x--;
        y--;

        // 求 LCA
        int a = x, b = y;
        if (dep[a] < dep[b]) { int tmp = a; a = b; b = tmp; }
        int diff = dep[a] - dep[b];
        for (int k = 0; k < LOG; k++)
            if ((diff >> k) & 1) a = jmp[k][a];
        int l;
        if (a == b) {
            l = a;
        } else {
            for (int k = LOG - 1; k >= 0; k--)
                if (jmp[k][a] != jmp[k][b]) { a = jmp[k][a]; b = jmp[k][b]; }
            l = par[a];
        }

        long long ans;
        if (l == x) {
            // x 是 y 的祖先：从根到 y = 从根到 x 再加 x 到 y
            ans = (eVal[y] - eVal[x] + MOD) % MOD;
        } else {
            // 第一段在公共祖先集上等概率平均，等于沿 x 到 l 的链累加
            long long reachm = (up[x] + upd[par[x]] - upd[l] + MOD) % MOD;
            // 第二段：E_y - (1/|S|) * sum_{w in S} E_w
            long long wtoy = (eVal[y] - inv(dep[l] + 1) * esum[l] % MOD + MOD) % MOD;
            ans = (reachm + wtoy) % MOD;
        }
        printf("%lld\n", ans);
    }
    return 0;
}
