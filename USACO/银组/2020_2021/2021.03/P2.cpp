#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 7;         // 每个测试用例最多给出 7 个数
const int MAXC = MAXN * MAXN + MAXN;    // 候选值个数：x_i 本身 + 所有正的差值

int n;
long long x[MAXN];
long long cand[MAXC];

// 判断 (a,b,c) 是否与给定的 n 个数相容：每个 x_i 都要是 7 个值之一
bool compatible(long long a, long long b, long long c) {
    long long value[7];
    value[0] = a;
    value[1] = b;
    value[2] = c;
    value[3] = a + b;
    value[4] = b + c;
    value[5] = c + a;
    value[6] = a + b + c;
    for (int i = 0; i < n; ++i) {
        bool found = false;
        for (int j = 0; j < 7; ++j) {
            if (value[j] == x[i]) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    return true;
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 0; i < n; ++i) scanf("%lld", &x[i]);

        // 把 0 也算进 x 里：8 个可能取值可以配成 4 对
        // (0,A) (B,A+B) (C,A+C) (B+C,A+B+C)，每对的差都是 A；
        // x 加 0 至少 5 个互不相同的数落入 4 对，必有一对里出现两个不同的数，
        // 于是 A 一定是两个数的差（0 与 x_i 的差就是 x_i 本身）。B、C 同理。
        int m = 0;
        for (int i = 0; i < n; ++i) cand[m++] = x[i];
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (x[j] > x[i]) cand[m++] = x[j] - x[i];
            }
        }
        sort(cand, cand + m);
        m = (int)(unique(cand, cand + m) - cand);

        // A <= B <= C，在候选值里三重枚举即可
        int answer = 0;
        for (int ai = 0; ai < m; ++ai) {
            for (int bi = ai; bi < m; ++bi) {
                for (int ci = bi; ci < m; ++ci) {
                    if (compatible(cand[ai], cand[bi], cand[ci])) ++answer;
                }
            }
        }
        printf("%d\n", answer);
    }
    return 0;
}
