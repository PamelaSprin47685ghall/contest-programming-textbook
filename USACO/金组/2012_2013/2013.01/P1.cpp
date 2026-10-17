#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100005;

int n, k;
int breed[MAXN];        // 原样读入的品种编号, 最大到 10^9
int sortedBreed[MAXN];  // 排序去重后的品种编号, 用来做离散化
int code[MAXN];         // code[i]: 第 i 头牛离散化后的品种编号
int cnt[MAXN];          // cnt[x]: 当前窗口里品种 x 的奶牛数量

int main() {
    scanf("%d %d", &n, &k);
    for (int i = 0; i < n; i++) {
        scanf("%d", &breed[i]);
        sortedBreed[i] = breed[i];
    }

    // 品种编号最大 10^9, 无法直接开数组计数, 先排序去重再二分映射到 0..distinct-1
    sort(sortedBreed, sortedBreed + n);
    int distinct = unique(sortedBreed, sortedBreed + n) - sortedBreed;
    for (int i = 0; i < n; i++)
        code[i] = lower_bound(sortedBreed, sortedBreed + distinct, breed[i]) - sortedBreed;

    int answer = 0;
    int distinctInWindow = 0;  // 窗口里出现的品种数
    for (int right = 0, left = 0; right < n; right++) {
        if (cnt[code[right]] == 0) distinctInWindow++;
        cnt[code[right]]++;

        // 窗口里品种超过 K+1 个时无法只靠删除 K 个品种变成单品种, 左端点必须右移;
        // 移到刚好不超过 K+1 个品种为止, 此时窗口是以 right 结尾的最小合法窗口
        while (distinctInWindow > k + 1) {
            cnt[code[left]]--;
            if (cnt[code[left]] == 0) distinctInWindow--;
            left++;
        }

        // 只需统计第 right 头牛的品种在窗口里的数量就够了:
        // 若某个合法区间 [l, r] 里有 c 头品种 b, 取其中最后一头 b 的位置 p,
        // 扫描到 right = p 时最小合法左端点一定不晚于 l (因为 [l, p] 的品种数不超过 K+1),
        // 于是更宽的窗口里品种 b 的数量不少于 c, 一定能被统计到
        if (cnt[code[right]] > answer) answer = cnt[code[right]];
    }

    printf("%d\n", answer);
    return 0;
}
