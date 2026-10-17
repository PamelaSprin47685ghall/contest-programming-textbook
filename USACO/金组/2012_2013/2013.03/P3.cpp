#include <cstdio>
#include <cstring>

using namespace std;

const int MAXN = 10005;   // 项链长度上限
const int MAXM = 1005;    // 对方名字长度上限
const int ALPHA = 26;
const int NEG = -1000000000;

char necklace[MAXN];
char name[MAXM];
int m;
int failState[MAXM];              // failState[q]: 已匹配 q 个字符时, 失配后要退回到的匹配长度
int goTo[MAXM][ALPHA];            // goTo[q][c]: 已匹配 q 个字符后再读入字母 c 得到的匹配长度
int best[MAXM], nextBest[MAXM];   // best[q]: 自动机停在状态 q 时最多能留下多少块石头

int main() {
    scanf("%s", necklace);
    scanf("%s", name);
    int n = strlen(necklace);
    m = strlen(name);
    for (int i = 0; i < n; i++) necklace[i] -= 'a';
    for (int i = 0; i < m; i++) name[i] -= 'a';

    // 先求名字自己的失配函数 (KMP 的前缀函数)
    for (int i = 1; i < m; i++) {
        int j = failState[i - 1];
        while (j > 0 && name[i] != name[j]) j = failState[j - 1];
        if (name[i] == name[j]) j++;
        failState[i] = j;
    }

    // 状态 q 表示"当前保留串的最长后缀恰好是名字的前 q 个字符",
    // 于是只有状态 0..m-1 合法, 走到 m 就意味着名字作为子串出现了
    for (int c = 0; c < ALPHA; c++) goTo[0][c] = (name[0] == c) ? 1 : 0;
    for (int q = 1; q < m; q++) {
        for (int c = 0; c < ALPHA; c++) {
            if (name[q] == c) goTo[q][c] = q + 1;            // 直接接上去
            else goTo[q][c] = goTo[failState[q - 1]][c];     // 退到失配位置再试
        }
    }

    for (int q = 0; q < m; q++) best[q] = NEG;
    best[0] = 0;
    for (int i = 0; i < n; i++) {
        for (int q = 0; q < m; q++) nextBest[q] = best[q];   // 丢掉第 i 块石头, 状态不变
        for (int q = 0; q < m; q++) {
            if (best[q] == NEG) continue;
            int nq = goTo[q][(unsigned char)necklace[i]];
            if (nq < m && best[q] + 1 > nextBest[nq]) nextBest[nq] = best[q] + 1;  // 留下这块石头
        }
        for (int q = 0; q < m; q++) best[q] = nextBest[q];
    }

    int keep = 0;
    for (int q = 0; q < m; q++)
        if (best[q] > keep) keep = best[q];
    printf("%d\n", n - keep);
    return 0;
}
