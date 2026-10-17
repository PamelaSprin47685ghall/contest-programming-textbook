#include <cstdio>
#include <algorithm>

using namespace std;

// 题意转化：不会封锁等价于"任何两个编号互不为前缀"（若 x 是 y 的前缀，
// 念 y 的奶牛可能停在 x，Bessie 就听到了另一头牛的编号）。补位只能加在末尾，
// 所以我们要花最少的 bit 让补完后的串集合"无前缀关系"。
//
// 把所有串插进一棵二叉 trie（根代表空串，走一位相当于补一个 bit），
// 一个串只能落到它自己结点的后代上。每个结点的状态：
//   0 FREE    ：空闲，某个串可以直接落在这里；
//   1 IN_USE  ：已被某个串占用（必是叶子）；
//   2 BLOCKED ：是某个已占用串的祖先，串落在这里迟早会与后代冲突，只能继续往下找。
const int FREE = 0;
const int IN_USE = 1;
const int BLOCKED = 2;

// 结点数上界：前缀结点 <= Σ|s|，BFS 时给阻塞结点补出的兄弟结点 <= Σ|s|，
// 每次下推新造两个儿子，而下推次数 <= N，所以 2 * Σ|s| + 2N 足够。
const int MAXNODE = 2400005;
const int MAXN = 100000;   // 奶牛数上限
const int MAXLEN = 1000005; // 单个编号长度上限（总长 <= 1e6）

int childOf[MAXNODE][2];    // 两个儿子，0 表示还没有
int depthOf[MAXNODE];       // 该结点代表的串的长度
int needOf[MAXNODE];        // 该结点上还有几个编号没被安置（只有原串终止结点可能非零）
char statusOf[MAXNODE];     // 上面定义的三种状态

int bfsQueue[MAXNODE];      // 第一队列：按深度递增遍历后代，找空闲结点
int bfsHead, bfsTail;
int pushQueue[MAXNODE];     // 第二队列：已占用的叶子（按深度递增），可以往后推
int pushHead, pushTail;

int nodeCount = 1;          // 1 号结点是根，代表空串

int srcNode[MAXN + 1];      // 每个不同编号的终止结点
int srcDepth[MAXN + 1];     // 该结点（也就是原串）的长度
int srcCount = 0;

// 取 node 的第 bit 个儿子，没有就新建一个空闲结点。
// 即使暂时用不到也建出来，是因为以后可能有串落到那里。
int getChild(int node, int bit) {
    if (childOf[node][bit] == 0) {
        ++nodeCount;
        childOf[node][bit] = nodeCount;
        childOf[nodeCount][0] = 0;
        childOf[nodeCount][1] = 0;
        depthOf[nodeCount] = depthOf[node] + 1;
        statusOf[nodeCount] = FREE;
        needOf[nodeCount] = 0;
    }
    return childOf[node][bit];
}

// 长度大的串可选位置更窄，必须优先安置
bool byDepthDesc(int a, int b) {
    return depthOf[a] > depthOf[b];
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    static char buf[MAXLEN]; // 编号可能很长，放静态区避免爆栈
    for (int i = 0; i < n; ++i) {
        scanf("%s", buf);
        int node = 1;
        for (int j = 0; buf[j] != '\0'; ++j) {
            statusOf[node] = BLOCKED; // 有串经过这里，这个位置就永远不能放串了
            node = getChild(node, buf[j] - '0');
        }
        if (needOf[node] == 0) { // 相同编号只登记一次，用计数表示重复出现
            srcNode[srcCount] = node;
            ++srcCount;
        }
        ++needOf[node];
    }

    sort(srcNode, srcNode + srcCount, byDepthDesc);

    long long answer = 0;
    for (int i = 0; i < srcCount; ++i) {
        int source = srcNode[i];
        int len = depthOf[source];
        bfsHead = bfsTail = 0;
        pushHead = pushTail = 0;
        bfsQueue[bfsTail++] = source;
        while (needOf[source] > 0) {
            // 下推一个已占用叶子要花 (深度 - len) + 2：老串补 1 位，
            // 新串补到另一个儿子上要补 (深度 + 1 - len) 位。
            // 所以只有当它比第一队列找到的下一个结点深至少 2 时，下推才更划算。
            if (pushHead < pushTail &&
                (bfsHead == bfsTail || depthOf[pushQueue[pushHead]] + 2 <= depthOf[bfsQueue[bfsHead]])) {
                int node = pushQueue[pushHead++];
                --needOf[source];
                statusOf[node] = BLOCKED; // 老串退到儿子那儿，把原位让给新串
                answer += depthOf[node] - len + 2;
                for (int bit = 0; bit < 2; ++bit) {
                    int c = getChild(node, bit);
                    statusOf[c] = IN_USE; // 两个儿子都被占用，都可能在以后被下推
                    pushQueue[pushTail++] = c;
                }
            } else {
                int node = bfsQueue[bfsHead++];
                if (statusOf[node] == FREE) { // 找到空闲位置，直接落在这里最省
                    --needOf[source];
                    statusOf[node] = IN_USE;
                    answer += depthOf[node] - len;
                }
                if (statusOf[node] == BLOCKED) { // 阻塞结点：还得继续往它的儿子里找
                    for (int bit = 0; bit < 2; ++bit) {
                        bfsQueue[bfsTail++] = getChild(node, bit);
                    }
                } else { // 叶子（刚占用的或本来就占用的）留待下推
                    pushQueue[pushTail++] = node;
                }
            }
        }
    }

    printf("%lld\n", answer);
    return 0;
}
