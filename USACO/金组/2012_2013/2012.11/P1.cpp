#include <cstdio>
#include <cstring>

using namespace std;

const int MAXN = 1005;
const int MOD = 2012;

int N;
char S[MAXN];

// 设总高度 h = 已扫描部分里 '(' 比 ')' 多的个数.
// 两个品种各自都是一个括号串, 它们的前缀合法当且仅当各自的"未匹配 '(' 数"始终非负;
// 又因为两个串的字符合起来就是原串, 所以两者未匹配数之和恒等于 h.
// 于是只要用 dp[a] 记录 "Holstein 串还剩 a 个未匹配的 '('" 的方案数, Guernsey 的就是 h - a.
int cur[MAXN], nxt[MAXN];

int main() {
    scanf("%s", S + 1);
    N = (int)strlen(S + 1);

    cur[0] = 1;
    int h = 0;
    for (int i = 1; i <= N; i++) {
        memset(nxt, 0, sizeof(nxt));
        for (int a = 0; a <= h; a++) {
            int ways = cur[a];
            if (ways == 0) continue;
            int b = h - a;  // Guernsey 串当前的未匹配 '(' 数
            if (S[i] == '(') {
                // 这一头是 '(': 分给哪个品种都只会让该品种的未匹配数 +1, 前缀合法性不会被破坏
                nxt[a + 1] = (nxt[a + 1] + ways) % MOD;  // 给 Holstein
                nxt[a] = (nxt[a] + ways) % MOD;          // 给 Guernsey
            } else {
                // 这一头是 ')': 只有对应品种还有未匹配的 '(' 时才能分给它 (否则该品种前缀为负)
                if (a > 0) nxt[a - 1] = (nxt[a - 1] + ways) % MOD;  // 给 Holstein
                if (b > 0) nxt[a] = (nxt[a] + ways) % MOD;          // 给 Guernsey
            }
        }
        if (S[i] == '(') h++;
        else h--;
        memcpy(cur, nxt, sizeof(cur));
    }

    // 只有整个串的括号数相等 (h == 0) 且 Holstein 无剩余未匹配 '(' 时两个品种才都平衡
    if (h == 0) printf("%d\n", cur[0]);
    else printf("0\n");
    return 0;
}
