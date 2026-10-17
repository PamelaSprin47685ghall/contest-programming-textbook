#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 1005;

int n, m;
int parentOld[MAXN], parentNew[MAXN];
vector<int> childrenOld[MAXN], childrenNew[MAXN];
char inFinal[MAXN];           // 该节点是否还出现在最终树里
char canMerge[MAXN][MAXN];    // canMerge[a][b]：节点 a 能否并入最终树的节点 b
int representative[MAXN];     // 每个节点最终并入的那个最终树节点
vector<int> byDepth[MAXN];    // 初始树的节点按深度分组
int maxDepth;

void collectByDepth(int v, int d) {
    if (d > maxDepth) maxDepth = d;
    byDepth[d].push_back(v);
    for (int c : childrenOld[v]) {
        collectByDepth(c, d + 1);
    }
}

void solve() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        parentOld[i] = 0;
        parentNew[i] = 0;
        childrenOld[i].clear();
        childrenNew[i].clear();
        inFinal[i] = 0;
        representative[i] = 0;
        byDepth[i - 1].clear();
        for (int j = 1; j <= n; j++) {
            canMerge[i][j] = 0;
        }
    }

    for (int i = 1; i < n; i++) {
        int v, p;
        scanf("%d%d", &v, &p);
        parentOld[v] = p;
        childrenOld[p].push_back(v);
    }
    int root = 1;
    for (int i = 1; i <= n; i++) {
        if (parentOld[i] == 0) root = i;
    }
    maxDepth = 0;
    collectByDepth(root, 0);

    scanf("%d", &m);
    for (int i = 1; i < m; i++) {
        int v, p;
        scanf("%d%d", &v, &p);
        parentNew[v] = p;
        childrenNew[p].push_back(v);
        inFinal[v] = 1;
    }
    // 初始树的根没有父亲，也就没有兄弟跟它合并，所以它必然留在最终树里
    inFinal[root] = 1;

    // 按深度从大到小计算：要先知道子节点能并到哪里，才能判断父节点能并到哪里
    for (int d = maxDepth; d >= 1; d--) {
        for (int a : byDepth[d]) {
            if (inFinal[a]) {
                // 自己必须留在最终树里，不能并进别人
                canMerge[a][a] = 1;
                continue;
            }
            // 合并后的节点取两者的较大值，所以想并入 b 就必须有 a 的值不超过 b 的值
            for (int b = a + 1; b <= n; b++) {
                if (!inFinal[b]) continue;
                // a 的每个子节点都必须能并进 b 在最终树里的某个子节点
                bool ok = true;
                for (int c : childrenOld[a]) {
                    bool found = false;
                    for (int c2 : childrenNew[b]) {
                        if (canMerge[c][c2]) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        ok = false;
                        break;
                    }
                }
                canMerge[a][b] = ok;
            }
        }
    }

    // 按深度从小到大决定去向：父节点先定下来，子节点才知道该并进谁的孩子
    int opA[MAXN], opB[MAXN], opCount = 0;
    representative[root] = root;
    for (int d = 1; d <= maxDepth; d++) {
        for (int a : byDepth[d]) {
            int p = representative[parentOld[a]];
            for (int b : childrenNew[p]) {
                if (canMerge[a][b]) {
                    representative[a] = b;
                    break;
                }
            }
            if (representative[a] != a) {
                // 此刻 a 和 representative[a] 的父亲都已经变成 p，两者是兄弟，可以合并
                opA[opCount] = a;
                opB[opCount] = representative[a];
                opCount++;
            }
        }
    }

    printf("%d\n", opCount);
    for (int i = 0; i < opCount; i++) {
        printf("%d %d\n", opA[i], opB[i]);
    }
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        solve();
    }
    return 0;
}
