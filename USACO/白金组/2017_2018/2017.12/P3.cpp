#include <cstdio>

using namespace std;

const int MAXN = 100005;

int n;
int jump[MAXN + 1];  // 第 i 头牛的“跳跃号码”：她拿到礼物后插回的名次，等于 n - c_i
int cnt[MAXN + 2];   // 检查时统计前 k-1 头牛里每种跳跃号码的个数

// 检查前 k 头牛（1..k）是否都能拿到礼物
// 由“任意两头牛的相对顺序永远不会改变”可知：只要第 k 头牛能拿到礼物，
// 前面的牛也都拿得到，所以只需检查第 k 头牛。
// 第 k 头牛永远拿不到礼物 ⇔ 存在 i，使她前面至少有 i 头牛的跳跃号码 ≤ i：
// 这 i 头牛每次插队都只能插到名次 ≤ i 的位置，永远挡在她前面。
bool allReceive(int k) {
    for (int i = 1; i <= n; ++i) cnt[i] = 0;
    for (int i = 0; i + 1 < k; ++i) ++cnt[jump[i]];
    int total = 0;
    for (int i = 1; i <= n; ++i) {
        total += cnt[i];
        if (total >= i) return false;
    }
    return true;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        int c;
        scanf("%d", &c);
        // 拿到礼物后插到“恰好 c 头牛之前”，即前面剩下 n-1-c 头牛，名次为 n-c
        jump[i] = n - c;
    }
    // 拿不到礼物的牛构成初始队列的一个后缀，二分出最后一个能拿到礼物的牛
    int lo = 1;      // 前 lo 头牛都能拿到礼物
    int hi = n + 1;  // 前 hi 头牛不能全部拿到（哨兵）
    while (hi - lo > 1) {
        int mid = (lo + hi) / 2;
        if (allReceive(mid)) {
            lo = mid;
        } else {
            hi = mid;
        }
    }
    printf("%d\n", n - lo);
    return 0;
}
