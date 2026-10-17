#include <cstdio>
#include <set>
#include <vector>

using namespace std;

const int MAXN = 100000;
const int MAXCOLOR = 100000;

int n, q;
vector<int> edges[MAXN + 1];
int parent[MAXN + 1];      // 根 1 的父亲记为 0
int stackNodes[MAXN + 1];  // 手写栈，先序遍历用
int nodeAtPos[MAXN + 1];   // 先序编号 -> 节点
int startPos[MAXN + 1];    // 节点 -> 先序编号
int endPos[MAXN + 1];      // 节点子树里最大的先序编号
int subtreeSize[MAXN + 1];

// 每种颜色的"特殊节点"：自己染了该色、父亲没染该色，也就是该色在这棵子树里的起始点。
// 键取先序编号，同一颜色的特殊节点天然按位置有序，便于找"包住 x 的那个"和"x 下面的那些"。
set<int> specialPos[MAXCOLOR + 1];

long long treeA[MAXN + 2];  // 区间加 + 单点查：某位置被多少个特殊节点的子树区间盖住
long long treeB[MAXN + 2];  // 单点加 + 区间查：特殊节点的子树大小按位置累加

void addToTree(long long* tree, int pos, long long value) {
    for (; pos <= n; pos += pos & (-pos)) tree[pos] += value;
}

long long prefixSum(long long* tree, int pos) {
    long long total = 0;
    for (; pos > 0; pos -= pos & (-pos)) total += tree[pos];
    return total;
}

// 让 / 撤销一个特殊节点：它代表"整棵子树都被染上它那种颜色"。
void markSpecial(int node, long long sign) {
    addToTree(treeA, startPos[node], sign);
    addToTree(treeA, endPos[node] + 1, -sign);
    addToTree(treeB, startPos[node], sign * subtreeSize[node]);
}

int main() {
    scanf("%d%d", &n, &q);
    for (int i = 0; i < n - 1; ++i) {
        int a, b;
        scanf("%d%d", &a, &b);
        edges[a].push_back(b);
        edges[b].push_back(a);
    }

    // 显式栈做先序遍历。树可能退化成一条链，递归深度会到 10^5，容易爆栈。
    int top = 0, timer = 0;
    stackNodes[top++] = 1;
    parent[1] = 0;
    while (top > 0) {
        int cur = stackNodes[--top];
        startPos[cur] = ++timer;
        nodeAtPos[timer] = cur;
        for (int i = 0; i < (int)edges[cur].size(); ++i) {
            int nxt = edges[cur][i];
            if (nxt == parent[cur]) continue;  // 树上只需绕开父亲这一个方向
            parent[nxt] = cur;
            stackNodes[top++] = nxt;
        }
    }

    // 先序序列里子树是连续一段；逆着先序累加即可算出子树大小（儿子总在父亲后面出现）。
    for (int v = 1; v <= n; ++v) subtreeSize[v] = 1;
    for (int pos = n; pos >= 2; --pos) {
        int v = nodeAtPos[pos];
        subtreeSize[parent[v]] += subtreeSize[v];
    }
    for (int v = 1; v <= n; ++v) endPos[v] = startPos[v] + subtreeSize[v] - 1;

    for (int i = 0; i < q; ++i) {
        int type;
        scanf("%d", &type);
        if (type == 1) {
            int x, c;
            scanf("%d%d", &x, &c);
            set<int>& positions = specialPos[c];
            set<int>::iterator it = positions.upper_bound(startPos[x]);
            // 若某个同色特殊节点的子树已经整个包住 x，说明 x 本来就是这个颜色，这次泼洒什么也没变。
            if (it != positions.begin()) {
                set<int>::iterator before = it;
                --before;
                if (endPos[nodeAtPos[*before]] >= endPos[x]) continue;
            }
            // 落在 x 子树里、比 x 更深的同色特殊节点现在被 x 覆盖，不再是"起点"，要撤销。
            while (it != positions.end() && endPos[nodeAtPos[*it]] <= endPos[x]) {
                markSpecial(nodeAtPos[*it], -1);
                it = positions.erase(it);
            }
            positions.insert(startPos[x]);
            markSpecial(x, 1);
        } else {
            int x;
            scanf("%d", &x);
            // 祖先（含自己）里的特殊节点：每一个都意味着 x 这一整棵子树被某种颜色染满
            long long coveredColors = prefixSum(treeA, startPos[x]);
            // 子树内部（不含 x）的特殊节点：它们各自那棵子树是该颜色在 x 子树里的全部着色范围
            long long insidePart = prefixSum(treeB, endPos[x]) - prefixSum(treeB, startPos[x]);
            printf("%lld\n", coveredColors * subtreeSize[x] + insidePart);
        }
    }
    return 0;
}
