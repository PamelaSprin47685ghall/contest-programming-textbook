#include <cstdio>
#include <vector>

// 一次洗牌把位置 i 的牛送到 shuffleTo[i].
// 三次洗牌后, 初始在位置 i 的牛到达 shuffleTo³(i).
// 题目给的是三次洗牌后的顺序, 因此初始位置 i 的牛 = 最终位于 shuffleTo³(i) 的牛.
int main() {
    int n;
    scanf("%d", &n);
    std::vector<int> shuffleTo(n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &shuffleTo[i]);
        --shuffleTo[i];
    }
    std::vector<int> cow(n);
    for (int i = 0; i < n; ++i) scanf("%d", &cow[i]);
    for (int i = 0; i < n; ++i) {
        int pos = i;
        for (int step = 0; step < 3; ++step) {
            pos = shuffleTo[pos];
        }
        printf("%d\n", cow[pos]);
    }
    return 0;
}