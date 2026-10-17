#include <cstdio>
#include <vector>

int main() {
    int n;
    scanf("%d", &n);
    std::vector<int> cows(n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &cows[i]);
    }

    // 从后往前寻找最长的严格递增后缀.
    // 只要后缀已经升序排列, 之前乱序的每头牛只需依次插入到后缀的合适位置.
    int sortedSuffixLength = 1;
    for (int i = n - 2; i >= 0; --i) {
        if (cows[i] < cows[i + 1]) {
            ++sortedSuffixLength;
        } else {
            break;
        }
    }

    // 所需最少步数即为不在后缀内的牛的数量
    printf("%d\n", n - sortedSuffixLength);
    return 0;
}
