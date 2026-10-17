#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXQ = 100000;
const int MAXLOG = 17; // 2^17 > 100000，一次最多向上跳 2^17-1 步，足够从任意牛棚走到树根

int queryIsBuild[MAXQ]; // 指令类型：true 表示建棚，false 表示询问
int queryValue[MAXQ];

int barnParent[MAXQ + 1];     // 新棚连到的那座旧棚；独立建棚用 0 号虚拟点当父亲
int barnDepth[MAXQ + 1];      // 在最终森林里到所在树根的边数
int jumpUp[MAXQ + 1][MAXLOG]; // 倍增表：向上跳 2^j 步到达哪座棚
int treeId[MAXQ + 1];         // 每座棚所在的那棵树，用树里最早建的那座棚代表

int diameterEndU[MAXQ + 1];   // 每棵树"当前已建部分"的直径两端
int diameterEndV[MAXQ + 1];
int diameterLength[MAXQ + 1]; // 当前直径的长度

// 求 a、b 的最近公共祖先
int lowestCommonAncestor(int a, int b) {
    if (barnDepth[a] < barnDepth[b]) swap(a, b); // 让 a 更深
    for (int j = MAXLOG - 1; j >= 0; --j) {
        if (barnDepth[a] - (1 << j) >= barnDepth[b]) a = jumpUp[a][j];
    }
    if (a == b) return a;
    // 只要跳 2^j 步后两人还不重合，就一起跳；跳完停在 LCA 的两个不同儿子上
    for (int j = MAXLOG - 1; j >= 0; --j) {
        if (jumpUp[a][j] != jumpUp[b][j]) {
            a = jumpUp[a][j];
            b = jumpUp[b][j];
        }
    }
    return barnParent[a];
}

int treeDistance(int a, int b) {
    return barnDepth[a] + barnDepth[b] - 2 * barnDepth[lowestCommonAncestor(a, b)];
}

int main() {
    int qCount;
    scanf("%d", &qCount);
    int barnCount = 0;
    for (int i = 0; i < qCount; ++i) {
        char type = '?';
        int value = 0;
        scanf(" %c%d", &type, &value); // 格式里的空格用来跳掉上一行末尾的换行
        queryIsBuild[i] = (type == 'B');
        queryValue[i] = value;
        if (type == 'B') {
            ++barnCount;
            barnParent[barnCount] = (value == -1) ? 0 : value; // 独立建棚时父亲是虚拟点 0
        }
    }

    // 新棚连到的旧棚一定编号更小，所以按编号从小到大就能一次算出深度与所在树
    for (int v = 1; v <= barnCount; ++v) {
        if (barnParent[v] == 0) {
            barnDepth[v] = 0;
            treeId[v] = v;
        } else {
            barnDepth[v] = barnDepth[barnParent[v]] + 1;
            treeId[v] = treeId[barnParent[v]];
        }
    }

    for (int v = 1; v <= barnCount; ++v) jumpUp[v][0] = barnParent[v];
    // 向上跳 2^j 步 = 先跳 2^(j-1) 步，再跳 2^(j-1) 步；虚拟点 0 始终自动跳回自己
    for (int j = 1; j < MAXLOG; ++j) {
        for (int v = 1; v <= barnCount; ++v) {
            jumpUp[v][j] = jumpUp[jumpUp[v][j - 1]][j - 1];
        }
    }

    int built = 0;
    for (int i = 0; i < qCount; ++i) {
        if (queryIsBuild[i]) {
            ++built;
            int v = built;
            if (barnParent[v] == 0) {
                // 孤零零一座棚，到最远棚的距离是 0
                diameterEndU[v] = v;
                diameterEndV[v] = v;
                diameterLength[v] = 0;
            } else {
                int root = treeId[v];
                int du = treeDistance(v, diameterEndU[root]);
                int dv = treeDistance(v, diameterEndV[root]);
                // 往树里接一片新叶子后，离这片叶子最远的点必是原直径的某一端，
                // 所以新直径只可能是"原直径"或"叶子连到某一端"三种之一
                if (du >= dv && du > diameterLength[root]) {
                    diameterEndV[root] = v;
                    diameterLength[root] = du;
                } else if (dv > du && dv > diameterLength[root]) {
                    diameterEndU[root] = v;
                    diameterLength[root] = dv;
                }
            }
        } else {
            // 树上离任意一点最远的点一定是当前直径的端点之一
            int v = queryValue[i];
            int root = treeId[v];
            int du = treeDistance(v, diameterEndU[root]);
            int dv = treeDistance(v, diameterEndV[root]);
            printf("%d\n", du > dv ? du : dv);
        }
    }
    return 0;
}
