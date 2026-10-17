#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 50005;   // N 的上限
const int MAXC = 100005;  // 牌面最大是 2N, 即 100000

int n;
int elsie[MAXN];      // Elsie 依次打出的牌
int mine[MAXN];       // Bessie 的牌, 升序排列
bool used[MAXC];      // 标记某张牌是否在 Elsie 手里

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &elsie[i]);
        used[elsie[i]] = true;
    }

    int cnt = 0;
    for (int card = 1; card <= 2 * n; card++) {
        if (!used[card]) mine[cnt++] = card;   // 整副牌去掉 Elsie 的, 剩下的都是 Bessie 的
    }

    int half = n / 2;
    sort(elsie, elsie + half);          // 上半场 Elsie 的牌升序
    sort(elsie + half, elsie + n);      // 下半场 Elsie 的牌升序

    int score = 0;

    // 上半场比大, 所以 Bessie 的牌里较大的 half 张都应该留在这里用,
    // 下半场比小, 较小的 half 张留在下半场用: 把上下半场里"位置放反"的两张牌互换,
    // 上半场只会变大、下半场只会变小, 因此不会更差, 于是两个半场可以完全独立地贪心。
    // 上半场: 对 Elsie 从小到大的每张牌, 用"能赢它的最小牌"去迎战。
    int j = half;   // mine[half..n-1] 是 Bessie 较大的 half 张
    for (int i = 0; i < half; i++) {
        // 比 elsie[i] 还小的牌, 对后面更大的 Elsie 牌同样赢不了, 可以直接跳过
        while (j < n && mine[j] < elsie[i]) j++;
        if (j == n) break;
        score++;
        j++;
    }

    // 下半场比小, 把牌面 x 看成 2N+1-x 就等价于"比大", 于是贪心方向整体反过来:
    // 对 Elsie 从大到小的每张牌, 用"能赢它的最大牌"(即比它小的最大牌)去迎战。
    j = half - 1;   // mine[0..half-1] 是 Bessie 较小的 half 张
    for (int i = n - 1; i >= half; i--) {
        while (j >= 0 && mine[j] > elsie[i]) j--;
        if (j < 0) break;
        score++;
        j--;
    }

    printf("%d\n", score);
    return 0;
}
