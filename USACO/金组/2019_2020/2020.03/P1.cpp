#include <cstdio>

using namespace std;

const int MAXN = 100005;

int N;
int length[MAXN];         // 每根头发的初始长度
long long tree[MAXN];     // 树状数组：统计已经扫过的长度各出现了多少次
long long countAt[MAXN];  // countAt[v]：右端头发长度为 v 的逆序对总数

int lowbit(int x) {
    return x & (-x);
}

void addLength(int value) {
    // 长度范围是 0..N，而树状数组下标从 1 开始，所以统一加一
    for (int i = value + 1; i <= N + 1; i += lowbit(i)) {
        tree[i]++;
    }
}

long long countUpTo(int value) {
    // 已经扫过、且长度不超过 value 的头发根数
    long long result = 0;
    for (int i = value + 1; i > 0; i -= lowbit(i)) {
        result += tree[i];
    }
    return result;
}

int main() {
    scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        scanf("%d", &length[i]);
    }

    // 先算出每个位置 y 左边比它长的头发根数 n[y]：
    // 扫到 y 时，树状数组里正好有它左边全部 y 根头发，
    // 长度不超过 length[y] 的能直接查到，剩下的就是更长的。
    for (int y = 0; y < N; y++) {
        countAt[length[y]] += y - countUpTo(length[y]);
        addLength(length[y]);
    }

    // j 增大 1 时，只有长度恰为 j 的头发从 j 变到 j+1，
    // 新出现的逆序对恰好是「右边长度为 j、左边更长」的那些，
    // 也就是 countAt[j]。所以坏度按长度做前缀和即可，不必对每个 j 重算。
    long long badness = 0;
    for (int j = 0; j < N; j++) {
        printf("%lld\n", badness);
        badness += countAt[j];
    }
    return 0;
}
