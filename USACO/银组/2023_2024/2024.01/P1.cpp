#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 300005;

int n, q;
long long maxScore;      // 题目中的 C
long long score[MAXN];   // 每个位置的分数；约翰忘记的位置会先填成最小可能值
int firstGreater[MAXN];  // firstGreater[a] = h，表示记忆「第 h 头是第一个比前 a 头都强的」
bool fixedByJohn[MAXN];  // 该分数由约翰给出，不能再改动

int main() {
    int testCount;
    scanf("%d", &testCount);
    while (testCount--) {
        scanf("%d%d%lld", &n, &q, &maxScore);
        for (int i = 1; i <= n; ++i) {
            scanf("%lld", &score[i]);
            fixedByJohn[i] = (score[i] != 0);
            if (score[i] == 0) {
                score[i] = 1;   // 候选序列要字典序最小，未知分数尽量取小
            }
            firstGreater[i] = 0;
        }
        for (int j = 0; j < q; ++j) {
            int a, h;
            scanf("%d%d", &a, &h);
            firstGreater[a] = h;
        }

        // 若 B(i) = h，则 i 到 h-1 的答案全是 h：这一段里还没出现第 h 头，
        // 前若干头的最大分数一直相同。于是把记忆向后铺满 [a, h)。
        bool possible = true;
        int cur = 1;
        while (possible && cur <= n) {
            int a = cur;
            while (cur < firstGreater[a]) {
                if (firstGreater[cur] != 0 && firstGreater[cur] != firstGreater[a]) {
                    possible = false;   // 同一个区间里出现两种不同的记忆，矛盾
                    break;
                }
                firstGreater[cur] = firstGreater[a];
                ++cur;
            }
            if (!possible) {
                break;
            }
            cur = max(cur, a + 1);
        }

        long long maxPrefix = 0;   // max(score[1..i])
        long long maxInside = 0;   // max(score[1..h-1])
        int i = 1;
        while (possible && i <= n) {
            maxPrefix = max(maxPrefix, score[i]);
            maxInside = max(maxInside, score[i]);
            if (firstGreater[i] == 0) {
                ++i;
                continue;
            }
            int h = firstGreater[i];
            for (int k = i; k < h; ++k) {
                maxInside = max(maxInside, score[k]);
            }
            if (maxInside > maxPrefix) {
                // 记忆要求前 i 头的最大分数等于前 h-1 头的最大分数，
                // 所以要把某个 j <= i 的分数抬到 maxInside；抬得越靠后字典序越小。
                int j = i;
                bool raised = false;
                while (j >= 1) {
                    if (firstGreater[j] != 0 && firstGreater[j] < h) {
                        // 抬 c[j] 会连带抬高更早那条记忆的「前若干头最大值」，
                        // 与 c[firstGreater[j]] 已经确定的取值冲突
                        possible = false;
                        break;
                    }
                    if (!fixedByJohn[j]) {
                        score[j] = maxInside;
                        raised = true;
                        break;
                    }
                    --j;
                }
                if (!possible) {
                    break;
                }
                if (!raised) {
                    possible = false;   // 没有可改的位置
                    break;
                }
                maxPrefix = maxInside;
            }
            if (!fixedByJohn[h]) {
                score[h] = maxPrefix + 1;   // 第一个更强，取刚好大一档
            }
            if (score[h] <= maxPrefix) {
                possible = false;   // 给出的分数压不住前面的最大值
                break;
            }
            // [i, h) 的答案都等于 h，这一段不必再单独处理
            i = h;
        }

        if (possible) {
            for (int k = 1; k <= n; ++k) {
                if (score[k] > maxScore) {
                    possible = false;
                    break;
                }
            }
        }
        if (!possible) {
            printf("-1\n");
            continue;
        }
        for (int k = 1; k <= n; ++k) {
            printf(k < n ? "%lld " : "%lld", score[k]);
        }
        printf("\n");
    }
    return 0;
}
