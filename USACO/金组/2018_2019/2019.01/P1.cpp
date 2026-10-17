#include <cstdio>

using namespace std;

const int MAXN = 5005; // 单词数上限
const int MAXK = 5005; // 行长(音节数)上限
const long long MOD = 1000000007;

int n, m, k;
int syl[MAXN]; // 每个单词的音节数
int cls[MAXN]; // 每个单词所属的韵脚类
int freq[26];  // 韵脚方案里每个大写字母出现的次数

long long dp[MAXK];      // dp[i]: 恰好摆下 i 个音节、且这一行还没有收尾的方案数
long long endCnt[MAXN];  // endCnt[c]: 以韵脚类 c 的词收尾、整行恰好 k 个音节的方案数

// 快速幂: 底数最大 1e9, 指数最多 1e5, 反复平方法 O(log 指数)
long long power(long long base, int exponent) {
    long long res = 1;
    base %= MOD;
    while (exponent > 0) {
        if (exponent & 1) res = res * base % MOD;
        base = base * base % MOD;
        exponent >>= 1;
    }
    return res;
}

int main() {
    if (scanf("%d %d %d", &n, &m, &k) != 3) return 0;
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &syl[i], &cls[i]);
    }
    // 韵脚字母用 char 缓冲读入, 只需要知道每种字母出现多少次
    char buf[4];
    for (int i = 0; i < m; i++) {
        scanf("%s", buf);
        freq[buf[0] - 'A']++;
    }

    // 完全背包: 按音节长度从小到大推, 每次接上一个单词
    // 长度没到 k 就继续往后放词, 正好到 k 就说明这一行以该词的韵脚类收尾
    dp[0] = 1;
    for (int len = 0; len < k; len++) {
        if (dp[len] == 0) continue;
        for (int i = 0; i < n; i++) {
            int nextLen = len + syl[i];
            if (nextLen > k) continue;
            if (nextLen == k) endCnt[cls[i]] = (endCnt[cls[i]] + dp[len]) % MOD;
            else dp[nextLen] = (dp[nextLen] + dp[len]) % MOD;
        }
    }

    // 同一个字母对应的所有行必须用同一个韵脚类收尾: 每个韵脚类单独统计次数次幂,
    // 再把这些选择加起来; 不同字母互不影响, 所以各字母的结果相乘
    long long answer = 1;
    for (int c = 0; c < 26; c++) {
        if (freq[c] == 0) continue;
        long long sum = 0;
        for (int r = 1; r <= n; r++) {
            if (endCnt[r] == 0) continue;
            sum = (sum + power(endCnt[r], freq[c])) % MOD;
        }
        answer = answer * sum % MOD;
    }
    printf("%lld\n", answer);
    return 0;
}
