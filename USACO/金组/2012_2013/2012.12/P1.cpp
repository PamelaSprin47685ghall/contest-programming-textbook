#include <cstdio>
#include <cstdlib>

using namespace std;

const int MAXN = 105;

int N, M;
// G[i] 表示帮派 i 还没进场的牛数
int G[MAXN];

// 一头帮派为 g 的牛走进场地: 空场时它控制场地, 否则同帮派累积, 异帮派两牛同归于尽
void updateState(int &cg, int &cc, int g) {
    if (cc == 0) {
        cg = g;
        cc = 1;
    } else if (cg == g) {
        cc++;
    } else {
        cc--;
    }
}

// 从当前场地状态 (cg 控制场地且 cc 头牛在场上) 出发, 返回最终帮派 1 能留在场上的最大牛数
// 思路: 为了让帮派 1 最后剩得最多, 应该先让其它帮派互相消耗, 使场上剩下的异帮派牛尽量少
// (等价于让它们"自我抵消"); 每次挑选剩余牛数最多的异帮派, 正是让剩下的牛最少的最优贪心.
// 异帮派全部放完后, 再把帮派 1 的牛全部放上, 剩下的就是答案.
int maxCows(int cg, int cc, const int *g) {
    int c[MAXN];
    for (int i = 2; i <= M; i++) c[i] = g[i];
    while (true) {
        int bi = 0;
        for (int i = 2; i <= M; i++) {
            if (c[i] > 0 && (bi == 0 || c[i] > c[bi])) bi = i;
        }
        if (bi == 0) break;  // 其它帮派都放完了
        c[bi]--;
        updateState(cg, cc, bi);
    }
    for (int i = 0; i < g[1]; i++) updateState(cg, cc, 1);
    return (cg == 1 && cc > 0) ? cc : 0;
}

int main() {
    scanf("%d %d", &N, &M);
    for (int i = 1; i <= M; i++) scanf("%d", &G[i]);

    int res = maxCows(0, 0, G);
    if (res == 0) {
        printf("NO\n");
        return 0;
    }
    printf("YES\n");
    printf("%d\n", res);

    // 逐头确定: 为了让字典序最小, 每次尝试当前能放的最小帮派编号,
    // 只要放它之后"最多还能留下 res 头帮派 1 的牛"这一事实不变, 放它就是安全的.
    int cg = 0, cc = 0;
    for (int step = 0; step < N; step++) {
        for (int j = 1; j <= M; j++) {
            if (G[j] == 0) continue;
            int ncg = cg, ncc = cc;
            updateState(ncg, ncc, j);
            G[j]--;
            if (maxCows(ncg, ncc, G) == res) {
                printf("%d\n", j);
                cg = ncg;
                cc = ncc;
                break;
            }
            G[j]++;  // 放 j 会破坏最优值, 撤销后试下一个帮派
        }
    }
    return 0;
}
