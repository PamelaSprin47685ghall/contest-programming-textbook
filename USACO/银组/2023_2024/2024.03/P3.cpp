#include <cstdio>

using namespace std;

const int MAXN = 3005;

int n;
char s[MAXN];
int lcp[MAXN][MAXN];       // lcp[i][j]：后缀 i 与后缀 j 的最长公共前缀长度
int deltas[MAXN][MAXN];    // deltas[l][x]：最大合法 K 恰好为 x 的位置个数（只统计 L = l）

// 比较 S[i..i+k) 与 S[j..j+k) 的字典序，返回负数 / 0 / 正数
int compareSub(int i, int j, int k) {
    int common = lcp[i][j];
    if (common >= k) return 0;          // 前 k 个字符完全一样
    if (i + common == n) return -1;     // 后缀 i 已经到底，它成了对方的前缀
    if (j + common == n) return 1;
    return s[i + common] < s[j + common] ? -1 : 1;
}

int main() {
    scanf("%d", &n);
    scanf("%s", s);

    // 从后往前递推，避免逐个字符比较：相同字符就接上斜后一格的结果
    for (int i = n - 1; i >= 0; --i)
        for (int j = n - 1; j >= 0; --j)
            if (s[i] == s[j]) lcp[i][j] = lcp[i + 1][j + 1] + 1;

    int bEnd[MAXN], aBegin[MAXN];
    for (int p = 0; p < n; ++p) {
        // 右侧第一个「字典序更小」的位置：L 变小时它只会右移，于是用双指针扫一遍
        int bCand = p + 1;
        for (int len = n; len >= 1; --len) {
            while (bCand < n && compareSub(p, bCand, len) <= 0) ++bCand;
            int v = bCand + len;
            bEnd[len] = v < n + 1 ? v : n + 1;   // 越界就当成没有限制
        }
        // 左侧第一个「字典序不大于自己」的位置：L 变大时它只会左移
        int aCand = p - 1;
        for (int len = 1; len <= n; ++len) {
            while (aCand >= 0 && compareSub(p, aCand, len) < 0) --aCand;
            aBegin[len] = aCand + 1;
        }
        // 只要 K 不超过 bEnd - aBegin - 2，就存在一段不含「对手」的窗口让 p 胜出；
        // 另外窗口本身必须装得下，于是 K 还不得超过 n - aBegin - 1，这里已并入 bEnd
        for (int len = 1; p + len <= n; ++len) {
            int maxK = bEnd[len] - aBegin[len] - 1;
            if (maxK > n) maxK = n;
            ++deltas[len][maxK];
        }
    }

    // 对每个 L，|P| 等于「最大合法 K >= K」的位置个数，用后缀和一次算出来
    int answer[MAXN] = {0};
    for (int len = 1; len <= n; ++len) {
        int reaching = 0;
        for (int k = n; k >= len; --k) {
            reaching += deltas[len][k];
            ++answer[reaching];
        }
    }
    for (int v = 1; v <= n; ++v) printf("%d\n", answer[v]);
    return 0;
}
