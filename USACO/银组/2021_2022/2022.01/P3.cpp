#include <cstdio>

using namespace std;

const int MAXN = 100005;  // 牛(边)的数量上限
const int MAXM = 100005;  // 麦片(点)的数量上限

int N, M;

// 把每头牛看成连接"最爱麦片"和"第二爱麦片"的一条无向边, 麦片看成点.
// 邻接表用 head + tailEdge 尾插, 使同一块麦片上的边保持读入顺序.
int head[MAXM];
int tailEdge[MAXM];
int to[2 * MAXN];
int cowOf[2 * MAXN];
int nextEdge[2 * MAXN];
int edgeCount;

int favorite[MAXN];    // 每头牛最爱的麦片
bool seenCycle[MAXM];  // 找环那一遍的访问标记
int cycleCow;          // 当前分量里找到的非树边对应的牛, 0 表示这个分量是一棵树

bool visited[MAXM];  // 定顺序那一遍的访问标记
bool fed[MAXN];      // 该牛是否被安排吃到麦片
int order[MAXN];     // 输出顺序
int orderLen;

int stackVertex[MAXM];
int stackEdge[MAXM];
int stackPrev[MAXM];

void AddEdge(int a, int b, int cow) {  // 在麦片 a 的邻接表末尾加一条指向 b 的边
    to[edgeCount] = b;
    cowOf[edgeCount] = cow;
    nextEdge[edgeCount] = -1;
    if (head[a] == -1) head[a] = edgeCount;
    else nextEdge[tailEdge[a]] = edgeCount;
    tailEdge[a] = edgeCount;
    ++edgeCount;
}

// 第一遍 DFS: 建立生成树, 并记下第一条"非树边"(环上多出来的那条边).
// 用显式栈模拟递归, 保证访问顺序与递归写法完全一致(栈顶的那条边检查完才轮到下一条).
void FindCycle(int start) {
    int top = 0;
    seenCycle[start] = true;
    stackVertex[top] = start;
    stackPrev[top] = -1;
    stackEdge[top] = head[start];
    ++top;
    while (top > 0) {
        int e = stackEdge[top - 1];
        if (e == -1) {
            --top;
            continue;
        }
        stackEdge[top - 1] = nextEdge[e];  // 这条边已检查, 指针后移
        int v = to[e];
        if (seenCycle[v]) {
            // 走到已访问的点: 只要不是"退回父亲"的那条树边, 它就是不构成生成树的多余边.
            if (cycleCow == 0 && v != stackPrev[top - 1]) cycleCow = cowOf[e];
        } else {
            seenCycle[v] = true;
            stackVertex[top] = v;
            stackPrev[top] = stackVertex[top - 1];  // 父亲就是现在栈顶这个点
            stackEdge[top] = head[v];
            ++top;
        }
    }
}

// 第二遍 DFS: 按"发现顺序"让每条树边对应的牛吃它新发现的那块麦片.
// 父边一定先于子边出手, 所以轮到它时父亲那块麦片已被占用, 而子端点还空着.
// ignoreCow 是环上先出手、已经吃过麦片的那头牛, 不再参与.
void BuildOrder(int start, int ignoreCow) {
    int top = 0;
    visited[start] = true;
    stackEdge[top++] = head[start];
    while (top > 0) {
        int e = stackEdge[top - 1];
        if (e == -1) {
            --top;
            continue;
        }
        stackEdge[top - 1] = nextEdge[e];
        if (cowOf[e] == ignoreCow) continue;  // 两个方向都要跳过
        int v = to[e];
        if (!visited[v]) {
            visited[v] = true;
            fed[cowOf[e]] = true;
            order[orderLen++] = cowOf[e];
            stackEdge[top++] = head[v];
        }
    }
}

int main() {
    scanf("%d%d", &N, &M);
    for (int c = 1; c <= M; ++c) {
        head[c] = -1;
        tailEdge[c] = -1;
    }
    for (int i = 1; i <= N; ++i) {
        int f, s;
        scanf("%d%d", &f, &s);
        favorite[i] = f;
        AddEdge(f, s, i);
        AddEdge(s, f, i);
    }
    // 每个连通分量独立处理: 分量有 V 块麦片、E 头牛时, 最多只有 min(E, V) 头能吃到,
    // 下面构造出的方案正好吃掉 V 块麦片中能吃的那些, 于是恰好饿 E - V 头(若 E > V).
    for (int r = 1; r <= M; ++r) {
        if (seenCycle[r]) continue;  // 该分量已经处理过
        cycleCow = 0;
        FindCycle(r);
        if (cycleCow == 0) {
            // 是一棵树: 每一头牛都能吃到麦片, 任选起点按发现顺序输出即可.
            BuildOrder(r, 0);
        } else {
            // 有环: 让环上这头牛第一个出手, 吃自己最爱的麦片;
            // 去掉它以后剩下的图仍然连通(它是非树边), 再从头遍历一遍生成树.
            fed[cycleCow] = true;
            order[orderLen++] = cycleCow;
            BuildOrder(favorite[cycleCow], cycleCow);
        }
    }
    // 分量的麦片已被吃光, 剩下没安排的牛无论放哪儿都会饿, 全部塞到末尾.
    int hungry = 0;
    for (int i = 1; i <= N; ++i) {
        if (!fed[i]) {
            ++hungry;
            order[orderLen++] = i;
        }
    }
    printf("%d\n", hungry);
    for (int k = 0; k < orderLen; ++k) printf("%d\n", order[k]);
    return 0;
}
