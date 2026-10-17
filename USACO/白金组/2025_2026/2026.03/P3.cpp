#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

// 记 p[i] 为覆盖位置 i 的 t=1 约束中 k 最大者，q[i] 为覆盖 i 的 t=2 约束中 k 最小者。
// 合法数组必然满足 k[p[i]] <= a[i] <= k[q[i]]（否则无解），而且
// "某位置取 k[h]" 只可能满足 p[i]=h 或 q[i]=h 的那一条约束，于是把每个位置看成
// 连接 p[i]、q[i] 的一条边（两者相同即自环），每条边只能去满足其中一个端点，
// 问题化为：能否给每条边选一个端点，使每个约束顶点至少被选中一次。
// 有解当且仅当每个连通块的边数 >= 点数；构造时每个连通块取一条环上的边判给一侧当根，
// 再沿生成树 DFS，把每条树边判给"尚未被满足"的孩子顶点。

int n, m;
vector<int> tt, kk, L, R;      // 约束：类型、k、左右端点
vector<int> pp, qq;            // 每个位置的 p[i]、q[i]，-1 表示不存在
vector<int> aa;                // 答案数组
vector<char> cov;              // 每个约束是否已被满足
vector<int> par;               // 顶点的并查集
vector<int> nxt1, nxt2;        // 求 p/q 时"下一个未赋值位置"的并查集

int findv(int x) {             // 顶点并查集：找根
    while (par[x] != x) { par[x] = par[par[x]]; x = par[x]; }
    return x;
}

int findn(int x, vector<int> &nx) {   // 位置并查集：找 >= x 的第一个未赋值位置
    while (nx[x] != x) { nx[x] = nx[nx[x]]; x = nx[x]; }
    return x;
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d %d", &n, &m);
        tt.assign(m + 1, 0); kk.assign(m + 1, 0);
        L.assign(m + 1, 0); R.assign(m + 1, 0);
        vector<int> ord1, ord2;
        for (int h = 1; h <= m; h++) {
            scanf("%d %d %d %d", &tt[h], &L[h], &R[h], &kk[h]);
            if (tt[h] == 1) ord1.push_back(h); else ord2.push_back(h);
        }
        // 用"按 k 排序 + 并查集跳过后已赋值位置"直接得到每个位置的 p[i]、q[i]
        pp.assign(n + 2, -1); qq.assign(n + 2, -1);
        nxt1.resize(n + 3); nxt2.resize(n + 3);
        for (int i = 1; i <= n + 1; i++) { nxt1[i] = i; nxt2[i] = i; }
        sort(ord1.begin(), ord1.end(), [](int x, int y) { return kk[x] > kk[y]; });
        for (size_t j = 0; j < ord1.size(); j++) {
            int h = ord1[j];
            for (int i = findn(L[h], nxt1); i <= R[h]; i = findn(i, nxt1)) {
                pp[i] = h; nxt1[i] = findn(i + 1, nxt1);
            }
        }
        sort(ord2.begin(), ord2.end(), [](int x, int y) { return kk[x] < kk[y]; });
        for (size_t j = 0; j < ord2.size(); j++) {
            int h = ord2[j];
            for (int i = findn(L[h], nxt2); i <= R[h]; i = findn(i, nxt2)) {
                qq[i] = h; nxt2[i] = findn(i + 1, nxt2);
            }
        }
        // 基准取值 + 可行性检查
        aa.assign(n + 1, 0);
        cov.assign(m + 1, 0);
        bool ok = true;
        for (int i = 1; i <= n; i++) {
            int u = pp[i], v = qq[i];
            if (u != -1 && v != -1 && kk[u] > kk[v]) { ok = false; break; }
            if (u != -1) aa[i] = kk[u];
        }
        if (!ok) { printf("-1\n"); continue; }
        for (int i = 1; i <= n; i++) {
            int v = pp[i] != -1 ? pp[i] : qq[i];
            if (v != -1 && aa[i] == kk[v]) cov[v] = 1;
        }
        // 建图：每个位置一条边，先按位置升序做并查集，分出树边与非树边
        par.resize(m + 1);
        for (int h = 1; h <= m; h++) par[h] = h;
        vector<int> hhl(n + 1, -1), hhr(n + 1, -1);
        vector<int> teE, teU, teV, cyc;
        for (int i = 1; i <= n; i++) {
            int u = pp[i] != -1 ? pp[i] : qq[i];
            int v = qq[i] != -1 ? qq[i] : pp[i];
            if (u == -1) continue;
            hhl[i] = u; hhr[i] = v;
            int ru = findv(u), rv = findv(v);
            if (ru == rv) cyc.push_back(i);
            else { par[ru] = rv; teE.push_back(i); teU.push_back(u); teV.push_back(v); }
        }
        // 每个连通块必须含环（即边数 >= 点数），否则无解
        vector<int> compFirst(m + 1, -1);
        for (size_t j = 0; j < cyc.size(); j++) {
            int i = cyc[j], r = findv(hhl[i]);
            if (compFirst[r] == -1) compFirst[r] = i;
        }
        for (int h = 1; h <= m && ok; h++)
            if (compFirst[findv(h)] == -1) ok = false;
        if (!ok) { printf("-1\n"); continue; }
        // 生成树上的邻接表
        vector<int> head(m + 1, -1);
        vector<int> nextE(2 * teE.size(), -1), toE(2 * teE.size(), 0), idE(2 * teE.size(), 0);
        int cnt = 0;
        for (size_t j = 0; j < teE.size(); j++) {
            int i = teE[j], u = teU[j], v = teV[j];
            nextE[cnt] = head[u]; toE[cnt] = v; idE[cnt] = i; head[u] = cnt++;
            nextE[cnt] = head[v]; toE[cnt] = u; idE[cnt] = i; head[v] = cnt++;
        }
        // 逐连通块：把"最小位置的那条非树边"判给 hhr 端点当根，再 DFS 覆盖其余顶点
        vector<char> vis(m + 1, 0);
        for (int h = 1; h <= m; h++) {
            int r = findv(h);
            if (compFirst[r] == -1) continue;
            int e0 = compFirst[r];
            compFirst[r] = -1;
            int root = hhr[e0];
            aa[e0] = kk[root]; cov[root] = 1;
            vector<int> st;
            st.push_back(root); vis[root] = 1;
            while (!st.empty()) {
                int x = st.back(); st.pop_back();
                for (int e = head[x]; e != -1; e = nextE[e]) {
                    int y = toE[e];
                    if (vis[y]) continue;
                    vis[y] = 1;
                    if (!cov[y]) { aa[idE[e]] = kk[y]; cov[y] = 1; }
                    st.push_back(y);
                }
            }
        }
        for (int i = 1; i <= n; i++) printf("%d%c", aa[i], i == n ? '\n' : ' ');
    }
    return 0;
}
