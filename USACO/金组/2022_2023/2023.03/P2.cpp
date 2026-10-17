#include <cstdio>
#include <cstring>

using namespace std;

const int MAXLEN = 200005;
const int INF = 1000000000;
const char PATTERN[] = "bessie";
const int PLEN = 6;

struct State {
    int count;  // 已经完整拼出的 "bessie" 个数
    int cost;   // 为了拼出这些 "bessie" 而删掉的字符总代价
};

// 先比数量多的，数量一样时比代价小的；count 为 -1 表示这种情况做不到
State better(const State &a, const State &b) {
    if (a.count != b.count) return a.count > b.count ? a : b;
    return a.cost < b.cost ? a : b;
}

int main() {
    char text[MAXLEN];
    scanf("%s", text);
    int len = strlen(text);

    // dp[j]：读完当前前缀以后，末尾恰好是 "bessie" 的前 j 个字符时，
    // 最优的（完整次数，删除代价）。j = 0 表示末尾没有未完成的匹配。
    State dp[PLEN + 1], next[PLEN + 1];
    for (int j = 0; j <= PLEN; j++) {
        dp[j].count = -1;
        dp[j].cost = INF;
    }
    dp[0].count = 0;
    dp[0].cost = 0;

    for (int k = 0; k < len; k++) {
        int deleteCost;
        scanf("%d", &deleteCost);
        for (int j = 1; j <= PLEN; j++) {
            // 选择一：把这个字符删掉，已匹配的 j 个字符还留在末尾
            next[j] = dp[j];
            if (next[j].count >= 0) next[j].cost += deleteCost;
            // 选择二：让它继续匹配第 j 个字符，不花代价
            if (text[k] == PATTERN[j - 1] && dp[j - 1].count >= 0) {
                next[j] = better(next[j], dp[j - 1]);
            }
        }
        // 匹配满 6 个字符就凑成了一个 "bessie"，回到「没有未完成匹配」的状态。
        // j = 0 时删掉字符不用付代价：这些字符留在原处，不属于任何一个 "bessie" 也不碍事
        State closed = next[PLEN];
        if (closed.count >= 0) closed.count++;
        dp[0] = better(dp[0], closed);
        for (int j = 1; j <= PLEN; j++) {
            dp[j] = next[j];
        }
    }

    printf("%d\n%d\n", dp[0].count, dp[0].cost);
    return 0;
}
