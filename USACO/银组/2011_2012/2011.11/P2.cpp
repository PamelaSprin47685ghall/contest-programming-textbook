#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 50005;
const int INF = 2000000000;    // 位置差最大 1e9, 用它当"无穷大"足够

struct Cow {
    int pos;        // 位置 (x 坐标)
    int breed;      // 压缩后的品种编号, 范围 0..total-1
};

// 按位置从左到右排序, 便于用双指针扫一段连续的奶牛
bool byPos(const Cow &a, const Cow &b) {
    return a.pos < b.pos;
}

int n;
Cow cows[MAXN];
int rawBreed[MAXN];    // 品种编号最大 1e9, 先原样读入
int ids[MAXN];         // 用来排序去重, 把品种编号压缩到 0..total-1
int cnt[MAXN];         // 窗口内每个品种的出现次数

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &cows[i].pos, &rawBreed[i]);
        ids[i] = rawBreed[i];
    }

    // 品种编号可达 1e9, 无法直接开数组, 排序去重后用二分映射到 0..total-1
    sort(ids, ids + n);
    int total = (int)(unique(ids, ids + n) - ids);
    for (int i = 0; i < n; i++)
        cows[i].breed = (int)(lower_bound(ids, ids + total, rawBreed[i]) - ids);

    sort(cows, cows + n, byPos);

    // 双指针: 右端点不断右移, 直到窗口里包含全部品种;
    // 之后再尽量右移左端点(只要不丢品种), 得到的窗口就是"右端点为 cows[right]
    // 时的最小窗口", 它的位置差就是该右端点下的最优代价.
    int left = 0, have = 0, answer = INF;
    for (int right = 0; right < n; right++) {
        if (cnt[cows[right].breed] == 0) have++;
        cnt[cows[right].breed]++;
        while (cnt[cows[left].breed] > 1) {
            cnt[cows[left].breed]--;
            left++;
        }
        if (have == total) {
            int cost = cows[right].pos - cows[left].pos;
            if (cost < answer) answer = cost;
        }
    }

    printf("%d\n", answer);
    return 0;
}
