#include <cstdio>
#include <cstring>

using namespace std;

const int MAXN = 200005;

// 记子串里 C, O, W 的个数为 c, o, w. 答案是 Y 当且仅当
//   o + w 为偶数 且 c + o 为奇数.
// 为什么这两个奇偶性是关键:
//   操作 1 删掉两个相同字母, 三个计数里有一个减 2, 上述两个和都不变;
//   操作 2 把一个字母换成另外两个, 例如 C -> OW 使 o + w 增加 2, c + o 不变;
//   O -> CW 使 o + w 不变, c + o 不变; W -> CO 同理. 因此它们在任意操作下
//   的奇偶性都不变.
//   目标串 "C" 满足 o + w = 0(偶), c + o = 1(奇), 所以两个条件都是必要的.
//   充分性: 任意相邻两字符总能合并成至多一个字符 —— 相同则直接删掉, 不同
//   (比如 x, y) 则把 x 换成"第三个字母 z 加上 y", 再删掉相邻的 yy, 剩下 z.
//   于是任何串都能缩到长度至多 1; 由不变量知, 满足上述奇偶性的串只能缩成
//   一个 "C"(空串的 c + o 是偶数, 不满足). 所以条件也是充分的.

char s[MAXN];
int preC[MAXN], preO[MAXN], preW[MAXN];  // 三个字母各自的前缀计数

int main() {
    scanf("%s", s);
    int n = strlen(s);
    for (int i = 1; i <= n; ++i) {
        preC[i] = preC[i - 1] + (s[i - 1] == 'C');
        preO[i] = preO[i - 1] + (s[i - 1] == 'O');
        preW[i] = preW[i - 1] + (s[i - 1] == 'W');
    }
    int Q;
    scanf("%d", &Q);
    static char out[MAXN];  // 询问数最多 2e5, 一次性拼成一行输出
    for (int q = 0; q < Q; ++q) {
        int l, r;
        scanf("%d%d", &l, &r);
        int c = preC[r] - preC[l - 1];
        int o = preO[r] - preO[l - 1];
        int w = preW[r] - preW[l - 1];
        out[q] = ((o + w) % 2 == 0 && (c + o) % 2 == 1) ? 'Y' : 'N';
    }
    out[Q] = '\0';
    printf("%s\n", out);
    return 0;
}
