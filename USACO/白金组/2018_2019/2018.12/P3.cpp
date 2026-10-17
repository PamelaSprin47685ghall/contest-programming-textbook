#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 100000; // 奶牛数上限

int n, m;
vector<int> friends[MAXN];        // 朋友关系：N 个点、N-1 条边构成的树
vector<int> mustGoAfterMe[MAXN];  // mustGoAfterMe[a]：必须排在 a 之后离开的奶牛（出边约束）
int aliveFriends[MAXN];           // 当前还在场的朋友数量
int pendingBefore[MAXN];          // 还有几头"必须排在我之前"的奶牛没走
bool canLeaveNow[MAXN];           // 已经被判定为"现在可以安全离开"
bool canBeLast[MAXN];             // 最终答案：能否成为最后一个离开的奶牛
int leaveOrder[MAXN];             // 按"可以安全离开"的先后顺序记录奶牛
int stack_[MAXN];                 // 显式栈，避免递归深度过大

int main() {
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n - 1; ++i) {
        int x, y;
        scanf("%d%d", &x, &y);
        --x;
        --y;
        friends[x].push_back(y);
        friends[y].push_back(x);
        ++aliveFriends[x];
        ++aliveFriends[y];
    }
    for (int i = 0; i < m; ++i) {
        int a, b;
        scanf("%d%d", &a, &b);
        --a;
        --b;
        mustGoAfterMe[a].push_back(b); // b 必须排在 a 之后离开
        ++pendingBefore[b];
    }

    // 贪心构造一个合法离开顺序：每次送走一头"朋友数不超过 1 且没有'必须在别人之后'约束"的奶牛。
    // 若中途再也找不到这样的奶牛却还有两头以上，则无解：剩下的每头牛要离开都得是某棵子树的叶子，
    // 而它又被别人压在后面，第一个走的那个叶子必然违反约束。
    int head = 0, tail = 0;
    for (int v = 0; v < n; ++v) {
        if (aliveFriends[v] <= 1 && pendingBefore[v] == 0) {
            canLeaveNow[v] = true;
            leaveOrder[tail++] = v;
        }
    }
    bool feasible = true;
    for (int removed = 0; removed < n - 1; ++removed) {
        if (head >= tail) {
            feasible = false;
            break;
        }
        int cur = leaveOrder[head++];
        // cur 走了，还在场的朋友们少一个朋友
        for (int t = 0; t < (int)friends[cur].size(); ++t) {
            int y = friends[cur][t];
            if (canLeaveNow[y]) continue; // 已经判定可以离开的奶牛不必再更新
            if (--aliveFriends[y] <= 1 && pendingBefore[y] == 0) {
                canLeaveNow[y] = true;
                leaveOrder[tail++] = y;
            }
        }
        // cur 走了，"排在 cur 之后"的那些约束各解除一条
        for (int t = 0; t < (int)mustGoAfterMe[cur].size(); ++t) {
            int y = mustGoAfterMe[cur][t];
            if (canLeaveNow[y]) continue;
            if (--pendingBefore[y] == 0 && aliveFriends[y] <= 1) {
                canLeaveNow[y] = true;
                leaveOrder[tail++] = y;
            }
        }
    }
    if (!feasible) {
        for (int v = 0; v < n; ++v) {
            printf("0\n");
        }
        return 0;
    }

    // 贪心剩下的最后一头奶牛 r 本身就是一种合法的最后一名。
    // 一头带着"必须排在别人之前"约束的奶牛不可能当最后一个（她后面还得留人），
    // 从 r 出发、只经过没有这类出边约束的奶牛能到达的连通块，恰好就是所有可能的最后一名：
    //  - 必要性：路径上若有奶牛 a 带约束 (a, b)，a 把树分成两半，r 与待判定的奶牛 s 分居两侧。
    //    若 b 不在 r 那侧，则 a 落在 r 到 b 的路径上，a 离开时既有通向 r 的还在场的朋友、
    //    又有通向 b 的还在场的朋友，不可能是叶子，与 r 能当最后一名矛盾；
    //    于是 b 在 r 那侧，也就不在 s 那侧，对 s 同理可得 s 不能当最后一个。
    //  - 充分性：在一条以 r 结尾的合法顺序里，把 s 与紧随其后的奶牛 x 交换位置不会破坏约束——
    //    s 没有出边约束；而 (x 必须排在 s 之前) 也不可能存在，否则 x 早已排在 s 前面。
    //    逐次交换即可把 s 挪到最后，于是 s 也能当最后一个。
    int root = leaveOrder[n - 1];
    int top = 0;
    if (mustGoAfterMe[root].empty()) {
        canBeLast[root] = true;
        stack_[top++] = root;
        while (top > 0) {
            int cur = stack_[--top];
            for (int t = 0; t < (int)friends[cur].size(); ++t) {
                int y = friends[cur][t];
                if (canBeLast[y] || !mustGoAfterMe[y].empty()) continue;
                canBeLast[y] = true;
                stack_[top++] = y;
            }
        }
    }
    for (int v = 0; v < n; ++v) {
        printf("%d\n", canBeLast[v] ? 1 : 0);
    }
    return 0;
}
