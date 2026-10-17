#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 100000;
const int MAXLABEL = 1000000000; // 车厢标签的上界
const int MOD = 1000000007;

int seqDp[MAXN + 3]; // 计数用的 DP

// 快速幂：求 base 的 exponent 次方对 MOD 取模
int powerMod(long long base, int exponent) {
    long long result = 1;
    base %= MOD;
    while (exponent > 0) {
        if (exponent & 1) result = result * base % MOD;
        base = base * base % MOD;
        exponent >>= 1;
    }
    return (int)result;
}

// 数出长度为 len 的序列个数：每个元素都不小于 mnval，
// 并且每个长度恰为 k 的窗口的最小值恰好是 mnval（即窗口里至少出现一次 mnval）
int countSequences(int mnval, int k, int len) {
    long long bigger = MAXLABEL - mnval; // 严格大于 mnval 的取值个数
    if (len < k) return powerMod(bigger + 1, len); // 不足一个窗口，元素只需不小于 mnval
    // seqDp[u]：长度 u、每个窗口最小值都是 mnval、且末尾元素恰好等于 mnval 的序列数。
    // 按"最后一个等于 mnval 的位置"分类可得 seqDp[u] = seqDp[u-1] + x*seqDp[u-2] + ... ，
    // 相邻两项相减就得到 seqDp[u+1] = seqDp[u] + x*(seqDp[u] - x^(k-1)*seqDp[u-k])，
    // 于是不必枚举 k 项，线性递推即可。
    seqDp[0] = 1;
    seqDp[1] = 1;
    for (int i = 2; i <= k; ++i) {
        seqDp[i] = (int)((long long)seqDp[i - 1] * (bigger + 1) % MOD);
    }
    long long lowPower = powerMod(bigger, k - 1); // x^(k-1)，只算一次，避免每段都重建幂表
    for (int i = k; i <= len; ++i) {
        long long cur = seqDp[i] - (long long)seqDp[i - k] * lowPower % MOD;
        cur = (cur % MOD + MOD) % MOD;
        cur = (cur * bigger + seqDp[i]) % MOD;
        seqDp[i + 1] = (int)cur;
    }
    return seqDp[len + 1];
}

int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    int windowCount = n - k + 1;

    // 把滑动窗口最小值序列压缩成 (取值, 连续出现次数)。
    // 相邻两个窗口最小值不同时能立刻定下某节车厢的标签：
    // 若 c[i] < c[i+1]，则第 i 节车厢等于 c[i]；若 c[i] > c[i+1]，则第 i+k 节车厢等于 c[i+1]。
    vector<pair<int, int> > runs;
    for (int i = 0; i < windowCount; ++i) {
        int value;
        scanf("%d", &value);
        if (!runs.empty() && runs.back().first == value) {
            ++runs.back().second;
        } else {
            runs.push_back(make_pair(value, 1));
        }
    }

    // 这些被定死的车厢把整条序列切成若干段，每段内部的元素都只需不小于该段的 mnval，
    // 且段内每个完整窗口的最小值恰好是该段的 mnval，各段之间互不干扰，
    // 所以答案就是各段方案数之积。段的长度由前后两段取值的大小关系决定。
    int runCount = (int)runs.size();
    long long answer = 1;
    if (runCount == 1) {
        answer = countSequences(runs[0].first, k, n);
    } else {
        for (int i = 0; i < runCount; ++i) {
            int value = runs[i].first;
            int len;
            if (i == 0) {
                // 最左一段：右侧定死的车厢由它与下一段的大小关系决定
                if (runs[1].first > value) {
                    len = runs[0].second - 1; // 右端 c 上升，第 r 节车厢被定死
                } else {
                    len = runs[0].second + k - 1; // 右端 c 下降，定死的是第 r+k 节
                }
            } else if (i == runCount - 1) {
                // 最右一段：左侧定死的车厢由它与上一段的大小关系决定
                if (runs[i - 1].first > value) {
                    len = runs[i].second - 1;
                } else {
                    len = runs[i].second + k - 1;
                }
            } else {
                bool biggerBefore = runs[i - 1].first > value;
                bool biggerAfter = runs[i + 1].first > value;
                if (biggerBefore) {
                    if (biggerAfter) {
                        // 两侧都比本段大：两端的定死车厢都落在本段内部，
                        // 中间空出来的长度要减掉 2k + 1，可能为空
                        len = max(0, runs[i].second - k - 1);
                    } else {
                        len = runs[i].second - 1;
                    }
                } else {
                    if (biggerAfter) {
                        len = runs[i].second - 1;
                    } else {
                        len = runs[i].second + k - 1;
                    }
                }
            }
            answer = answer * countSequences(value, k, len) % MOD;
        }
    }

    printf("%d\n", (int)answer);
    return 0;
}
