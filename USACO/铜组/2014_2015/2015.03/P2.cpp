#include <cstdio>

struct Variable {
    long long ways[2];  // 该表达式取值为 0 / 1 的赋值方法数。
};

// 异或：结果为 0 当且仅当两操作数相同。
Variable xorVariable(const Variable& v0, const Variable& v1) {
    long long w00 = v0.ways[0] * v1.ways[0];
    long long w01 = v0.ways[0] * v1.ways[1];
    long long w10 = v0.ways[1] * v1.ways[0];
    long long w11 = v0.ways[1] * v1.ways[1];
    return Variable{w00 + w11, w01 + w10};
}

// 与：结果为 1 当且仅当两操作数都是 1。
Variable andVariable(const Variable& v0, const Variable& v1) {
    long long w00 = v0.ways[0] * v1.ways[0];
    long long w01 = v0.ways[0] * v1.ways[1];
    long long w10 = v0.ways[1] * v1.ways[0];
    long long w11 = v0.ways[1] * v1.ways[1];
    return Variable{w00 + w01 + w10, w11};
}

int main() {
    int n;
    scanf("%d", &n);
    Variable variable[26] = {};
    for (int i = 0; i < n; ++i) {
        char c;
        int w;
        scanf(" %c%d", &c, &w);
        // 只关心奇偶性，偶数与偶数等价。
        ++variable[c - 'A'].ways[w & 1];
    }
    // 按题目表达式 (B^I) & (G^O) & (E^S) & M 逐层合并，最后取值为 1 的方法数。
    Variable v0 = xorVariable(variable['B' - 'A'], variable['I' - 'A']);
    Variable v1 = xorVariable(variable['G' - 'A'], variable['O' - 'A']);
    Variable v2 = xorVariable(variable['E' - 'A'], variable['S' - 'A']);
    Variable v3 = xorVariable(v1, v2);
    Variable v4 = andVariable(v0, v3);
    Variable v5 = andVariable(v4, variable['M' - 'A']);
    printf("%lld\n", v5.ways[0]);
    return 0;
}
