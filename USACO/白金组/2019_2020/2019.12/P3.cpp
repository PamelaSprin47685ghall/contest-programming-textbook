#include <cstdio>

using namespace std;

const int MAXN = 300;
const int MAXDEG = MAXN * (MAXN - 1) / 2 + 1;  // 逆序数最多 N(N-1)/2，多项式再长一项

int n, k, mod;

// poly 存当前多项式，poly[d] 是 x^d 的系数（mod 意义下），polyLen 是有效长度。
int poly[MAXDEG];
int polyLen = 1;

// 取 x^degree 的系数；越界说明这种逆序数不可能出现，按 0 处理。
int coefficient(int degree) {
    if (degree < 0 || degree >= polyLen) return 0;
    return poly[degree];
}

// 乘上 (1 + x + ... + x^(b-1))：等价于"每个系数向后摊到 b 个位置"的滑动窗口求和。
// 先记成差分（i 处 +c[i]、i+b 处 -c[i]）再整体前缀和，就把 O(长度 * b) 降到 O(长度)。
void multiplyFactor(int b) {
    int oldLen = polyLen;
    polyLen = oldLen + b - 1;
    for (int i = oldLen; i < polyLen; ++i) poly[i] = 0;  // 高位残留的旧系数先清掉
    // 只标记 i+b 还在新长度以内的那些系数：最高位系数的滑窗尾巴本来就超出数组，不必标记。
    for (int i = oldLen - 2; i >= 0; --i) {
        poly[i + b] = (poly[i + b] - poly[i] + mod) % mod;
    }
    for (int i = 1; i < polyLen; ++i) {
        poly[i] += poly[i - 1];
        if (poly[i] >= mod) poly[i] -= mod;
    }
}

// 除掉 (1 + x + ... + x^(b-1))：把上面两步倒着做回去，同样只需 O(长度)；调用前保证能整除。
void divideFactor(int b) {
    for (int i = polyLen - 1; i >= 1; --i) {
        poly[i] -= poly[i - 1];
        if (poly[i] < 0) poly[i] += mod;
    }
    for (int i = 0; i + b < polyLen; ++i) {
        poly[i + b] += poly[i];
        if (poly[i + b] >= mod) poly[i + b] -= mod;
    }
    polyLen = polyLen - b + 1;
}

int main() {
    scanf("%d%d%d", &n, &k, &mod);

    // 逐个插入第 t 个数时，它相对前面 t-1 个数能带来 0..t-1 个新逆序；
    // 各步的生成函数相乘，x^d 的系数就是"恰好 d 个逆序的排列数"。
    poly[0] = 1;
    for (int t = 2; t <= n; ++t) multiplyFactor(t);

    int depthSum[MAXN];
    // 每个位置的深度里都有"自己"这一层，先加上恰好 k 个逆序的排列总数。
    for (int i = 0; i < n; ++i) depthSum[i] = coefficient(k);

    // 枚举所有位置对（left, left+diff）：一个是祖先、另一个更深的两种情况分别处理。
    for (int diff = 1; diff < n; ++diff) {
        divideFactor(diff + 1);  // 除掉 a[left..left+diff] 这一段自己完全自由的相对顺序
        // 左边更深：右端 a[left+diff] 是这一段的最小值，固定多出 diff 个逆序
        int leftDeeper = coefficient(k - diff);
        // 右边更深：最小值在最左端，不额外强迫逆序
        int rightDeeper = coefficient(k);
        multiplyFactor(diff + 1);  // 还原多项式，供下一对使用
        for (int left = 0; left + diff < n; ++left) {
            depthSum[left] = (depthSum[left] + leftDeeper) % mod;
            depthSum[left + diff] = (depthSum[left + diff] + rightDeeper) % mod;
        }
    }

    for (int i = 0; i < n; ++i) printf("%d ", depthSum[i]);
    printf("\n");
    return 0;
}
