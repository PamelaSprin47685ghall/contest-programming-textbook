#include <cstdio>
#include <algorithm>

using namespace std;

const int SIZE = 1 << 16; // 65536 >= 40000, 线段树的叶子数量

int n, d;

// 每个区间记四个值, 表示区间左右两端"允许使用 / 禁止使用"的四种组合下,
// 区间内部能挤出的最大产奶量. 字母含义: b = 可以自由使用, p = 禁止使用.
// 例如 bp[x] = 区间 x 内左端点自由、右端点被禁止时的最大值.
long long bb[2 * SIZE], bp[2 * SIZE], pb[2 * SIZE], pp[2 * SIZE];

// 由两个儿子的信息合并出父亲: 相邻的机器不能同时使用, 所以拼接时
// 左儿子的右端和右儿子的左端至少有一个要"禁止使用".
void Update(int node) {
    int l = node * 2, r = node * 2 + 1;
    pp[node] = max(pb[l] + pp[r], pp[l] + bp[r]);
    pb[node] = max(pb[l] + pb[r], pp[l] + bb[r]);
    bp[node] = max(bb[l] + pp[r], bp[l] + bp[r]);
    bb[node] = max(bb[l] + pb[r], bp[l] + bb[r]);
}

int main() {
    scanf("%d %d", &n, &d);
    for (int i = 0; i < n; i++) {
        int milk;
        scanf("%d", &milk);
        bb[SIZE + i] = milk; // 单台机器: 两端自由时最多就是用它本身
    }
    for (int i = SIZE - 1; i >= 1; i--) Update(i);

    long long total = 0;
    for (int i = 0; i < d; i++) {
        int idx, milk;
        scanf("%d %d", &idx, &milk);
        int leaf = SIZE + idx - 1;
        bb[leaf] = milk;
        // 只有叶子的祖先会变, 自底向上重算
        for (int j = leaf / 2; j >= 1; j /= 2) Update(j);
        total += bb[1];
    }
    printf("%lld\n", total);
    return 0;
}
