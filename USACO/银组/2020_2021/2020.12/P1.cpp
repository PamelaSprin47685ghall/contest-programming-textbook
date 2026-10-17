#include <cstdio>

using namespace std;

const int MAXN = 100000;

int n;
int deg[MAXN + 5];  // 每个农场的度数

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n - 1; ++i) {
        int a, b;
        scanf("%d%d", &a, &b);
        --a;
        --b;
        ++deg[a];
        ++deg[b];
    }

    // 最优方案中每个农场只会"被首次感染"一次，所以移动事件恰好 n-1 次；
    // 一头牛离开后原地仍需要留一头，故一个农场若要在 d 个儿子上开花，
    // 必须先把自己这里的病牛复制到至少 d+1 头，需 ceil(log2(d+1)) 天。
    long long ans = n - 1;
    for (int i = 0; i < n; ++i) {
        // 以农场 1 为根，根没有父亲，其余农场要减去通往父亲的那条边
        int children = (i == 0) ? deg[i] : deg[i] - 1;
        if (children == 0) {
            continue;  // 叶子农场的病牛还要去感染儿子，不必再复制
        }
        int days = 0;
        int cows = 1;
        while (cows < children + 1) {
            ++days;
            cows *= 2;
        }
        ans += days;
    }

    printf("%lld\n", ans);
    return 0;
}
