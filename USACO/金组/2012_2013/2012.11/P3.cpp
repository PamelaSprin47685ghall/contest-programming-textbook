#include <cstdio>

using namespace std;

const int MAXN = 40005;

int N;
int par[MAXN];
int lab[MAXN];  // '(' 记 +1, ')' 记 -1

// 每个节点孩子中"子树内最优/次优值"以及最优值来自哪个孩子,
// 这样就能 O(1) 地求出 "去掉某个孩子后其余孩子中的最大值".
int bestPos1[MAXN], bestPos2[MAXN], bestPosIdx[MAXN];
int bestNeg1[MAXN], bestNeg2[MAXN], bestNegIdx[MAXN];

int dPos[MAXN], dNeg[MAXN];    // 从 v 出发, 只往子树里走能得到的最大和 (允许空路径, 即 0)
int upPos[MAXN], upNeg[MAXN];  // 从 par[v] 出发, 不进入 v 的子树能得到的最大和 (允许空路径, 即 0)

void upd(int *b1, int *b2, int *bi, int u, int v, int val) {
    if (val > b1[u]) {
        b2[u] = b1[u];
        b1[u] = val;
        bi[u] = v;
    } else if (val > b2[u]) {
        b2[u] = val;
    }
}

int main() {
    scanf("%d", &N);
    for (int v = 2; v <= N; v++) scanf("%d", &par[v]);
    for (int v = 1; v <= N; v++) {
        char ch;
        scanf(" %c", &ch);
        lab[v] = (ch == '(') ? 1 : -1;
    }

    // 第一遍: 自底向上求"一直向下走"的最大和 (空路径记 0)
    for (int v = N; v >= 1; v--) {
        dPos[v] = lab[v] + bestPos1[v];
        if (dPos[v] < 0) dPos[v] = 0;
        dNeg[v] = -lab[v] + bestNeg1[v];
        if (dNeg[v] < 0) dNeg[v] = 0;
        if (v > 1) {
            upd(bestPos1, bestPos2, bestPosIdx, par[v], v, dPos[v]);
            upd(bestNeg1, bestNeg2, bestNegIdx, par[v], v, dNeg[v]);
        }
    }

    // 第二遍: 自顶向下求"从父节点出发、绕开本子树"的最大和
    // 父亲编号一定小于孩子, 所以按编号递增处理即可
    int ans = 0;
    for (int v = 1; v <= N; v++) {
        if (v == 1) {
            upPos[v] = 0;
            upNeg[v] = 0;
            continue;
        }
        int p = par[v];
        int sibPos = (bestPosIdx[p] == v) ? bestPos2[p] : bestPos1[p];
        int sibNeg = (bestNegIdx[p] == v) ? bestNeg2[p] : bestNeg1[p];
        int restPos = (upPos[p] > sibPos) ? upPos[p] : sibPos;
        int restNeg = (upNeg[p] > sibNeg) ? upNeg[p] : sibNeg;

        upPos[v] = lab[p] + restPos;
        if (upPos[v] < 0) upPos[v] = 0;
        upNeg[v] = -lab[p] + restNeg;
        if (upNeg[v] < 0) upNeg[v] = 0;

        // 对边 (p, v): 一条平衡路径要么在 v 一侧用 '(' 贡献嵌套深度、在 p 一侧用 ')' 抵消,
        // 要么反过来; 两种取法中能取到的嵌套深度是两侧可提供的"余量"的较小者.
        int c1 = (dPos[v] < upNeg[v]) ? dPos[v] : upNeg[v];
        int c2 = (upPos[v] < dNeg[v]) ? upPos[v] : dNeg[v];
        int c = (c1 > c2) ? c1 : c2;
        if (c > ans) ans = c;
    }

    printf("%d\n", ans);
    return 0;
}
