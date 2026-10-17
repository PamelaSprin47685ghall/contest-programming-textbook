#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 100000;  // 奶牛数上限
const int INF = MAXN + 5; // 比任何合法夜晚数都大的"无穷大"

int n;                       // 奶牛数
vector<int> edges[MAXN + 1]; // 树的邻接表
bool infected[MAXN + 1];     // 题面给出的最终感染情况

int distToHealthy[MAXN + 1]; // 每头牛到最近未感染牛的距离（未感染牛本身为 0）
int distToSource[MAXN + 1];  // 每头牛到最近"合法起点"（下文集合 S）的距离

int bfsOrder[MAXN + 1]; // 以 1 号牛为根的 BFS 顺序，保证父亲排在儿子前面
int parentOf[MAXN + 1]; // 每头牛的父亲，根的父亲记为 0
int queueBuf[MAXN + 1]; // 多源 BFS 用的队列

bool isHealthy[MAXN + 1]; // 当前这批 BFS 源点标记
bool inSource[MAXN + 1];  // 当前查询下哪些牛属于集合 S

// 子树合并时一个点的状态（对应官解里的 pair）：
// kind = -1：子树里还有尚未被覆盖的感染牛，dist 是最远那头的距离；
// kind =  0：子树里没有任何感染牛；
// kind =  1：子树里已有选中的牛，dist 是到最近一头选中牛的距离。
struct State {
    int kind;
    int dist;
};

State stateOf[MAXN + 1]; // 每个点处理完自己子树后的状态

int nights; // 当前查询的夜晚数 T
int answer; // 当前查询的答案

// 多源 BFS：源点距离为 0，其余点求到最近源点的距离
void multiSourceBfs(const bool *isSource, int *dist) {
    int head = 0, tail = 0;
    for (int v = 1; v <= n; ++v) {
        if (isSource[v]) {
            dist[v] = 0;
            queueBuf[tail++] = v;
        } else {
            dist[v] = INF;
        }
    }
    while (head < tail) {
        int v = queueBuf[head++];
        for (int i = 0; i < (int)edges[v].size(); ++i) {
            int u = edges[v][i];
            if (dist[u] > dist[v] + 1) {
                dist[u] = dist[v] + 1;
                queueBuf[tail++] = u;
            }
        }
    }
}

// 合并两个状态（相当于官解的 comb 函数），先按 (kind, dist) 字典序排好
State combine(State a, State b) {
    if (a.kind > b.kind || (a.kind == b.kind && a.dist > b.dist)) {
        State tmp = a;
        a = b;
        b = tmp;
    }
    if (a.kind == 0) return b;
    if (b.kind == 0) return a;
    if (b.kind == -1) return (a.dist > b.dist) ? a : b; // 两边都有未覆盖的牛：留最远的那头
    if (a.kind == 1) return (a.dist < b.dist) ? a : b;  // 两边都有选中的牛：留最近的那头
    // 一侧有未覆盖的牛、另一侧有选中的牛：两头牛之间的路径经过当前点，
    // 距离为 a.dist + b.dist，不超过 T 时前者就被覆盖了
    if (a.dist + b.dist > nights) return a;
    return b;
}

// 回答一次查询（给定夜晚数 T），返回 -1 表示不可能
int query(int T) {
    nights = T;
    // 集合 S：以这头牛为起点时，T 夜之内被传染的牛全部是已感染的牛
    for (int v = 1; v <= n; ++v) inSource[v] = (distToHealthy[v] > T);
    multiSourceBfs(inSource, distToSource);
    // 只要有感染牛距任何合法起点都超过 T 夜，这组数据就不可能发生
    for (int v = 1; v <= n; ++v) {
        if (infected[v] && distToSource[v] > T) return -1;
    }

    answer = 0;
    // 逆 BFS 序自底向上：儿子先于父亲算完
    for (int idx = n - 1; idx >= 0; --idx) {
        int x = bfsOrder[idx];
        State best;
        best.kind = infected[x] ? -1 : 0;
        best.dist = 0;
        for (int i = 0; i < (int)edges[x].size(); ++i) {
            int y = edges[x][i];
            if (y == parentOf[x]) continue; // 只合并儿子
            State child = stateOf[y];
            if (child.kind != 0) ++child.dist; // 从儿子走到当前点多一条边
            best = combine(best, child);
        }
        if (best.kind == -1) {
            // 最深的未覆盖感染牛要么由祖先方向覆盖，要么必须在此处选起点。
            // 祖先方向的最优选择是父亲附近最近的合法起点，它到未覆盖牛的距离
            // 是 best.dist + 1 + distToSource[parentOf[x]]：超过 T 就够不着。
            bool coverFromOutside = (x != 1) && (best.dist + 1 + distToSource[parentOf[x]] <= T);
            if (!coverFromOutside) {
                // 选距离 x 最近的合法起点，它能覆盖子树内所有未覆盖的牛
                ++answer;
                best.kind = 1;
                best.dist = distToSource[x];
            }
        }
        stateOf[x] = best;
    }
    return answer;
}

int main() {
    scanf("%d", &n);
    char buf[MAXN + 2];
    scanf("%s", buf);
    for (int i = 0; i < n; ++i) infected[i + 1] = (buf[i] == '1');
    for (int i = 0; i < n - 1; ++i) {
        int a, b;
        scanf("%d%d", &a, &b);
        edges[a].push_back(b);
        edges[b].push_back(a);
    }

    // 未感染的牛作源点做多源 BFS：离非感染牛超过 T 的牛才可能当初始感染牛
    for (int v = 1; v <= n; ++v) isHealthy[v] = !infected[v];
    multiSourceBfs(isHealthy, distToHealthy);

    // 以 1 号牛为根 BFS，同时得到父亲和"父先子后"的处理顺序
    int head = 0, tail = 0;
    parentOf[1] = 0;
    bfsOrder[tail++] = 1;
    while (head < tail) {
        int v = bfsOrder[head++];
        for (int i = 0; i < (int)edges[v].size(); ++i) {
            int u = edges[v][i];
            if (u == parentOf[v]) continue; // 树里唯一要绕开的就是父亲
            parentOf[u] = v;
            bfsOrder[tail++] = u;
        }
    }

    int q;
    scanf("%d", &q);
    for (int i = 0; i < q; ++i) {
        int t;
        scanf("%d", &t);
        printf("%d\n", query(t));
    }
    return 0;
}
