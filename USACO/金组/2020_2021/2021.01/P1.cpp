#include <cstdio>

using namespace std;

const int MAXN = 20;           // 出现过的不同字母数上限（官方测试保证不超过 20）
const int MAXLEN = 100005;     // 听到的字符串长度上限
const int MAXHALF = 1 << 10;   // 折半后每一半的规模上限
const int INF = 1000000000;

char heard[MAXLEN];            // Farmer Nhoj 听到的字符串
int letterId[26];              // 字母 -> 编号（按首次出现顺序），-1 表示没出现过
int adj[MAXN][MAXN];           // adj[i][j]：串中相邻两字符恰好是 (字母 i, 字母 j) 的次数
int lowSum[MAXHALF][MAXN];     // lowSum[s][j]：低半部分取子集 s 时，sum(k in s) adj[j][k]
int highSum[MAXHALF][MAXN];    // highSum[s][j]：高半部分取子集 s 时的同一个和
int dp[1 << MAXN];             // dp[mask]：mask 中字母构成 cowphabet 前缀时的最少清唱次数

// 返回 s 中最低位的编号（保证 s != 0）
int lowestBit(int s) {
    int k = 0;
    while ((s & (1 << k)) == 0) ++k;
    return k;
}

int main() {
    scanf("%s", heard);

    for (int i = 0; i < 26; ++i) letterId[i] = -1;
    int n = 0, len = 0;
    while (heard[len] != '\0') {
        int c = heard[len] - 'a';
        // 没出现过的字母不可能贡献任何比较，直接不编号，把规模压到实际出现数
        if (letterId[c] == -1) letterId[c] = n++;
        ++len;
    }
    for (int i = 1; i < len; ++i)
        ++adj[letterId[heard[i - 1] - 'a']][letterId[heard[i] - 'a']];

    // 折半预处理子集和：把 mask 拆成低 lowBits 位与高 highBits 位，
    // 这样就不需要 N * 2^N 的二维表，查询任意 (字母 j, 子集 mask) 的和只需两次查表
    int lowBits = n / 2, highBits = n - lowBits;
    for (int s = 1; s < (1 << lowBits); ++s) {
        int k = lowestBit(s);
        for (int j = 0; j < n; ++j) lowSum[s][j] = lowSum[s ^ (1 << k)][j] + adj[j][k];
    }
    for (int s = 1; s < (1 << highBits); ++s) {
        int k = lowestBit(s);
        for (int j = 0; j < n; ++j) highSum[s][j] = highSum[s ^ (1 << k)][j] + adj[j][lowBits + k];
    }

    // dp[mask]：只用 mask 中字母（作为 cowphabet 的一段前缀）时的最少清唱次数。
    // 枚举最后一个字母 j：它排在 mask 中所有字母之后，故与 mask 内每个字母构成的
    // "前面字母 >= 后面字母" 的相邻对都要多算一次清唱。
    int full = (1 << n) - 1;
    dp[0] = 1;                 // 清唱次数 = 1 + 断点个数，起始的那个 1 是基础
    for (int mask = 1; mask <= full; ++mask) {
        int lowPart = mask & ((1 << lowBits) - 1);
        int highPart = mask >> lowBits;
        int best = INF;
        for (int j = 0; j < n; ++j) {
            if ((mask & (1 << j)) == 0) continue;
            int cand = dp[mask ^ (1 << j)] + lowSum[lowPart][j] + highSum[highPart][j];
            if (cand < best) best = cand;
        }
        dp[mask] = best;
    }
    printf("%d\n", dp[full]);
    return 0;
}
