#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

const int MAX_SIZE = 10000; // 钻石尺寸的上界，直接开计数数组

// 滑动窗口 [i - k, i] 内的钻石尺寸差不超过 k，可以放同一只盒子。
// 窗口每次右移一格，用计数数组 O(1) 维护。
int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    vector<int> counts(MAX_SIZE + 1);
    for (int i = 0; i < n; ++i) {
        int size;
        scanf("%d", &size);
        ++counts[size];
    }

    int current = 0; // 当前窗口里的钻石数
    for (int i = 0; i <= k; ++i) current += counts[i];

    int answer = 0;
    for (int i = k + 1;; ++i) {
        answer = max(answer, current);
        if (i == (int)counts.size()) break;
        current += counts[i];          // 右端进入窗口
        current -= counts[i - k - 1];  // 左端离开窗口
    }
    printf("%d\n", answer);
    return 0;
}