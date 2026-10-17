#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100000;

int n;
long long k, l;         // 调查篇数与每篇调查的引用上限，k*l 最大可达 10^10
int c[MAXN];

// 引用数从大到小排序，这样「引用最多的 h 篇」就是前 h 个
bool cmpDesc(int a, int b) {
    return a > b;
}

// 判断能否把 h-index 提高到至少 h
// 只需考虑引用数最多的 h 篇论文：每篇需要补 max(0, h-c_i) 次引用
bool canAchieve(int h) {
    long long needed = 0;
    for (int i = 0; i < h; ++i) {
        int ci = c[i];
        if (ci + k < h) return false;       // 单篇论文最多被引用 k 次（每篇调查至多一次）
        if (ci < h) needed += h - ci;
    }
    return needed <= k * l;                 // 总共只有 k*l 次引用额度
}

int main() {
    scanf("%d%lld%lld", &n, &k, &l);
    for (int i = 0; i < n; ++i) scanf("%d", &c[i]);
    sort(c, c + n, cmpDesc);

    int lo = 0, hi = n;                     // h-index 显然不超过论文总数
    while (lo < hi) {
        int mid = (lo + hi + 1) / 2;
        if (canAchieve(mid)) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    printf("%d\n", lo);
    return 0;
}
