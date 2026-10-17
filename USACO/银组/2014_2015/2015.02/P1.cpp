#include <cstdio>
#include <cstring>

using namespace std;

const int MAXN = 1000005;

char S[MAXN];        // 原文
char T[MAXN];        // 需要删掉的敏感词
char res[MAXN];      // 结果串, 当成栈来用
int state[MAXN];     // state[i]: 结果串前 i 个字符当前匹配到 T 的最长前缀长度
int fail[MAXN];      // KMP 失配指针

int main() {
    scanf("%s", S);
    scanf("%s", T);
    int ns = (int)strlen(S);
    int nt = (int)strlen(T);

    // 构造 T 的失配函数: fail[i] 表示 T 前 i 个字符的最长真前缀且同时是后缀的长度.
    // 有了它, 失配时可以一次跳回上一个还可能匹配成功的位置.
    fail[0] = 0;
    fail[1] = 0;
    for (int i = 1; i < nt; i++) {
        int j = fail[i];
        while (j > 0 && T[i] != T[j]) j = fail[j];
        if (T[i] == T[j]) j++;
        fail[i + 1] = j;
    }

    int len = 0;
    state[0] = 0;
    for (int i = 0; i < ns; i++) {
        // 从结果串当前末尾的匹配状态继续, 所以每个字符只需要一次匹配判断.
        int j = state[len];
        while (j > 0 && S[i] != T[j]) j = fail[j];
        if (S[i] == T[j]) j++;

        res[len++] = S[i];
        state[len] = j;

        // 末尾刚好凑出完整的 T, 直接从栈顶弹掉这一段.
        // 弹掉之后的新末尾状态保存在 state 里, 就是拼上这段之前的状态,
        // 等价于"删除后继续匹配", 因此不需要重新扫描结果串.
        if (j == nt) len -= nt;
    }
    res[len] = '\0';
    printf("%s\n", res);
    return 0;
}
