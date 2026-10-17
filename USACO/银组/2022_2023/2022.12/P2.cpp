#include <cstdio>

using namespace std;

const int MAXV = 5000000;

static int turns[MAXV + 1];     // turns[v]：把一间有 v 头牛的屋子清空需要的最少回合数
static bool composite[MAXV + 1]; // 埃氏筛的合数标记
// largest[r]：目前已经筛出的最大素数 p，满足 p % 4 == r
static int largest[4] = {2, 1, 2, 3};

int main() {
    // 预处理：v 为偶数时 turns[v] = v/2；
    // v 为奇数时要先减掉一个同余的素数 p（余下 v-p 是 4 的倍数），取最大的 p 最优
    for (int i = 2; i <= MAXV; ++i) {
        if (!composite[i]) {
            for (int j = i; j <= MAXV; j += i) composite[j] = true;
            largest[i % 4] = i;
        }
        turns[i] = (i - largest[i % 4]) / 2 + 1;
    }
    turns[1] = 1; // P 只能取 1，一步清空

    int t;
    scanf("%d", &t);
    while (t--) {
        int n;
        scanf("%d", &n);
        // best 记录回合数最少的那间屋子的回合数，先出现的优先（回合数相同则靠前的屋子先空）
        int bestKey = -1, bestTurns = 0;
        for (int i = 0; i < n; ++i) {
            int a;
            scanf("%d", &a);
            int key = turns[a] / 2; // 两名农夫各走一步才算走完一间屋子的一轮
            if (bestKey < 0 || key < bestKey) {
                bestKey = key;
                bestTurns = turns[a];
            }
        }
        // 该屋子的回合数为奇数说明是农夫约翰最后走完它，回到这间屋子时轮到农夫 Nhoj 面对空屋子
        printf("%s\n", (bestTurns & 1) ? "Farmer John" : "Farmer Nhoj");
    }
    return 0;
}
