#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100005;

struct Entry {
    int value;  // 元素的值
    int index;  // 元素在原数组中的下标
};

Entry entries[MAXN];

// 按值升序排序; 值相同时按下标升序, 这正好等于稳定排序后每个元素的最终位置
bool cmpEntry(const Entry &a, const Entry &b) {
    if (a.value != b.value) return a.value < b.value;
    return a.index < b.index;
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        entries[i].index = i;
        scanf("%d", &entries[i].value);
    }

    sort(entries, entries + n, cmpEntry);

    // 一趟冒泡最多让一个元素向左移动一格, 所以元素 i 需要左移的格数 i - rank[i] 是所需趟数的下界;
    // 反过来, 每一趟过后 max(i - rank[i]) 恰好减 1, 因此它就是需要的趟数(答案还要加上最后退出循环的那一趟).
    int need = 0;
    for (int rank = 0; rank < n; rank++) {
        int moveLeft = entries[rank].index - rank;
        if (moveLeft > need) need = moveLeft;
    }

    printf("%d\n", need + 1);
    return 0;
}
