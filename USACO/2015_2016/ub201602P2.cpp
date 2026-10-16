#include <cstdio>
#include <limits>
#include <vector>

// 枚举出口的位置。相邻两个位置的总路程可以 O(1) 递推：
// 出口顺时针移一格，所有牛少走 1 步，而原来正对出口的
// rooms[i] 头牛要多走一整圈 n 步。
int main() {
    int n;
    scanf("%d", &n);
    std::vector<int> rooms(n);
    for (int& r : rooms) scanf("%d", &r);

    // 出口在第 0 个房间时，第 i 个房间的牛要走 i 步。
    int total = 0, sum = 0;
    for (int i = 0; i < n; ++i) {
        total += rooms[i] * i;
        sum += rooms[i];
    }

    int answer = std::numeric_limits<int>::max();
    for (int i = 0; i < n; ++i) {
        answer = std::min(answer, total);
        total -= sum;          // 每头牛都少走 1 步
        total += rooms[i] * n; // 原出口处的牛改走一整圈
    }
    printf("%d\n", answer);
    return 0;
}