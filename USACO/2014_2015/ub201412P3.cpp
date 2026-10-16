#include <cstdio>
#include <limits>
#include <vector>

int main() {
    int n;
    scanf("%d", &n);
    std::vector<int> speed(n);
    for (int i = 0; i < n; ++i) {
        // 位置无关紧要，只关心速度，故用 %*d 丢弃位置。
        scanf("%*d%d", &speed[i]);
    }
    int answer = 0;
    int current = std::numeric_limits<int>::max();
    // 从右往左：只要当前奶牛不比右边已形成的队伍慢，就自成新队伍。
    for (int i = n - 1; i >= 0; --i) {
        if (speed[i] <= current) {
            current = speed[i];
            ++answer;
        }
    }
    printf("%d\n", answer);
    return 0;
}
