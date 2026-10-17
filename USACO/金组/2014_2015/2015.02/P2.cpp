#include <cstdio>

using namespace std;

const int MAXN = 100005; // 原文长度、以及所有违禁词长度之和的上界

int child[MAXN][26]; // 字典树; 建好自动机后每个状态对 26 个字母都有确定转移
int fail[MAXN];      // 失配指针
int matchLen[MAXN];  // 以该状态结尾的最长违禁词长度, 0 表示没有
int nodeCnt = 1;     // 0 号是根

char text[MAXN];      // 原文 S
char result[MAXN];    // 逐字符构造的答案 R
int stateAfter[MAXN]; // R 的每个前缀在自动机里对应的状态
int queueBuf[MAXN];   // BFS 队列

void insert(const char *w) {
    int u = 0, len = 0;
    for (int i = 0; w[i]; i++) {
        int c = w[i] - 'a';
        if (child[u][c] == 0) child[u][c] = nodeCnt++;
        u = child[u][c];
        len = i + 1;
    }
    matchLen[u] = len;
}

void buildAutomaton() {
    int head = 0, tail = 0;
    for (int c = 0; c < 26; c++) {
        int v = child[0][c];
        if (v) {
            fail[v] = 0;
            queueBuf[tail++] = v;
        } else {
            child[0][c] = 0; // 根节点缺失的转移都指回根
        }
    }
    while (head < tail) {
        int u = queueBuf[head++];
        // 以 u 结尾的最长违禁词: 自身有就用自身的长度, 否则继承失配状态的答案。
        // 因为 BFS 保证了 fail[u] 先被处理, 一次继承就够了
        if (matchLen[u] == 0) matchLen[u] = matchLen[fail[u]];
        for (int c = 0; c < 26; c++) {
            int v = child[u][c];
            if (v) {
                fail[v] = child[fail[u]][c];
                queueBuf[tail++] = v;
            } else {
                // 补全转移, 让每个状态对每个字母都能 O(1) 走一步
                child[u][c] = child[fail[u]][c];
            }
        }
    }
}

int main() {
    int n;
    scanf("%s", text);
    scanf("%d", &n);
    char word[MAXN];
    for (int i = 0; i < n; i++) {
        scanf("%s", word);
        insert(word);
    }
    buildAutomaton();

    int len = 0;   // 当前答案的长度
    int state = 0; // 当前答案对应的自动机状态
    stateAfter[0] = 0;
    for (int i = 0; text[i]; i++) {
        state = child[state][text[i] - 'a'];
        result[len++] = text[i];
        stateAfter[len] = state;
        // 新出现的违禁词必定以刚加入的这个字符结尾, 其中起点最靠左 (也就是最早
        // 出现) 的那个就是最长的匹配。删掉它之后剩下的部分是删之前答案的前缀,
        // 而删之前的答案已经不含违禁词, 所以每加入一个字符最多删一次
        if (matchLen[state] > 0) {
            len -= matchLen[state];
            state = stateAfter[len]; // 状态一并回退到删除后的位置
        }
    }
    result[len] = '\0';
    printf("%s\n", result);
    return 0;
}
