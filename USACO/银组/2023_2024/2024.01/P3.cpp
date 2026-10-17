#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 10005;
const int MAXCAND = 800000;   // 4 个数的 6 个差值，每个差值最多贡献 2*sqrt(4e9) 个因数

long long month[MAXN];
long long candidate[MAXCAND];
long long seen[4];   // 已出现的余数种类，最多记 4 种

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%lld", &month[i]);
    }
    sort(month, month + n);

    // 相同的月份对「有几种余数」没有影响，先去重
    int distinct = 0;
    for (int i = 0; i < n; ++i) {
        if (i == 0 || month[i] != month[i - 1]) {
            month[distinct++] = month[i];
        }
    }

    // 每个月至少 4 周，所以 L <= 最短月份 / 4
    long long maxWeek = month[0] / 4;
    if (distinct <= 3) {
        // 余数种类本来就不会超过 3 种，所有 L 都合法
        printf("%lld\n", maxWeek * (maxWeek + 1) / 2);
        return 0;
    }

    // L 合法时，前 4 个不同月份里必有两只余数相同，即 L 整除它们的差。
    // 于是所有候选 L 都藏在这 6 个差值的因数里。
    int candidateCount = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < i; ++j) {
            long long diff = month[i] - month[j];
            for (long long t = 1; t * t <= diff; ++t) {
                if (diff % t != 0) {
                    continue;
                }
                if (t <= maxWeek) {
                    candidate[candidateCount++] = t;
                }
                long long other = diff / t;
                if (other != t && other <= maxWeek) {
                    candidate[candidateCount++] = other;
                }
            }
        }
    }
    sort(candidate, candidate + candidateCount);

    long long answer = 0;
    for (int i = 0; i < candidateCount; ++i) {
        if (i > 0 && candidate[i] == candidate[i - 1]) {
            continue;   // 同一个 L 可能被多个差值算到
        }
        long long week = candidate[i];
        int kinds = 0;
        for (int k = 0; k < distinct; ++k) {
            long long rest = month[k] % week;
            bool found = false;
            for (int s = 0; s < kinds; ++s) {
                if (seen[s] == rest) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                if (kinds == 3) {
                    kinds = 4;   // 出现第 4 种余数，这个 L 不合法
                    break;
                }
                seen[kinds++] = rest;
            }
        }
        if (kinds <= 3) {
            answer += week;
        }
    }
    printf("%lld\n", answer);
    return 0;
}
