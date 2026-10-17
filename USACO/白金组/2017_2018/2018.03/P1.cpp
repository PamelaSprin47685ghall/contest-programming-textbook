#include <algorithm>
#include <cstdio>

using namespace std;

const int MAXN = 100000;

int n;
int value[MAXN];        // 输入数组
int sortedOrder[MAXN];  // 把下标按 (值, 下标) 排序后的顺序
int needPass[MAXN + 1]; // needPass[i]：前 i 个位置与后 n-i 个位置之间的划分点
                        // 最晚要几趟冒泡排序才会出现

bool byValueThenIndex(int a, int b) {
    if (value[a] != value[b]) return value[a] < value[b];
    return a < b;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) scanf("%d", &value[i]);
    for (int i = 0; i < n; ++i) sortedOrder[i] = i;
    sort(sortedOrder, sortedOrder + n, byValueThenIndex);

    // 划分点 i 成立，要求"前 i 小的元素"恰好都落在前 i 个位置上。设这些元素里
    // 原始下标的最大值为 mx，每趟冒泡让最靠后的越位元素恰好左移一格，所以需要
    // mx+1-i 趟。排序时值相同时按下标排序，保证 mx 取的是合法的一组"前 i 小"。
    int mx = 0;
    for (int i = 1; i < n; ++i) {
        if (sortedOrder[i - 1] > mx) mx = sortedOrder[i - 1];
        needPass[i] = mx + 1 - i;
    }

    // 把递归过程压平来看：对若干已经分开的段分别做冒泡，和整段一起做一趟的效果、
    // 开销都相同。一个元素只要左右两侧的划分点还没有同时出现，就会一直贡献长度 1；
    // 所以它贡献的趟数是两侧 needPass 的较大值。do-while 保证第一趟一定会做，
    // 因此一开始就被隔开的元素也要贡献一趟——除非整个数组只有一个元素。
    long long answer = 0;
    for (int i = 0; i < n; ++i) {
        int pass = 0;
        if (i > 0) {
            if (needPass[i] > pass) pass = needPass[i];
        }
        if (i + 1 < n) {
            if (needPass[i + 1] > pass) pass = needPass[i + 1];
        }
        if (pass == 0 && n > 1) pass = 1;
        answer += pass;
    }

    printf("%lld\n", answer);
    return 0;
}
