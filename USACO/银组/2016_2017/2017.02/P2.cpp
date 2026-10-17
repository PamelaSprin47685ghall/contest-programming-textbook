#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100000;

int n, k, b;
int broken[MAXN + 5];       // 先标记损坏信号, 之后原地变成前缀和

int main() {
    scanf("%d%d%d", &n, &k, &b);
    for (int i = 0; i < b; i++) {
        int id;
        scanf("%d", &id);
        broken[id] = 1;
    }

    // broken[i] 变为前 i 个信号中损坏的个数
    for (int i = 1; i <= n; i++) broken[i] += broken[i - 1];

    // 枚举所有长度为 k 的连续区间, 区间内的损坏数就是要修的信号数
    int answer = k;         // 上界: 最坏情况下整段都得修
    for (int i = 1; i + k - 1 <= n; i++) {
        int repaired = broken[i + k - 1] - broken[i - 1];
        answer = min(answer, repaired);
    }
    printf("%d\n", answer);
    return 0;
}
