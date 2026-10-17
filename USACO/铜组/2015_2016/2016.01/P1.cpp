#include <cstdio>

using namespace std;

// 从最高组往低组算：某一组及以上的总人数在赛前赛后的差，
// 就是升入该组的数量，也就是低一级组升入这一组的人数。
int main() {
    int before[4], after[4]; // 依次是 Bronze, Silver, Gold, Platinum 的人数
    for (int i = 0; i < 4; ++i) {
        scanf("%d%d", &before[i], &after[i]);
    }
    int totalBefore = 0, totalAfter = 0;
    int promote[3]; // promote[i] 表示第 i 组升入第 i+1 组的人数
    for (int i = 3; i > 0; --i) {
        totalBefore += before[i];
        totalAfter += after[i];
        promote[i - 1] = totalAfter - totalBefore;
    }
    for (int i = 0; i < 3; ++i) {
        printf("%d\n", promote[i]);
    }
    return 0;
}