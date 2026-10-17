#include <cstdio>
#include <algorithm>

using namespace std;

int main() {
    int N;
    scanf("%d", &N);

    // 记前缀和 S[i] 为前 i 头牛的编号之和，则区间 (l, r] 的和为 S[r]-S[l]，
    // 它被 7 整除等价于 S[r] 与 S[l] 模 7 同余。所以对每个余数 r，只要记住
    // 最早出现的位置 first[r] 和最晚出现的位置 last[r]，二者之差就是一个
    // 和为 7 的倍数的区间的最大长度。first[0] = 0 表示空的空前缀。
    int first[7], last[7];
    for (int r = 0; r < 7; ++r) {
        first[r] = -1;
        last[r] = -1;
    }
    first[0] = 0;

    int pref = 0; // 前缀和模 7 的余数（编号很大，但取模后不会溢出）
    for (int i = 1; i <= N; ++i) {
        int id;
        scanf("%d", &id);
        pref = (pref + id) % 7;
        if (first[pref] == -1) {
            first[pref] = i;
        }
        last[pref] = i;
    }

    int ans = 0;
    for (int r = 0; r < 7; ++r) {
        // 余数 r 至少要出现两次才能夹出一个非空区间
        if (first[r] != -1 && last[r] > first[r]) {
            ans = max(ans, last[r] - first[r]);
        }
    }
    printf("%d\n", ans);
    return 0;
}
