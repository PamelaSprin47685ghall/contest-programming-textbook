#include <cstdio>

using namespace std;

const int MAXN = 305;

int n;
int range[MAXN][MAXN]; // range[i][j] = a[i..j] 的极差（输入直接给出）
long long a[MAXN];     // 还原出的数组；倒推过程中可能出现较大的正负偏移，用 64 位保存

// 校验第 i 行：a[i..j] 的极差必须等于 range[i][j]
bool rowOk(int i) {
    long long mx = a[i], mn = a[i];
    for (int j = i; j <= n; ++j) {
        if (a[j] > mx) mx = a[j];
        if (a[j] < mn) mn = a[j];
        if (mx - mn != (long long)range[i][j]) return false;
    }
    return true;
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        for (int j = i; j <= n; ++j) scanf("%d", &range[i][j]);
    }

    // 一个元素与自己的极差是 0，所以最后一个元素可以任取，取 0 最方便
    a[n] = 0;
    // 倒推：a[i] 只能是 a[i+1] + range[i][i+1] 或 a[i+1] - range[i][i+1]。
    // 此时 a[i+1..n] 已经固定，第 i 行的全部极差都确定了，可以整行校验；
    // 先试加号，矛盾就改用减号（题解可证明两者中必有一个成立）
    for (int i = n - 1; i >= 1; --i) {
        a[i] = a[i + 1] + range[i][i + 1];
        if (!rowOk(i)) a[i] = a[i + 1] - range[i][i + 1];
    }

    // 题目只要求输出任意一个合法数组：整体平移不改变任何极差。
    // 这里把最小值平移到 0，输出就正好落在原数据的取值范围 [0, 10^9] 内
    long long shift = a[1];
    for (int i = 2; i <= n; ++i) {
        if (a[i] < shift) shift = a[i];
    }
    for (int i = 1; i <= n; ++i) {
        printf("%lld", a[i] - shift);
        printf(i == n ? "\n" : " ");
    }
    return 0;
}
