#include <cstdio>

using namespace std;

const int MAXN = 200005;    // 字符串长度上限
const int MAXSEG = 262144;  // 不小于 2e5 的 2 的幂，线段树的叶子数
const char KEY[] = "bessie";// 要找的模式串

char buf[MAXN];  // 输入的字符串 t

// 转移结构：把"用 token 计数算法扫过一段子串所发生的一切"压缩成 O(1) 信息。
// 设扫过之前下标 d 的计数为 v[d]，则扫过这段子串后：
//   位置 j 的计数 = Σ_{d: endsUpAt[d] == j} v[d] + fixedEndsUpAt[j]
//   答案的增量   = Σ_d coefficients[d] * v[d] + fixedContribution
// 为什么这么少的信息就够：
//   - 一批计数在扫描中始终整批一起移动，故只需记它最终落到哪个下标；
//   - 每当匹配完模式的第 6 个字符时，答案要加上 (n - 当前位置) × 该批计数，
//     位置是固定的，所以答案关于每批计数是线性的，只需记系数；
//   - 每个字符还会给下标 0 凭空加一个 1，这些"新来的"量与 v 无关，
//     所以另外记一组固定去向和固定贡献。
struct Transition {
    int endsUpAt[6];            // 扫前位于 d 的计数扫完后落在哪个下标
    long long coefficients[6];  // 该批计数在这段子串里贡献的答案系数
    long long fixedEndsUpAt[6]; // 串内部每字符 +1 的那些量最终落在哪个下标
    long long fixedContribution;// 串内部新加的量贡献的答案
};

Transition tree[2 * MAXSEG];

// 合并左右相邻两段（先扫 L 再扫 R）
Transition merge(const Transition &L, const Transition &R) {
    Transition res;
    for (int d = 0; d < 6; ++d) {
        res.endsUpAt[d] = 0;
        res.coefficients[d] = 0;
        res.fixedEndsUpAt[d] = 0;
    }
    res.fixedContribution = L.fixedContribution + R.fixedContribution;
    for (int d = 0; d < 6; ++d) {
        // L 里的第 d 批先被 L 送到 L.endsUpAt[d]，再被 R 送到 R.endsUpAt[...]
        res.endsUpAt[d] = R.endsUpAt[L.endsUpAt[d]];
        // 两段各自对这批计数的线性贡献相加
        res.coefficients[d] = L.coefficients[d] + R.coefficients[L.endsUpAt[d]];
        // L 内部新加的量要穿过 R；R 内部新加的量原地不动
        res.fixedEndsUpAt[R.endsUpAt[d]] += L.fixedEndsUpAt[d];
        res.fixedEndsUpAt[d] += R.fixedEndsUpAt[d];
        // L 内部新加的量在 R 里走一圈，也会贡献答案
        res.fixedContribution += R.coefficients[d] * L.fixedEndsUpAt[d];
    }
    return res;
}

// 单个字符 c 对应的转移：它在下标 idx 处，整串长度为 len
Transition makeLeaf(char c, int idx, int len) {
    Transition res;
    for (int d = 0; d < 6; ++d) {
        // 与模式第 d 位相同就把这一批计数推进一位，否则原地不动
        res.endsUpAt[d] = (c == KEY[d]) ? (d + 1) % 6 : d;
        res.coefficients[d] = 0;
        res.fixedEndsUpAt[d] = 0;
    }
    // 每个字符开始时会先给下标 0 加 1，这份 1 也要走这一格
    res.fixedEndsUpAt[res.endsUpAt[0]] = 1;
    res.fixedContribution = 0;
    // 若本字符正好匹配模式最后一位 'e'，那么此刻下标 5 的每一批计数
    // 都能以 idx 往后的任意位置作为子串结尾：共 (len - idx) 个起点/终点组合
    if (res.endsUpAt[5] == 0) res.coefficients[5] = len - idx;
    return res;
}

// 用于补齐线段树的单位元（什么都不做）
Transition makeIdentity() {
    Transition res;
    for (int d = 0; d < 6; ++d) {
        res.endsUpAt[d] = d;
        res.coefficients[d] = 0;
        res.fixedEndsUpAt[d] = 0;
    }
    res.fixedContribution = 0;
    return res;
}

int main() {
    scanf("%s", buf);
    int len = 0;
    while (buf[len] != '\0') ++len;

    int segN = 1;
    while (segN < len) segN <<= 1;
    for (int i = 0; i < segN; ++i) {
        if (i < len) tree[segN + i] = makeLeaf(buf[i], i, len);
        else tree[segN + i] = makeIdentity();
    }
    for (int i = segN - 1; i >= 1; --i) tree[i] = merge(tree[2 * i], tree[2 * i + 1]);

    printf("%lld\n", tree[1].fixedContribution);

    int U;
    scanf("%d", &U);
    for (int u = 0; u < U; ++u) {
        int p;
        char c;
        scanf("%d %c", &p, &c);
        --p;
        buf[p] = c;
        tree[segN + p] = makeLeaf(c, p, len);
        for (int i = (segN + p) >> 1; i >= 1; i >>= 1) tree[i] = merge(tree[2 * i], tree[2 * i + 1]);
        printf("%lld\n", tree[1].fixedContribution);
    }
    return 0;
}
