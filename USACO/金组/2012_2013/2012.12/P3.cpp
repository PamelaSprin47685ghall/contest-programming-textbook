#include <cstdio>

using namespace std;

const int MAXN = 200005;
const int LOG = 18;  // 2^18 = 262144 > 200000, 足够从任意点跳到根

int N;
long long L;
// up[k][v] 是 v 向上跳 2^k 步到达的祖先, 0 表示不存在(已经越过树根)
int up[LOG + 1][MAXN];
long long depth[MAXN];
// delta[v]: 自下而上做子树和时用到的差分标记
long long delta[MAXN];
long long answer[MAXN];

int main() {
    scanf("%d %lld", &N, &L);
    up[0][1] = 0;
    depth[1] = 0;
    for (int i = 2; i <= N; i++) {
        int p;
        long long l;
        scanf("%d %lld", &p, &l);
        up[0][i] = p;  // 题目保证 p_i < i, 所以按编号从前往后就能算出深度
        depth[i] = depth[p] + l;
    }
    for (int k = 1; k <= LOG; k++) {
        for (int i = 1; i <= N; i++) up[k][i] = up[k - 1][up[k - 1][i]];
    }

    // 对每个点 i 求出它能向上走到的最高祖先 top: depth[i] - depth[top] <= L.
    // i 恰好能被"从 top 到 i 这条路径上的所有点"到达(它们是 i 的祖先且深度足够),
    // 于是给这条路径整体加 1: 在 i 处 +1, 在 top 的父节点处 -1,
    // 最后对每个点求子树内 delta 之和即得答案.
    for (int i = 1; i <= N; i++) {
        int cur = i;
        for (int k = LOG; k >= 0; k--) {
            int a = up[k][cur];
            if (a != 0 && depth[i] - depth[a] <= L) cur = a;
        }
        delta[i]++;
        if (cur != 1) delta[up[0][cur]]--;
    }

    for (int i = 1; i <= N; i++) answer[i] = delta[i];
    // 父节点编号一定小于子节点编号, 所以从后往前累加即可完成子树求和
    for (int i = N; i >= 2; i--) answer[up[0][i]] += answer[i];

    for (int i = 1; i <= N; i++) printf("%lld\n", answer[i]);
    return 0;
}
