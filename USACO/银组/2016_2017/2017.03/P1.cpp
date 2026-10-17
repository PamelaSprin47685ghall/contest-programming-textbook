#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100005;

struct Group {
    long long output;       // 这一组每头奶牛的产奶量
    long long count;        // 产奶量为 output 的奶牛头数
};

int groupCount;
Group group[MAXN];

// 按产奶量升序排序, 排序后同一个区间里的最小值在左端, 最大值在右端.
bool compareByOutput(const Group &a, const Group &b) {
    return a.output < b.output;
}

int main() {
    scanf("%d", &groupCount);
    for (int i = 0; i < groupCount; i++) {
        scanf("%lld%lld", &group[i].count, &group[i].output);
    }
    sort(group, group + groupCount, compareByOutput);

    long long answer = 0;
    int low = 0, high = groupCount - 1;
    while (low <= high) {
        // 最优配法是"产奶量最小的配产奶量最大的": 若 A<B<C<D 时存在配对 (A,B) 与 (C,D),
        // 把 B 和 D 互换得到 (A,D) 与 (B,C), 两间隔间的耗时都不超过原来的 C+D, 所以最小配最大不劣.
        long long pairs = min(group[low].count, group[high].count);
        if (low == high) {
            pairs = group[low].count / 2;   // 区间内只剩一种产奶量时, 只能同产奶量的牛互相配对
        }
        long long time = group[low].output + group[high].output;
        if (time > answer) {
            answer = time;
        }
        group[low].count -= pairs;
        group[high].count -= pairs;
        if (group[low].count == 0) {
            low++;
        }
        if (group[high].count == 0) {
            high--;
        }
    }
    printf("%lld\n", answer);
    return 0;
}
