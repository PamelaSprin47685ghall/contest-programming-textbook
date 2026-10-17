#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 50005;

// 一头牛 5 个口味的某个非空子集: 口味按升序存放, 不足 5 个的位置补 0
// (口味编号都是正数, 所以 0 只作为占位符, 不会与真实口味冲突)
struct Subset {
    int v[5];
};

int n;
int fav[5];
Subset subsets[MAXN * 31];

bool cmpSubset(const Subset &a, const Subset &b) {
    for (int i = 0; i < 5; i++) {
        if (a.v[i] != b.v[i]) return a.v[i] < b.v[i];
    }
    return false;
}

bool sameSubset(const Subset &a, const Subset &b) {
    for (int i = 0; i < 5; i++) {
        if (a.v[i] != b.v[i]) return false;
    }
    return true;
}

int main() {
    scanf("%d", &n);
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 5; j++) scanf("%d", &fav[j]);
        // 先排序, 这样同一个子集在所有牛里表示完全一致, 可以直接比较相等
        sort(fav, fav + 5);
        for (int mask = 1; mask < 32; mask++) {
            int p = 0;
            for (int j = 0; j < 5; j++) {
                if (mask & (1 << j)) subsets[cnt].v[p++] = fav[j];
            }
            while (p < 5) subsets[cnt].v[p++] = 0;
            cnt++;
        }
    }
    sort(subsets, subsets + cnt, cmpSubset);

    // 容斥统计"至少共享一种口味"的牛对:
    // 大小为奇数的子集贡献 +C(c,2), 偶数贡献 -C(c,2)
    long long compatible = 0;
    int i = 0;
    while (i < cnt) {
        int j = i;
        while (j < cnt && sameSubset(subsets[i], subsets[j])) j++;
        long long c = j - i;
        int size = 0;
        for (int t = 0; t < 5; t++) {
            if (subsets[i].v[t] != 0) size++;
        }
        long long pairs = c * (c - 1) / 2;
        if (size % 2 == 1) compatible += pairs;
        else compatible -= pairs;
        i = j;
    }

    long long total = (long long)n * (n - 1) / 2;
    printf("%lld\n", total - compatible);
    return 0;
}
