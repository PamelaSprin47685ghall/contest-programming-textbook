#include <cstdio>
#include <cstring>

using namespace std;

const int MAXS = 300005;  // 字符串总长度上限
const int MAXN = 30005;   // 字符串个数上限

int N;
// 所有字符串按输入顺序拼在一起存放, 第 i 个串从 buf + start[i] 开始, 长度为 slen[i]
char buf[MAXS + MAXN];
int start[MAXN], slen[MAXN];

// 每个串在字典树上的路径: 第 i 个串的路径从 pathNode + pathAt[i] 开始
int pathAt[MAXN], pathNode[MAXS];

// 字典树, 儿子用"首儿子 + 兄弟链表"表示(总边数等于总字符数, 比 26 个儿子的数组省内存)
int head[MAXS], nxt[MAXS], to[MAXS];
char ech[MAXS];
// mask[u] 的第 c 位为 1 表示节点 u 存在字符 c 的儿子
int mask[MAXS];
char isEnd[MAXS];

int main() {
    scanf("%d", &N);
    int pos = 0;
    for (int i = 0; i < N; i++) {
        scanf("%s", buf + pos);
        start[i] = pos;
        slen[i] = (int)strlen(buf + pos);
        pos += slen[i] + 1;  // 每个串后面还占一个结束符的位置
    }

    int nodeCnt = 1, edgeCnt = 0, pathTop = 0;  // 根节点编号为 1
    head[1] = -1;
    mask[1] = 0;
    isEnd[1] = 0;
    for (int i = 0; i < N; i++) {
        int u = 1;
        pathAt[i] = pathTop;
        for (int j = 0; j < slen[i]; j++) {
            pathNode[pathTop++] = u;
            int c = buf[start[i] + j] - 'a';
            mask[u] |= 1 << c;
            int v = 0;
            for (int e = head[u]; e != -1; e = nxt[e]) {
                if (ech[e] == c) {
                    v = to[e];
                    break;
                }
            }
            if (v == 0) {
                v = ++nodeCnt;
                head[v] = -1;
                mask[v] = 0;
                isEnd[v] = 0;
                nxt[edgeCnt] = head[u];
                head[u] = edgeCnt;
                to[edgeCnt] = v;
                ech[edgeCnt] = (char)c;
                edgeCnt++;
            }
            u = v;
        }
        isEnd[u] = 1;
    }

    int cnt = 0;
    int ansIdx[MAXN];
    for (int i = 0; i < N; i++) {
        // 收集 "某个字母必须排在另一个字母前面" 的约束: 沿字典树上该串的路径走,
        // 每到一个节点, 该串的下一个字符必须比该节点所有其它儿子字符都靠前.
        char adj[26][26];
        memset(adj, 0, sizeof(adj));
        bool ok = true;
        for (int j = 0; j < slen[i]; j++) {
            int u = pathNode[pathAt[i] + j];
            if (isEnd[u]) {
                ok = false;  // 已经有别的串是这个串的前缀, 那它永远排不到第一
                break;
            }
            int c = buf[start[i] + j] - 'a';
            for (int d = 0; d < 26; d++) {
                if (d != c && ((mask[u] >> d) & 1)) adj[c][d] = 1;
            }
        }
        if (!ok) continue;

        // 约束图无环 <=> 存在一种字母表顺序同时满足全部约束, 从而该串能排第一 (拓扑排序)
        int deg[26];
        for (int x = 0; x < 26; x++) {
            deg[x] = 0;
            for (int y = 0; y < 26; y++) {
                if (adj[y][x]) deg[x]++;
            }
        }
        char used[26];
        memset(used, 0, sizeof(used));
        int removed = 0;
        for (int t = 0; t < 26; t++) {
            int pick = -1;
            for (int x = 0; x < 26; x++) {
                if (!used[x] && deg[x] == 0) {
                    pick = x;
                    break;
                }
            }
            if (pick < 0) break;
            used[pick] = 1;
            removed++;
            for (int y = 0; y < 26; y++) {
                if (adj[pick][y]) deg[y]--;
            }
        }
        if (removed == 26) ansIdx[cnt++] = i;
    }

    printf("%d\n", cnt);
    for (int i = 0; i < cnt; i++) printf("%s\n", buf + start[ansIdx[i]]);
    return 0;
}
