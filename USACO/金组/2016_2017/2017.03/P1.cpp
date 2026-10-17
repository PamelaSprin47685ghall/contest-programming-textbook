#include <cstdio>

using namespace std;

const int MAXN = 505;
const int MAXM = 505;

char spotty[MAXN][MAXM];
char plain[MAXN][MAXM];
// match[s * n + p]: 以当前枚举的左端点 i 为起点, 斑点牛 s 与普通牛 p 的基因组
// 从第 i 位起还能连续匹配多少个字符. 窗口只要长过这个数, 这一对牛在窗口内
// 的子串就必然不相等; 反过来, 窗口若不长过它, 这对牛就分不开.
int match[MAXN * MAXN];

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int s = 0; s < n; ++s) {
        scanf("%s", spotty[s]);
    }
    for (int p = 0; p < n; ++p) {
        scanf("%s", plain[p]);
    }

    int best = m;  // 整个基因组一定够用: 没有两头牛基因组完全相同
    // 从右往左枚举窗口左端点 i, 顺便把所有 (斑点牛, 普通牛) 对的匹配长度续上
    for (int i = m - 1; i >= 0; --i) {
        int need = 1;  // 以 i 为左端点时, 能区分斑点牛的最短窗口长度
        for (int s = 0; s < n; ++s) {
            char ch = spotty[s][i];
            int base = s * n;
            for (int p = 0; p < n; ++p) {
                if (ch == plain[p][i]) {
                    match[base + p] += 1;  // 这一位也相同, 匹配长度在上一列基础上延续
                } else {
                    match[base + p] = 0;   // 这一位不同, 从 i 开始的连续匹配到此为止
                }
                if (match[base + p] + 1 > need) {
                    need = match[base + p] + 1;  // 必须比所有配对的匹配长度都长
                }
            }
        }
        if (need <= m - i && need < best) {
            best = need;  // need > m - i 说明以 i 开头的窗口都躲不开重复, 只能换左端点
        }
    }
    printf("%d\n", best);
    return 0;
}
