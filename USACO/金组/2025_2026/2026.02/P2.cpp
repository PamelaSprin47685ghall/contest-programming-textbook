// P2 Lexicographically Smallest Path
// 记 S_l 为从 1 出发、走恰好 l 条边能得到的字典序最小串，V_l 为能走出 S_l 的终点集合。
// 图是无向的，可以来回走，因此 S_l 一定是 S_{l+1} 的前缀，且每个字符都比上一个更小或相等。
// 于是 S_{l+1} = S_l + c，其中 c 是 V_l 中所有邻边的字符最小值；
// 终点集合 V_{l+1} = V_l 中取字符 c 的边所到的新点。
// 已经赋过答案的点（属于更早的 V）不必再扩展：它们邻边的最小字符不可能比当前的更小，
// 所以只需维护"新加入的点"，每个点只入队一次，总复杂度 O(N + M)。
#include <cstdio>

using namespace std;

const int MAXN = 200005;
const int MAXM = 200005;

int n, m;
int head[MAXN];                  // 邻接表头（有向弧，无向边存两条）
int enxt[2 * MAXM], eto[2 * MAXM], echar[2 * MAXM];
int ecnt;
int ans[MAXN];                   // f(1, i) 的长度，-1 表示不存在
int act[MAXN], nact[MAXN];       // 当前层的点、下一层的点

void solve() {
    scanf("%d %d", &n, &m);
    ecnt = 0;
    for (int i = 1; i <= n; i++) {
        head[i] = -1;
        ans[i] = -1;
    }
    for (int i = 0; i < m; i++) {
        int u, v;
        char c;
        scanf("%d %d %c", &u, &v, &c); // 格式串里的空格负责吃掉换行等空白
        eto[ecnt] = v; echar[ecnt] = c - 'a'; enxt[ecnt] = head[u]; head[u] = ecnt++;
        if (u != v) { // 自环只存一条，省一半空间
            eto[ecnt] = u; echar[ecnt] = c - 'a'; enxt[ecnt] = head[v]; head[v] = ecnt++;
        }
    }
    ans[1] = 0;
    int an = 0;
    act[an++] = 1;
    while (an > 0) {
        int minc = 26;
        for (int i = 0; i < an; i++) {
            int v = act[i];
            for (int e = head[v]; e != -1; e = enxt[e])
                if (echar[e] < minc) minc = echar[e];
        }
        if (minc == 26) break; // 当前层再也没有出边，后面的点都不可达
        int bn = 0;
        for (int i = 0; i < an; i++) {
            int v = act[i];
            for (int e = head[v]; e != -1; e = enxt[e]) {
                if (echar[e] != minc) continue;
                int u = eto[e];
                if (ans[u] == -1) {       // 每个点只会被赋值一次
                    ans[u] = ans[v] + 1;  // 同层所有点的 ans 相同
                    nact[bn++] = u;
                }
            }
        }
        for (int i = 0; i < bn; i++) act[i] = nact[i];
        an = bn;
    }
    for (int i = 1; i <= n; i++) {
        if (i > 1) printf(" ");
        printf("%d", ans[i]);
    }
    printf("\n");
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) solve();
    return 0;
}
