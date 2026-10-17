#include <cstdio>

using namespace std;

const int MAXN = 100005;
const int NEG_INF = -2000000000;

int n;
int parent[MAXN];
int boundL[MAXN], boundR[MAXN];
int lowR[MAXN];          // 根到 i 的路径上 r 的最小值
int highL[MAXN];         // 根到 i 的路径上 l 的最大值
int subMaxL[MAXN];       // i 的子树（含 i）里 l 的最大值
int ancestorLow[MAXN];   // i 的祖先（不含 i）里 (取值 - 答案) 的最大值
int value[MAXN];

int main() {
    int T, B;
    scanf("%d %d", &T, &B);
    while (T--) {
        scanf("%d", &n);
        for (int i = 2; i <= n; ++i) {
            scanf("%d", &parent[i]);
        }
        for (int i = 1; i <= n; ++i) {
            scanf("%d %d", &boundL[i], &boundR[i]);
        }

        // 祖先关系的 (i, j) 对只出现在同一条根到叶的路径上：答案不小于
        // max(l_i - r_j)；另外任意两点的路径都经过根，所以答案还不小于
        // ceil((maxL - minR) / 2)。这两个下界都能取到（构造见下），故就是答案。
        // parent[i] < i，按编号从小到大扫描就能保证父节点已经算好。
        int minR = boundR[1], maxL = boundL[1];
        int answer = 0;
        for (int i = 1; i <= n; ++i) {
            if (boundR[i] < minR) {
                minR = boundR[i];
            }
            if (boundL[i] > maxL) {
                maxL = boundL[i];
            }
            if (i == 1) {
                lowR[i] = boundR[i];
                highL[i] = boundL[i];
            } else {
                lowR[i] = boundR[i] < lowR[parent[i]] ? boundR[i] : lowR[parent[i]];
                highL[i] = boundL[i] > highL[parent[i]] ? boundL[i] : highL[parent[i]];
            }
            // highL[i] - lowR[i] 恰好是这条路径上 max(l) - min(r)
            int span = highL[i] - lowR[i];
            if (span > answer) {
                answer = span;
            }
        }
        int half = (maxL - minR + 1) / 2;   // 上取整 (maxL - minR) / 2
        if (half > answer) {
            answer = half;
        }
        printf("%d\n", answer);

        if (B == 1) {
            // 构造：让每个节点取满足所有下界的最小值。
            // 后代 d 要求 w_i >= l_d - A，所以先自底向上求出每棵子树的 max(l)；
            // 祖先 a 要求 w_i >= w_a - A，所以再自顶向下传递 (祖先取值 - A) 的最大值。
            for (int i = 1; i <= n; ++i) {
                subMaxL[i] = boundL[i];
            }
            for (int i = n; i >= 2; --i) {
                int p = parent[i];
                if (subMaxL[i] > subMaxL[p]) {
                    subMaxL[p] = subMaxL[i];
                }
            }
            for (int i = 1; i <= n; ++i) {
                if (i == 1) {
                    ancestorLow[i] = NEG_INF;
                } else {
                    int fromParent = value[parent[i]] - answer;
                    ancestorLow[i] = fromParent > ancestorLow[parent[i]] ? fromParent : ancestorLow[parent[i]];
                }
                int v = boundL[i];
                if (subMaxL[i] - answer > v) {
                    v = subMaxL[i] - answer;
                }
                if (ancestorLow[i] > v) {
                    v = ancestorLow[i];
                }
                value[i] = v;
            }
            for (int i = 1; i <= n; ++i) {
                if (i > 1) {
                    printf(" ");
                }
                printf("%d", value[i]);
            }
            printf("\n");
        }
    }
    return 0;
}
