#include <cstdio>
#include <cstring>

using namespace std;

const int MAXLEN = 300005;

char t[MAXLEN + 1];
const char* target = "bessie";

int main() {
    scanf("%s", t);
    int len = (int)strlen(t);

    // waiting[d]：正在等 "bessie"[d] 的贪心指针个数。
    // waiting[6] 表示刚凑齐一个 "bessie"、等待结算的指针个数。
    long long waiting[7] = {0, 0, 0, 0, 0, 0, 0};
    long long answer = 0;

    for (int i = 0; i < len; ++i) {
        char c = t[i];
        waiting[0]++;                 // 以 i 为起点的贪心过程：先等一个 'b'
        // 从后往前移动：bessie 里有重复字符（两个 's'、两个 'e'），
        // 倒序扫描保证同一个字符不会被同一个指针用两次。
        for (int d = 5; d >= 0; --d) {
            if (c == target[d]) {
                waiting[d + 1] += waiting[d];
                waiting[d] = 0;
            }
        }
        // 此刻凑齐的 "bessie" 都在位置 i 结束，凡是右端点 >= i 的子串都包含它，
        // 一共有 len - i 个，故每个这样的指针给答案贡献 len - i。
        answer += waiting[6] * (len - i);
        waiting[0] += waiting[6];     // 这些贪心过程继续往后找下一个 "bessie"
        waiting[6] = 0;
    }

    printf("%lld\n", answer);
    return 0;
}
