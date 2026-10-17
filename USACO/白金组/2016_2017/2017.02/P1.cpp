#include <cstdio>

using namespace std;

const int MAXN = 100000; // 奶牛品种数上限

int n;
int sideA[MAXN + 1]; // 路一侧各字段的品种顺序
int sideB[MAXN + 1]; // 路另一侧各字段的品种顺序
int posA[MAXN + 1];  // 每个品种在 A 侧的位置
int posB[MAXN + 1];  // 每个品种在 B 侧的位置
int perm[MAXN + 1];  // perm[i]：当前排列里第 i 头牛在另一侧的位置
int bit[MAXN + 1];

void bitAdd(int x) {
    for (; x <= n; x += x & -x) ++bit[x];
}

int bitSum(int x) {
    int s = 0;
    for (; x > 0; x -= x & -x) s += bit[x];
    return s;
}

// 数 perm[1..n] 的逆序对数（调用前会清空树状数组）
long long countInversions() {
    for (int i = 1; i <= n; ++i) bit[i] = 0;
    long long inv = 0;
    for (int i = 1; i <= n; ++i) {
        // 已插入的 i-1 个数中比 perm[i] 大的，正是以 i 结尾的新增逆序对
        inv += (i - 1) - bitSum(perm[i]);
        bitAdd(perm[i]);
    }
    return inv;
}

// 枚举把 first 侧循环移位 0..n-1 位的全部情况，返回最少交叉对数
long long bestWithShift(int *first, int *posOther) {
    for (int i = 1; i <= n; ++i) perm[i] = posOther[first[i]];
    long long cur = countInversions(); // 不移位（k=0）时的交叉对数
    long long ans = cur;
    // 循环左移一位相当于把队首的牛搬到队尾。设这头牛在另一侧的位置是 v，
    // 它在队首时参与 v-1 对交叉（另一侧排在它前面的牛都跟它交叉），
    // 搬到队尾后参与 n-v 对，因此交叉总对数增加 n-2v+1。
    // 连续做 n 次就遍历了全部循环移位。
    for (int i = 1; i <= n; ++i) {
        cur += n - 2LL * perm[i] + 1;
        if (cur < ans) ans = cur;
    }
    return ans;
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) scanf("%d", &sideA[i]);
    for (int i = 1; i <= n; ++i) scanf("%d", &sideB[i]);
    for (int i = 1; i <= n; ++i) posA[sideA[i]] = i;
    for (int i = 1; i <= n; ++i) posB[sideB[i]] = i;

    // 题目允许任意一侧移位，两侧都试一遍
    long long ansA = bestWithShift(sideA, posB);
    long long ansB = bestWithShift(sideB, posA);
    printf("%lld\n", ansA < ansB ? ansA : ansB);
    return 0;
}
