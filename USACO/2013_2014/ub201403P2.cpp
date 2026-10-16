#include <algorithm>
#include <cstdio>
#include <vector>

// 用差分数组记录每个位置的草量, 再做前缀和得到每个位置的总草量.
// 然后滑动一个宽度为 2K+1 的区间, 求区间草量和的最大值.
int main() {
    const int MAX_POSITION = 1000000;
    std::vector<int> grass(MAX_POSITION + 2);
    int n, k;
    scanf("%d%d", &n, &k);
    for (int i = 0; i < n; ++i) {
        int amount, position;
        scanf("%d%d", &amount, &position);
        grass[position + 1] = amount;
    }

    // 前缀和: grass[i] = 前 i 个位置的累计草量, 便于 O(1) 求区间和.
    for (int i = 1; i < (int)grass.size(); ++i) grass[i] += grass[i - 1];

    int answer = 0;
    for (int left = 0; left < (int)grass.size(); ++left) {
        int right = left + k + k + 1; // 区间 [left, right] 宽 2K+1
        if (right >= (int)grass.size()) right = (int)grass.size() - 1;
        int sum = grass[right] - grass[left];
        answer = std::max(answer, sum);
    }
    printf("%d\n", answer);
    return 0;
}